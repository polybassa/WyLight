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
#include "wy_bl_network_if.h"
#include "gpio_if.h"

// wylight includes
#include "firmware_loader.h"

#define UART_PRINT          Report

//
// GLOBAL VARIABLES -- Start
//
extern void (* const g_pfnVectors[])(void);

static const unsigned short SERVER_PORT = 2000;
static const char APP_NAME[] = "WyLight Bootloader";
static const unsigned long BOOTLOADER_VERSION = 0;
//
// GLOBAL VARIABLES -- End
//

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void DisplayBanner(const char * AppName) {

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
	//
	// Set vector table base
	//
	MAP_IntVTableBaseSet((unsigned long) &g_pfnVectors[0]);

	//
	// Enable Processor
	//
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();
}

static int ReadJumper() {
	unsigned int GPIOPort;
	unsigned char GPIOPin;

//Read GPIO
	GPIO_IF_GetPortNPin(SH_GPIO_3, &GPIOPort, &GPIOPin);
	return GPIO_IF_Get(SH_GPIO_3, GPIOPort, GPIOPin);
}

#define BUFFERSIZE 1024

void TcpServer(void) {
	uint8_t welcomeMessage[30];
	memset(welcomeMessage, 0, sizeof(welcomeMessage));
	unsigned long nBootloaderVersion = htonl(BOOTLOADER_VERSION);
	memcpy(welcomeMessage, &nBootloaderVersion, sizeof(nBootloaderVersion));
	memcpy((char *) &welcomeMessage[4], APP_NAME, sizeof(APP_NAME));

	sockaddr_in RemoteAddr;
	socklen_t RemoteAddrLen = sizeof(sockaddr_in);
	uint8_t buffer[BUFFERSIZE];

	sockaddr_in LocalAddr;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(SERVER_PORT);
	LocalAddr.sin_addr.s_addr = htonl(0);
	int SocketTcpServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SocketTcpServer < 0) {
		UART_PRINT(" Socket Error: %d \r\n", SocketTcpServer);
		return;
	}

	int nonBlocking = 1;
	if (SUCCESS != setsockopt(SocketTcpServer, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking))) {
		UART_PRINT(" Setsockopt ERROR \r\n");
		close(SocketTcpServer);
		return;
	}

	if (SUCCESS != bind(SocketTcpServer, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
		UART_PRINT(" Bind Error\n\r");
		close(SocketTcpServer);
		return;
	}
	// Backlog = 1 to accept maximal 1 connection
	if (SUCCESS != listen(SocketTcpServer, 1)) {
		UART_PRINT(" Listen Error\n\r");
		close(SocketTcpServer);
		return;
	}

	int SocketTcpChild = ERROR;
	;
	while (1) {
		SocketTcpChild = accept(SocketTcpServer, (sockaddr *) &RemoteAddr, &RemoteAddrLen);

		if (SocketTcpChild == EAGAIN) {
			_SlNonOsMainLoopTask();
			continue;
		} else if (SocketTcpChild < 0) {
			UART_PRINT("Error: %d occured on accept", SocketTcpChild);
			continue;
		}

		UART_PRINT(" Connected TCP Client\r\n");
		uint8_t *pFirmware;
		pFirmware = (uint8_t *) FIRMWARE_ORIGIN;
		UART_PRINT(" Start writing Firmware at 0x%x \r\n", pFirmware);

		int bytesSend = send(SocketTcpChild, welcomeMessage, sizeof(APP_NAME) + sizeof(nBootloaderVersion), 0);
		if (bytesSend < 0) {
			close(SocketTcpChild);
			continue;
		}

		while (SocketTcpChild >= 0) {
			memset(buffer, sizeof(buffer), 0);
			int bytesReceived = recv(SocketTcpChild, buffer, sizeof(buffer), 0);
			if (bytesReceived > 0) {
				// Received some bytes
				memcpy(pFirmware, buffer, bytesReceived);
				pFirmware += bytesReceived;
				UART_PRINT("Tcp: Received %d bytes\r\n", bytesReceived);
			} else {
				switch (bytesReceived) {
				case EAGAIN: {
					_SlNonOsMainLoopTask();
				}
					continue;
				case 0: {
					// get return 0 if socket closed
					size_t length = (size_t) (pFirmware - FIRMWARE_ORIGIN);
					if (SUCCESS == SaveSRAMContentAsFirmware((uint8_t *) FIRMWARE_ORIGIN, length)) {
						close(SocketTcpChild);
						SocketTcpChild = ERROR;
						StartFirmware();
					}
				}
				default: {
					// Error occured on child socket
					close(SocketTcpChild);
					SocketTcpChild = ERROR;
				}
					break;
				}
			}
		}
	}
}

int main() {
	// Board Initialization
	BoardInit();

	// Configure the pinmux settings for the peripherals exercised
	PinMuxConfig();

	// Configuring UART
	InitTerm();

	GPIO_IF_LedConfigure(LED1 | LED2 | LED3);
	GPIO_IF_LedOff(MCU_ALL_LED_IND);

	// Display banner
	DisplayBanner(APP_NAME);

	Network_IF_InitDriver();

	// Starting the CC3200 networking layers
	Network_IF_StartSimpleLinkAsAP();

	GPIO_IF_LedOn(MCU_GREEN_LED_GPIO);

	if (ReadJumper() == 0) {

		if (ERROR == LoadAndExecuteFirmware()) {
			UART_PRINT("Firmware corrupt\r\n");
			GPIO_IF_LedOn(MCU_RED_LED_GPIO);
		}
	}
	GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);

	TcpServer();
}

