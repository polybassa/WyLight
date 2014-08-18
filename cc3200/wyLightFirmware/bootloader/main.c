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

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"

// SimpleLink includes
#include "simplelink.h"

// driverlib includes
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"
#include "interrupt.h"

// common interface includes
#include "uart_if.h"
#include "pinmux.h"
#include "network_if.h"
#include "wifi.h"

// wylight includes
#include "firmware_download.h"

#define UART_PRINT          Report
#define APP_NAME            "WyLight Bootloader"

#define FIRMWARE_ORIGIN 	0x20014000
#define BUFFER_SIZE 		1024

//
// GLOBAL VARIABLES -- Start
//
#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif

int g_iSockID;
//
// GLOBAL VARIABLES -- End
//

//****************************************************************************
//
//! Create an endpoint for communication and initiate connection on socket.*/
//!
//! \brief Create connection with server
//!
//! This function opens a socket and create the endpoint communication with server
//!
//! \param[in]      DestinationIP - IP address of the server
//!
//! \return         socket id for success and negative for error
//
//****************************************************************************
int CreateConnection(unsigned long DestinationIP) {
	SlSockAddrIn_t Addr;
	int Status = 0;
	int AddrSize = 0;
	int SockID = 0;

	Addr.sin_family = SL_AF_INET;
	Addr.sin_port = sl_Htons(80);
	Addr.sin_addr.s_addr = sl_Htonl(DestinationIP);

	AddrSize = sizeof(SlSockAddrIn_t);

	SockID = sl_Socket(SL_AF_INET, SL_SOCK_STREAM, 0);
	if (SockID < 0) {
		/* Error */
		UART_PRINT("Error while opening the socket\r\n");
		return -1;
	}

	Status = sl_Connect(SockID, (SlSockAddr_t *) &Addr, AddrSize);
	if (Status < 0) {
		/* Error */
		UART_PRINT("Error during connection with server\r\n");
		sl_Close(SockID);
		return -1;
	}
	return SockID;
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void DisplayBanner(char * AppName) {

	UART_PRINT("\n\n\n\r");
	UART_PRINT("\t\t *************************************************\n\r");
	UART_PRINT("\t\t	  CC3200 %s Application       \n\r", AppName);
	UART_PRINT("\t\t	        %s, %s          \n\r", __DATE__, __TIME__);
	UART_PRINT("\t\t *************************************************\n\r");
	UART_PRINT("\n\n\n\r");
}

//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void BoardInit(void) {
	/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
	//
	// Set vector table base
	//
#if defined(ccs) || defined(gcc)
	MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);
#endif
#if defined(ewarm)
	MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
	//
	// Enable Processor
	//
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();
}

static int LoadFirmware(unsigned char* sourceFile) {

	long sfileHandle = -1;
	unsigned long sToken = 0;
	long retVal = 0;
	unsigned long bytesCopied = 0, bytesReceived = 0;
	unsigned long readsize = 0;
	unsigned char buffer[BUFFER_SIZE];
	SlFsFileInfo_t sFileInfo;

	// get file size
	retVal = sl_FsGetInfo((unsigned char *) sourceFile, sToken, &sFileInfo);
	if (retVal < 0) {
		// File Doesn't exit create a new of 45 KB file
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}
	bytesReceived = sFileInfo.FileLen;

	// open the source file for reading
	retVal = sl_FsOpen((unsigned char *) sourceFile, FS_MODE_OPEN_READ, &sToken, &sfileHandle);
	if (retVal < 0) {
		// File Doesn't exit create a new of 45 KB file
		UART_PRINT("Error during opening the source file\r\n");
		return -1;
	}

	// Copy the files from temporary file to SRAM at FIRMWARE_ORIGIN
	while (bytesCopied < bytesReceived) {
		if ((bytesReceived - bytesCopied) > sizeof(buffer)) readsize = sizeof(buffer);
		else readsize = (bytesReceived - bytesCopied);

		memset(buffer, 0, sizeof(buffer));
		retVal = sl_FsRead(sfileHandle, bytesCopied, (unsigned char *) buffer, readsize);
		if (retVal < 0) {
			// Error close the file and delete the temporary file
			retVal = sl_FsClose(sfileHandle, 0, 0, 0);
			UART_PRINT("Error during reading the file\r\n");
			return -1;
		}
		memcpy((void *) (FIRMWARE_ORIGIN + bytesCopied), buffer, readsize);
		bytesCopied += readsize;
	}
	// Close the opened files
	retVal = sl_FsClose(sfileHandle, 0, 0, 0);
	return 0;
}

static void startFirmware(void) {

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

int main() {
	int retRes = -1;

	//
	// Board Initialization
	//
	BoardInit();

	//
	// Configure the pinmux settings for the peripherals exercised
	//
	PinMuxConfig();

	//
	// Configuring UART
	//
	InitTerm();

	//
	// Display banner
	//
	DisplayBanner(APP_NAME);

	Network_IF_InitDriver(ROLE_STA);

	// Connecting to WLAN AP - Set with static parameters defined at the top
	// After this call we will be connected and have IP address
	WlanConnect();

	UART_PRINT("Connected to the AP: %s\r\n", SSID_NAME);

	// Create a TCP connection to the Web Server
	g_iSockID = CreateConnection(GetServerIP());

	if (g_iSockID < 0) return -1;

	UART_PRINT("Connection to server created successfully\r\n");
	// Download the file, verify the file and replace the exiting file
	if (GetFileFromServer(g_iSockID, "firmware.bin", "/temp/firmware.bin") < 0) {
		retRes = sl_Close(g_iSockID);
		UART_PRINT("Error downloading file\r\n");
		return -1;
	}

	retRes = sl_Close(g_iSockID);
	if (0 > retRes) {
		UART_PRINT("Error during closing socket\r\n");
		return -1;
	}

	g_iSockID = CreateConnection(GetServerIP());

	if (g_iSockID < 0) return -1;

	UART_PRINT("Connection to server created successfully\r\n");
	if (GetFileFromServer(g_iSockID, "firmware.sha", "/temp/firmware.sha") < 0) {
		retRes = sl_Close(g_iSockID);
		UART_PRINT("Error downloading file\r\n");
		return -1;
	}

	retRes = sl_Close(g_iSockID);
	if (0 > retRes) {
		UART_PRINT("Error during closing socket\r\n");
		return -1;
	}

	retRes = VerifyFile("/temp/firmware.bin", "/temp/firmware.sha");
	if (retRes < 0) {
		UART_PRINT("Error verifying files\r\n");
	}
	/*retRes = ReplaceFile("/temp/firmware.bin", FILE_NAME);
	 if (retRes < 0) {
	 UART_PRINT("Error during replacing the file\r\n");
	 }*/

	UART_PRINT("\nThank you\r\n");
	// Stop the CC3200 device

	LoadFirmware((unsigned char *) "/temp/firmware.bin");
	startFirmware();
	sl_Stop(0xFF);

	LOOP_FOREVER(__LINE__);

}

