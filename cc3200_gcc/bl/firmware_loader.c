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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "hw_types.h"
#include "hw_shamd5.h"
#include "hw_memmap.h"
#include "hw_nvic.h"
#include "shamd5.h"
#include "prcm.h"

#include "uart_if.h"
#include "firmware_loader.h"
#include "wy_bl_network_if.h"
#include "bootloader.h"

#ifdef SIMULATOR
#include "simulator.h"
#include "fs.h"
#else
#include "simplelink.h"
#include "rom_map.h"
#include "hw_ints.h"
#include "interrupt.h"
#endif /* SIMULATOR */

#define BUFFER_SIZE 			1024
#define BLOCKSIZE		 		64 		/* Write block size for write to MD5SHA module */
#define CHECKSUM_SIZE			32		/* In Bytes, We use SHA-256 */
#define SUCCESS					0
#define FILENAME_SIZE			128
#define UART_PRINT          	Report

//
// GLOBAL VARIABLES -- Start
//
// Flags to check that interrupts were successfully generated.

volatile struct SHAMD5_StatusFlags {
	unsigned int ContextReadyFlag :1;
	unsigned int ParthashReadyFlag :1;
	unsigned int InputReadyFlag :1;
	unsigned int OutputReadyFlag :1;
} g_SHAMD5_StatusFlags;

#ifdef SIMULATOR
static uint8_t memory[0x3FFFF];
#undef FIRMWARE_ORIGIN
#define FIRMWARE_ORIGIN (void*)&memory[0]
static unsigned char* FIRMWARE_FILENAME = (unsigned char *) "firmware.bin";
#else /* SIMULATOR */
static unsigned char* FIRMWARE_FILENAME = (unsigned char *) FW_FILENAME;
#endif /* SIMULATOR */

//
// GLOBAL VARIABLES -- End
//

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
void SHAMD5IntHandler(void) {
	uint32_t ui32IntStatus;
	//
	// Read the SHA/MD5 masked interrupt status.
	//
	ui32IntStatus = MAP_SHAMD5IntStatus(SHAMD5_BASE, true);
	if (ui32IntStatus & SHAMD5_INT_CONTEXT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY);
		g_SHAMD5_StatusFlags.ContextReadyFlag = 1;
	}
	if (ui32IntStatus & SHAMD5_INT_PARTHASH_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_PARTHASH_READY);
		g_SHAMD5_StatusFlags.ParthashReadyFlag = 1;
	}
	if (ui32IntStatus & SHAMD5_INT_INPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_INPUT_READY);
		g_SHAMD5_StatusFlags.InputReadyFlag = 1;
	}
	if (ui32IntStatus & SHAMD5_INT_OUTPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_OUTPUT_READY);
		g_SHAMD5_StatusFlags.OutputReadyFlag = 1;
	}
}

//****************************************************************************
//
//! \brief ComputeSHAFromSRAM
//!
//! This function computes the SHA256 from a given memory block
//!
//! \param[in]      pSource -- Pointer to startadress of the memory
//! \param[in]		length -- Length of the memory block
//! \param[out]		resultHash -- Pointer to a 20 byte array, to store computed hash
//
//****************************************************************************
static void ComputeSHAFromSRAM(uint8_t *pSource, const size_t length, uint8_t *resultHash) {

	if (length == 0) {
		return;
	}

	UART_PRINT("Start computing hash\r\n");
	//clear flags
	g_SHAMD5_StatusFlags.ContextReadyFlag = 0;
	g_SHAMD5_StatusFlags.InputReadyFlag = 0;

	//Enable MD5SHA module
	PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
	MAP_SHAMD5IntRegister(SHAMD5_BASE, SHAMD5IntHandler);

	//reset modul
	PRCMPeripheralReset(PRCM_DTHE);

	//Enable Interrupts
	SHAMD5IntEnable(SHAMD5_BASE,
	SHAMD5_INT_CONTEXT_READY | SHAMD5_INT_PARTHASH_READY | SHAMD5_INT_INPUT_READY | SHAMD5_INT_OUTPUT_READY);

	//wait for context ready flag.
	while (!g_SHAMD5_StatusFlags.ContextReadyFlag)
		_SlNonOsMainLoopTask();
	;

	//Configure SHA/MD5 module
	SHAMD5ConfigSet(SHAMD5_BASE, SHAMD5_ALGO_SHA256);

	SHAMD5DataLengthSet(SHAMD5_BASE, (uint32_t) length);

	size_t bytesRead = 0, readsize = 0;

	while (bytesRead < length) {
		if ((length - bytesRead) > BLOCKSIZE) readsize = BLOCKSIZE;
		else readsize = (length - bytesRead);

		SHAMD5DataWrite(SHAMD5_BASE, pSource);
		bytesRead += readsize;
		pSource += readsize;
	}

	UART_PRINT("Computing hash successful\r\n");
	// return result
	SHAMD5ResultRead(SHAMD5_BASE, resultHash);
	// disable Interrupts
	SHAMD5IntDisable(SHAMD5_BASE,
	SHAMD5_INT_CONTEXT_READY | SHAMD5_INT_PARTHASH_READY | SHAMD5_INT_INPUT_READY | SHAMD5_INT_OUTPUT_READY);
	// disable MD5SHA module
	PRCMPeripheralClkDisable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
}

//****************************************************************************
//
//! \brief VerifySRAM
//!
//! This function computes the SHA256 from pSource to pSource + length - SHA256_SIZE
//! SRAM must contains the binary with it's SHA256 at the end. The last 20 Bytes are SHA256 Checksum
//!
//! \param[in]      pSource -- Pointer to startadress of the binary
//! \param[in]		length -- Length of the binary plus checksum in bytes
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
static long VerifySRAM(uint8_t *pSource, const size_t length) {
	uint8_t firstHash[CHECKSUM_SIZE], secoundHash[CHECKSUM_SIZE];

	if (length < CHECKSUM_SIZE) {
		return ERROR;
	}

	// get hash of file to verify
	ComputeSHAFromSRAM(pSource, length - CHECKSUM_SIZE, firstHash);

	//get hash from end of SRAM
	const void *pHash = pSource + length - CHECKSUM_SIZE;
	if ((uint32_t) pHash <= (uint32_t) pSource) {
		return ERROR;
	}
	memcpy(secoundHash, pHash, CHECKSUM_SIZE);

	if (memcmp(secoundHash, firstHash, sizeof(firstHash)) != 0) {
		UART_PRINT("\r\nHash-Sums are different\r\n");
		return ERROR;
	}
	return SUCCESS;
}

//****************************************************************************
//
//! \brief LoadFirmware
//!
//! This function copies the content of a given file in the SRAM at adress FIRMWARE_ORIGIN
//!
//! \param[in]      pSourceFile -- Filename of the file to copy
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
static long LoadFirmware(unsigned char* pSourceFile) {
	SlFsFileInfo_t sFileInfo;
	// get file size
	if (sl_FsGetInfo(pSourceFile, 0, &sFileInfo)) {
		UART_PRINT("Error during opening the source file\r\n");
		return ERROR;
	}

	long fileHandle = -1;
	unsigned long token = 0;
	// open the source file for reading
	if (sl_FsOpen(pSourceFile, FS_MODE_OPEN_READ, &token, &fileHandle)) {
		// File Doesn't exit
		UART_PRINT("Error during opening the source file\r\n");
		return ERROR;
	}
	size_t bytesCopied = sl_FsRead(fileHandle, 0, (unsigned char *) FIRMWARE_ORIGIN, sFileInfo.FileLen);
	// Close the opened files
	if (sl_FsClose(fileHandle, 0, 0, 0)) {
		return ERROR;
	}
	return bytesCopied;
}

//****************************************************************************
//
//! \brief SaveSRAMContentAsFirmware
//!
//! This function verify a given memory block and save this memory block as firmware file in FLASH
//!
//! \param[in]      pSource -- Pointer to startadress of the memory
//! \param[in]		length -- Length of the memory block
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
long SaveSRAMContent(uint8_t *pSource, const size_t length) {
	long fileHandle = -1;
	unsigned long token = 0;

	if (length < FILENAME_SIZE) {
		return ERROR;
	}

	unsigned char filename[FILENAME_SIZE];
	memcpy(filename, pSource, sizeof(filename));

	// set spaces to NULL
	unsigned char *pTemp = filename;
	while (*pTemp != ' ')
		pTemp++;
	*pTemp = 0x00;

	pSource += FILENAME_SIZE;
	size_t filesize = length - FILENAME_SIZE;

	if (VerifySRAM(pSource, filesize)) {
		UART_PRINT("Invalid SHA256SUM\r\n");
		return ERROR;
	}

	// if filename indicates a webdata, remove checksum at the end
	// checksum at the end should not be delivered to a browser
	const char webSubdirectory[] = "/www/";
	// normally we should use strstr here, but this will need a lot more code than memcmp.
	// the websubdirectory string will always be at the beginning. so we can compare sizeof(webSubdirectory) minus trailing NULL
	if (0 == memcmp(filename, webSubdirectory, sizeof(webSubdirectory) - 1)) {
		filesize -= CHECKSUM_SIZE;
	}

	// Delete old Firmware
	sl_FsDel(filename, token);

	// Save Firmware
	UART_PRINT("\r\nStarted saving %s\r\n", filename);
	// open a user file for writing
	if (sl_FsOpen(filename, FS_MODE_OPEN_WRITE, &token, &fileHandle)) {
		// File Doesn't exit create a new file
		if (sl_FsOpen(filename,
				FS_MODE_OPEN_CREATE(filesize,
						_FS_FILE_OPEN_FLAG_COMMIT | _FS_FILE_PUBLIC_WRITE | _FS_FILE_PUBLIC_READ
								| _FS_FILE_OPEN_FLAG_VENDOR), &token, &fileHandle)) {
			sl_FsDel(filename, token);
			UART_PRINT("Error during creating the destination file\r\n");
			return ERROR;
		}
		UART_PRINT("File %s created\r\n", filename);
	}

	if (sl_FsWrite(fileHandle, 0, pSource, filesize) < 0) {
		// Error close the file and delete the temporary file
		sl_FsClose(fileHandle, 0, 0, 0);
		UART_PRINT("Error during writing the file\r\n");
		return ERROR;
	}
	// Close the opened files
	sl_FsClose(fileHandle, 0, 0, 0);
	// if we saved a firmware, than return SUCCESS (0) to start this firmware immediately
	return (long) strcmp((const char *) filename, (const char *) FIRMWARE_FILENAME);
}

//****************************************************************************
//
//! \brief StartFirmware
//!
//! This function patches the processors vector table and jumps to the new reset vector
//
//****************************************************************************
static void StartFirmware(void) {
	Network_IF_DeInitDriver();
	MAP_IntDisable(FAULT_SYSTICK);
	MAP_IntMasterDisable();
	// patch Interrupt Vector Table
	MAP_IntVTableBaseSet((size_t)FIRMWARE_ORIGIN);

	// call Firmware
	void (*firmware_origin_entry)(void);
	unsigned int resetVector = *((unsigned int *) (FIRMWARE_ORIGIN + 4));
	firmware_origin_entry = (void (*)(void)) (resetVector);

	firmware_origin_entry();
}

//****************************************************************************
//
//! \brief LoadAndExecuteFirmware
//!
//! This function loads and executes a firmware file from FLASH
//!
//! \return         0 for success and negative for error
//
//****************************************************************************
long LoadAndExecuteFirmware(void) {
	// retVal contains length of Firmware after load
	long fw_length = LoadFirmware(FIRMWARE_FILENAME);
	if (fw_length <= 0) {
		return ERROR;
	}

	if (SUCCESS == VerifySRAM(FIRMWARE_ORIGIN, fw_length)) {
		UART_PRINT("Starting Firmware\r\n");
		StartFirmware();
	}
	return ERROR;
}
