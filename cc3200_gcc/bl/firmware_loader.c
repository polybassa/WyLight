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
#include "hw_ints.h"
#include "interrupt.h"
#include "prcm.h"
#include "rom_map.h"
#include "shamd5.h"

#include "uart_if.h"
#include "firmware_loader.h"
#include "wy_bl_network_if.h"
#include "bootloader.h"


#define BUFFER_SIZE 			1024
#define BLOCKSIZE		 		64 		/* Write block size for write to MD5SHA module */
#define CHECKSUM_SIZE			32		/* In Bytes, We use SHA-256 */
#define SUCCESS					0
#define FILENAME_SIZE			128


static int g_ContextReadyFlag = 0;

static unsigned char* FIRMWARE_FILENAME = (unsigned char *) FW_FILENAME;


/**
 * Interrupt handler to handle SHAMD5 engine interrupts
 */
void SHAMD5IntHandler(void) {
	uint32_t ui32IntStatus;
	
	ui32IntStatus = MAP_SHAMD5IntStatus(SHAMD5_BASE, true);
	if (ui32IntStatus & SHAMD5_INT_CONTEXT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_CONTEXT_READY);
		g_ContextReadyFlag = 1;
	}
	if (ui32IntStatus & SHAMD5_INT_PARTHASH_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_PARTHASH_READY);
	}
	if (ui32IntStatus & SHAMD5_INT_INPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_INPUT_READY);
	}
	if (ui32IntStatus & SHAMD5_INT_OUTPUT_READY) {
		MAP_SHAMD5IntDisable(SHAMD5_BASE, SHAMD5_INT_OUTPUT_READY);
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
	g_ContextReadyFlag = 0;

	//Enable MD5SHA module
	PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
	MAP_SHAMD5IntRegister(SHAMD5_BASE, SHAMD5IntHandler);

	//reset modul
	PRCMPeripheralReset(PRCM_DTHE);

	//Enable Interrupts
	SHAMD5IntEnable(SHAMD5_BASE,
	SHAMD5_INT_CONTEXT_READY | SHAMD5_INT_PARTHASH_READY | SHAMD5_INT_INPUT_READY | SHAMD5_INT_OUTPUT_READY);

	//wait for context ready flag.
	while (!g_ContextReadyFlag) {
		_SlNonOsMainLoopTask();
	}

	//Configure SHA/MD5 module
	MAP_SHAMD5ConfigSet(SHAMD5_BASE, SHAMD5_ALGO_SHA256);
	
	MAP_SHAMD5DataProcess(SHAMD5_BASE, pSource, length, resultHash);

	UART_PRINT("Computing hash successful\r\n");
	
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
//! \return         0 for success
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

	return memcmp(secoundHash, firstHash, sizeof(firstHash));
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

/**
 * Load, verify and run the firmware file from flash
 * \return ERROR, if this function was successful it will not return
 */
long LoadAndExecuteFirmware(void) {
	// retVal contains length of Firmware after load
	long fw_length = LoadFirmware(FIRMWARE_FILENAME);
	if (fw_length <= 0) {
		return ERROR;
	}

	if (VerifySRAM(FIRMWARE_ORIGIN, fw_length)) {
		UART_PRINT("Invalid SHA256SUM\r\n");
		return ERROR;
	}
	UART_PRINT("Starting Firmware\r\n");
	StartFirmware();
	return ERROR;
}
