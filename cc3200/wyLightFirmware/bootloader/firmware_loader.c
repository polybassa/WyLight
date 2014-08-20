/*
 Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

 This file is part of Wifly_Light.

 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "firmware_loader.h"

#include <stdint.h>
#include <stdbool.h>

#include "shamd5.h"
#include "hw_shamd5.h"
#include "simplelink.h"
#include "uart_if.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "rom_map.h"
#include "prcm.h"
#include "wy_bl_network_if.h"
//#include "utils.h"

#define MAX_BUFF_SIZE  			1460
#define READ_SIZE   			1450
#define BLOCKSIZE		 		64 /* Write block size for write to MD5SHA module */
#define SHA1_SIZE				20
#define SUCCESS					0

#define UART_PRINT          	Report

//
// GLOBAL VARIABLES -- Start
//
// Flags to check that interrupts were successfully generated.
volatile bool g_bContextReadyFlag;
volatile bool g_bParthashReadyFlag;
volatile bool g_bInputReadyFlag;
volatile bool g_bOutputReadyFlag;
int g_iSockID;
//
// GLOBAL VARIABLES -- End
//

//
// FUNCTION PROTOTYPES -- Start
//
static void SHAMD5IntHandler(void);
static void ComputeSHAFromSRAM(const uint8_t *pSource, const unsigned long length, uint8_t *resultHash);
static int ComputeSHAFromFile(const unsigned char *sourceFile, uint8_t *resultHash);
static int VerifyFile(const unsigned char *fileToVerify, const unsigned char *fileWithSHAHash);
static int VerifySRAM(const unsigned char *pSource, const unsigned long length, const unsigned char *fileWithSHAHash);
static long LoadFirmware(const unsigned char* sourceFile);
//
// FUNCTION PROTOTYPES -- End
//
//*****************************************************************************

//*****************************************************************************
//
//! SHAMD5IntHandler - Interrupt Handler which handles different interrupts from
//! different sources
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
static void SHAMD5IntHandler(void) {
	uint32_t ui32IntStatus;
	//
	// Read the SHA/MD5 masked interrupt status.
	//
	ui32IntStatus = MAP_SHAMD5IntStatus(SHAMD5_BASE, true);
	if (ui32IntStatus & SHAMD5_INT_CONTEXT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY);
		g_bContextReadyFlag = true;
	}
	if (ui32IntStatus & SHAMD5_INT_PARTHASH_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_PARTHASH_READY);
		g_bParthashReadyFlag = true;
	}
	if (ui32IntStatus & SHAMD5_INT_INPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_INPUT_READY);
		g_bInputReadyFlag = true;
	}
	if (ui32IntStatus & SHAMD5_INT_OUTPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_OUTPUT_READY);
		g_bOutputReadyFlag = true;
	}
}

static void ComputeSHAFromSRAM(const uint8_t *pSource, const unsigned long length, uint8_t *resultHash) {
	unsigned long bytesRead = 0, readsize = 0;

	UART_PRINT("\r\nStarted comuting hash\r\n");

	//Enable MD5SHA module
	PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
	MAP_SHAMD5IntRegister(SHAMD5_BASE, SHAMD5IntHandler);

	//reset modul
	PRCMPeripheralReset(PRCM_DTHE);

	//clear flags
	g_bContextReadyFlag = false;
	g_bInputReadyFlag = false;

	//Enable Interrupts
	SHAMD5IntEnable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY | SHAMD5_INT_PARTHASH_READY | SHAMD5_INT_INPUT_READY | SHAMD5_INT_OUTPUT_READY);

	//wait for context ready flag.
	while (!g_bContextReadyFlag)
		;

	//Configure SHA/MD5 module
	SHAMD5ConfigSet(SHAMD5_BASE, SHAMD5_ALGO_SHA1);

	SHAMD5DataLengthSet(SHAMD5_BASE, (uint32_t) length);

	while (bytesRead < length) {
		if ((length - bytesRead) > BLOCKSIZE) readsize = BLOCKSIZE;
		else readsize = (length - bytesRead);

		SHAMD5DataWrite(SHAMD5_BASE, (uint8_t*) pSource);
		bytesRead += readsize;
		pSource += readsize;
	}

	UART_PRINT("Hash successfully\r\n");
	SHAMD5ResultRead(SHAMD5_BASE, resultHash);
}

//****************************************************************************
//
//! \brief Compute SHA1SUM
//!
//! This function computes the SHA1SUM of a given file
//!
//! \param[in]      sourceFile -- Source File of a file in the flash
//! \param[out]		resultHash -- Pointer to a 20 Byte Array for the result
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
static int ComputeSHAFromFile(const unsigned char *sourceFile, uint8_t *resultHash) {
	long sfileHandle = -1;
	unsigned long sToken = 0;
	long retVal = 0;
	unsigned long bytesRead = 0;
	unsigned long readsize = 0;
	uint8_t buffer[BLOCKSIZE];
	SlFsFileInfo_t sFileInfo;

	UART_PRINT("\r\nStarted computing hash\r\n");

	//Enable MD5SHA module
	PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
	MAP_SHAMD5IntRegister(SHAMD5_BASE, SHAMD5IntHandler);

	//reset modul
	PRCMPeripheralReset(PRCM_DTHE);

	//clear flags
	g_bContextReadyFlag = false;
	g_bInputReadyFlag = false;

	//Enable Interrupts
	SHAMD5IntEnable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY | SHAMD5_INT_PARTHASH_READY | SHAMD5_INT_INPUT_READY | SHAMD5_INT_OUTPUT_READY);

	//wait for context ready flag.
	while (!g_bContextReadyFlag)
		;

	//Configure SHA/MD5 module
	SHAMD5ConfigSet(SHAMD5_BASE, SHAMD5_ALGO_SHA1);

	// open the source file for reading
	retVal = sl_FsOpen((unsigned char *) sourceFile, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (retVal < 0) {
		sl_FsClose(sfileHandle, NULL, NULL, 0);
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}

	// get file size
	retVal = sl_FsGetInfo((unsigned char *) sourceFile, sToken, &sFileInfo);
	if (retVal < 0) {
		// File Doesn't exit create a new of 45 KB file
		sl_FsClose(sfileHandle, NULL, NULL, 0);
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}

	SHAMD5DataLengthSet(SHAMD5_BASE, (uint32_t) sFileInfo.FileLen);

	// Copy the files from temporary file to original file
	// If user file has checksum which can be used to verify the temporary
	// file then file should be verified before copying
	while (bytesRead < sFileInfo.FileLen) {
		if ((sFileInfo.FileLen - bytesRead) > BLOCKSIZE) readsize = BLOCKSIZE;
		else readsize = (sFileInfo.FileLen - bytesRead);

		memset(buffer, 0, sizeof(buffer));
		retVal = sl_FsRead(sfileHandle, bytesRead, (unsigned char *) buffer, readsize);
		if (retVal < 0) {
			// Error close the file and delete the temporary file
			retVal = sl_FsClose(sfileHandle, 0, 0, 0);
			UART_PRINT("Error during reading the file\r\n");
			return -1;
		}
		SHAMD5DataWrite(SHAMD5_BASE, buffer);
		bytesRead += readsize;
	}

	// Close the opened files
	retVal = sl_FsClose(sfileHandle, 0, 0, 0);
	if (retVal < 0) {
		// Error close the file and delete the temporary file
		UART_PRINT("Error during close the file\r\n");
		return -1;
	}
	UART_PRINT("Hash successfully\r\n");
	SHAMD5ResultRead(SHAMD5_BASE, resultHash);
	return 0;
}

//****************************************************************************
//
//! \brief Verify a File
//!
//! This function computes the checksum for a given file and compare's it with a
//! hash sum from a secound file
//! The secound file is generated from terminal sha1sum
//! Filecreation: sha1sum -b someBinary.bin > someBinary.sha
//!
//! \param[in]      sourceFile -- Source File of a file in the flash
//! \param[out]		resultHash -- Pointer to a 20 Byte Array for the result
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
static int VerifyFile(const unsigned char *fileToVerify, const unsigned char *fileWithSHAHash) {
	unsigned long sToken = 0;
	long sfileHandle = -1;
	uint8_t firstHash[SHA1_SIZE];
	uint8_t secoundHash[SHA1_SIZE];
	long retVal;
	memset(firstHash, 0, sizeof(firstHash));
	memset(secoundHash, 0, sizeof(secoundHash));

	// get hash of file to verify
	if (ComputeSHAFromFile(fileToVerify, firstHash) < 0) {
		UART_PRINT("Error computing SHA1SUM of %s\r\n", fileToVerify);
		return ERROR;
	}

	// open the source file for reading
	retVal = sl_FsOpen((unsigned char *) fileWithSHAHash, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (retVal < 0) {
		UART_PRINT("Error during opening the source file %s\r\n", fileWithSHAHash);
		return ERROR;
	}

	// transform the hash from file with sha hash to byte values. The Sha1Sum is decoded in ascii chars

	retVal = sl_FsRead(sfileHandle, 0, (unsigned char *) secoundHash, SHA1_SIZE);
	if (retVal != SHA1_SIZE) {
		sl_FsClose(sfileHandle, 0, 0, 0);
		return ERROR;
	}

	// Close the opened files
	retVal = sl_FsClose(sfileHandle, 0, 0, 0);
	if (retVal < 0) {
		// Error close the file and delete the temporary file
		UART_PRINT("Error during close the file\r\n");
		return ERROR;
	}

	if (memcmp(secoundHash, firstHash, sizeof(firstHash)) != 0) {
		UART_PRINT("\r\nHash-Sums are different\r\n");
		return ERROR;
	}
	return SUCCESS;
}

static int VerifySRAM(const unsigned char *pSource, const unsigned long length, const unsigned char *fileWithSHAHash) {
	unsigned long sToken = 0;
	long sfileHandle = -1;
	uint8_t firstHash[SHA1_SIZE], secoundHash[SHA1_SIZE];
	long retVal;
	memset(firstHash, 0, sizeof(firstHash));
	memset(secoundHash, 0, sizeof(secoundHash));

	// get hash of file to verify
	ComputeSHAFromSRAM(pSource, length, firstHash);

	// open the source file for reading
	retVal = sl_FsOpen((unsigned char *) fileWithSHAHash, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (retVal < 0) {
		UART_PRINT("Error during opening the source file %s\r\n", fileWithSHAHash);
		return ERROR;
	}

	// transform the hash from file with sha hash to byte values. The Sha1Sum is decoded in ascii chars

	retVal = sl_FsRead(sfileHandle, 0, (unsigned char *) secoundHash, SHA1_SIZE);
	if (retVal != SHA1_SIZE) {
		sl_FsClose(sfileHandle, 0, 0, 0);
		return ERROR;
	}

	// Close the opened files
	retVal = sl_FsClose(sfileHandle, 0, 0, 0);
	if (retVal < 0) {
		// Error close the file and delete the temporary file
		UART_PRINT("Error during close the file\r\n");
		return ERROR;
	}

	if (memcmp(secoundHash, firstHash, sizeof(firstHash)) != 0) {
		UART_PRINT("\r\nHash-Sums are different\r\n");
		return ERROR;
	}
	return SUCCESS;
}

static long LoadFirmware(const unsigned char* sourceFile) {
	long sfileHandle = -1;
	unsigned long sToken = 0;
	long lRetVal = -1;
	unsigned long bytesCopied = 0;
	unsigned long readsize = 0;
	unsigned char buffer[BUFFER_SIZE];
	SlFsFileInfo_t sFileInfo;

	// get file size
	lRetVal = sl_FsGetInfo((unsigned char *) sourceFile, sToken, &sFileInfo);
	if (lRetVal < 0) {
		// File Doesn't exit create a new of 45 KB file
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}

	// open the source file for reading
	lRetVal = sl_FsOpen((unsigned char *) sourceFile, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (lRetVal < 0) {
		// File Doesn't exit
		UART_PRINT("Error during opening the source file\r\n");
		return ERROR;
	}
	// Copy the files from temporary file to SRAM at FIRMWARE_ORIGIN
	while (bytesCopied < sFileInfo.FileLen) {
		if ((sFileInfo.FileLen - bytesCopied) > sizeof(buffer)) readsize = sizeof(buffer);
		else readsize = (sFileInfo.FileLen - bytesCopied);

		memset(buffer, 0, sizeof(buffer));
		lRetVal = sl_FsRead(sfileHandle, bytesCopied, (unsigned char *) buffer, readsize);
		if (lRetVal < 0) {
			// Error close the file and delete the temporary file
			lRetVal = sl_FsClose(sfileHandle, 0, 0, 0);
			UART_PRINT("Error during reading the file\r\n");
			return -1;
		}
		memcpy((void *) (FIRMWARE_ORIGIN + bytesCopied), buffer, readsize);
		bytesCopied += readsize;
	}
	// Close the opened files
	lRetVal = sl_FsClose(sfileHandle, 0, 0, 0);
	if (lRetVal != SUCCESS) {
		return ERROR;
	}
	return bytesCopied;
}

long SaveSRAMContentAsFirmware(const unsigned char *pSource, const unsigned long length) {
	long lRetVal = -1;
	long dfileHandle = -1;
	unsigned long dToken = 0;

	const unsigned char *pSHA1SUM;
	pSHA1SUM = pSource + length - SHA1_SIZE;

	UART_PRINT("\r\nStarted saving SHASUM\r\n");
	// open a user file for writing
	lRetVal = sl_FsOpen((unsigned char *) SHA1SUM_FILENAME, FS_MODE_OPEN_WRITE, &dToken, &dfileHandle);
	if (lRetVal < 0) {
		// File Doesn't exit create a new of 20 B file
		lRetVal = sl_FsOpen((unsigned char *) SHA1SUM_FILENAME,
				FS_MODE_OPEN_CREATE(SHA1_SIZE, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), &dToken,
				&dfileHandle);
		if (lRetVal < 0) {
			lRetVal = sl_FsDel((unsigned char *) SHA1SUM_FILENAME, dToken);
			UART_PRINT("Error during opening the destination file\r\n");
			return -1;
		}
	}
	lRetVal = sl_FsWrite(dfileHandle, 0, (unsigned char *) pSHA1SUM, SHA1_SIZE);
	if (lRetVal < 0) {
		// Error close the file and delete the temporary file
		lRetVal = sl_FsClose(dfileHandle, 0, 0, 0);
		UART_PRINT("Error during writing the file\r\n");
		return -1;
	}

	// Close the opened files
	lRetVal = sl_FsClose(dfileHandle, 0, 0, 0);
	UART_PRINT("Write SHA1 SUM successfully\r\n");

	if (SUCCESS != VerifySRAM(pSource, length - SHA1_SIZE, (const unsigned char *) SHA1SUM_FILENAME)) {
		UART_PRINT("Invalid SHA1SUM");
		return -1;
	}

	// Delete old Firmware
	sl_FsDel((unsigned char *) FIRMWARE_FILENAME, dToken);

	// Save Firmware
	UART_PRINT("\r\nStarted saving Firmware\r\n");
	// open a user file for writing
	lRetVal = sl_FsOpen((unsigned char *) FIRMWARE_FILENAME, FS_MODE_OPEN_WRITE, &dToken, &dfileHandle);
	if (lRetVal < 0) {
		// File Doesn't exit create a new of 20 B file
		lRetVal = sl_FsOpen((unsigned char *) FIRMWARE_FILENAME,
				FS_MODE_OPEN_CREATE(length, _FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ | _FS_FILE_OPEN_FLAG_VENDOR), &dToken, &dfileHandle);
		if (lRetVal < 0) {
			lRetVal = sl_FsDel((unsigned char *) FIRMWARE_FILENAME, dToken);
			UART_PRINT("Error during opening the destination file\r\n");
			return -1;
		}
	}
	lRetVal = sl_FsWrite(dfileHandle, 0, (unsigned char *) pSource, length - SHA1_SIZE);
	if (lRetVal < 0) {
		// Error close the file and delete the temporary file
		lRetVal = sl_FsClose(dfileHandle, 0, 0, 0);
		UART_PRINT("Error during writing the file\r\n");
		return -1;
	}
	// Close the opened files
	lRetVal = sl_FsClose(dfileHandle, 0, 0, 0);
	return 0;
}

void StartFirmware(void) {
	Network_IF_DeInitDriver();

	// patch Interrupt Vector Table
	unsigned int *pVectorTableOffset;
	pVectorTableOffset = (unsigned int *) 0xe000ed08;
	*pVectorTableOffset = FIRMWARE_ORIGIN;

	// call Firmware
	void (*firmware_origin_entry)(void);
	unsigned int resetVector = *((unsigned int *) (FIRMWARE_ORIGIN + 4));
	firmware_origin_entry = (void (*)(void)) (resetVector);

	firmware_origin_entry();
}

int LoadAndExecuteFirmware(void) {
	long lRetVal = -1;

	lRetVal = VerifyFile((const unsigned char *) FIRMWARE_FILENAME, (const unsigned char *) SHA1SUM_FILENAME);
	if (lRetVal != SUCCESS) {
		return ERROR;
	}

	lRetVal = LoadFirmware((const unsigned char *) FIRMWARE_FILENAME);
	if (lRetVal <= 0) {
		return ERROR;
	}

	lRetVal = VerifySRAM((unsigned char *) FIRMWARE_ORIGIN, lRetVal, (const unsigned char *) SHA1SUM_FILENAME);
	if (lRetVal != SUCCESS) {
		return ERROR;
	}
	UART_PRINT("Starting Firmware\r\n");
	StartFirmware();

	//never reached;
	return ERROR;
}
