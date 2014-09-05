/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

 This file is part of WyLight.

 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICUL<AR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "string.h"

#include "simplelink.h"

//common
#include "uart_if.h"

//WyLight
#include "bootloader.h"
#include "firmware_loader.h"
#include "wy_bl_network_if.h" //TODO UART_PRINT

#define BUFFERSIZE 1024

static const unsigned short SERVER_PORT = 2000;

extern void TcpServer(void)
{
	static const char APP_NAME[] = "WyLight Bootloader";
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
				UART_PRINT("Tcp: Received 0x%x\r\n", bytesReceived);
				switch (bytesReceived) {
				case EAGAIN: {
					_SlNonOsMainLoopTask();
				}
				
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
