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

//common
#include "uart_if.h"

//WyLight
#include "bootloader.h"
#include "firmware_loader.h"
#include "tcp_server.h"

#ifndef SIMULATOR
#include "simplelink.h"
#include "wy_bl_network_if.h"
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define SUCCESS 0
#define UART_PRINT Report
#endif /*SIMULATOR */

#define BUFFERSIZE 1024
#define SERVER_PORT htons(2000)

#ifdef SIMULATOR

static uint8_t memory[0x3FFFF];

#undef FIRMWARE_ORIGIN
#define FIRMWARE_ORIGIN ((uint8_t*)&memory[0])

#endif

/**
 * @return 0 on success, if new firmware was saved and validated
 */
static int ReceiveFw(int SocketTcpChild)
{
	uint8_t *pFirmware = (uint8_t *) FIRMWARE_ORIGIN;
	UART_PRINT("Start writing Firmware at 0x%x \r\n", pFirmware);

	for(;;) {
		int bytesReceived = recv(SocketTcpChild, pFirmware, BUFFERSIZE, 0);

		if (bytesReceived > 0) {
			// Received some bytes
			pFirmware += bytesReceived;
			UART_PRINT("Tcp: Received %d bytes\r\n", bytesReceived);
#ifdef SIMULATOR
			if (bytesReceived < BUFFERSIZE) {
    			const size_t length = (size_t) (pFirmware - FIRMWARE_ORIGIN);
				return SaveSRAMContentAsFirmware((uint8_t *) FIRMWARE_ORIGIN, length);
			}
#endif
			continue;
		}

		if (EAGAIN == bytesReceived) {
			_SlNonOsMainLoopTask();
			continue;
		}

		if (bytesReceived < 0) {
			return bytesReceived;
		}

		const size_t length = (size_t) (pFirmware - FIRMWARE_ORIGIN);
		return SaveSRAMContentAsFirmware((uint8_t *) FIRMWARE_ORIGIN, length);
	}
}

/**
 * Create a new tcp server socket and start listening on it
 * @return positiv socket descriptor, or negative value on error
 */
static int TcpServer_Listen()
{
	static const struct sockaddr_in localAddr = {
		.sin_family = AF_INET,
		.sin_port = SERVER_PORT,
		.sin_addr = {
			.s_addr = 0
		}
	};

	int status;
	
	const int listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket >= 0) {
#ifndef SIMULATOR
		const int dontBlock = 1;
		status = setsockopt(listenSocket, SOL_SOCKET, SO_NONBLOCKING, &dontBlock, sizeof(dontBlock));	
		if (status) {
			UART_PRINT("Setsockopt ERROR \r\n");
			goto on_error_close;
		}
#endif
		status = bind(listenSocket, (struct sockaddr *) &localAddr, sizeof(localAddr));

		if (status) {
			UART_PRINT("Bind Error\n\r");
			goto on_error_close;
		}

		const int maxConnections = 1;
		status = listen(listenSocket, maxConnections);
		if (status) {
			UART_PRINT("Listen Error\n\r");
			goto on_error_close;
		}
	}
	return listenSocket;

on_error_close:
	close(listenSocket);
	return status;
}

/**
 * @return if new firmware was received and validated
 */
extern void TcpServer(void)
{
	char welcome[] = "\0\0\0\0WyLightBootloader";
	uint32_t nBootloaderVersion = htonl(BOOTLOADER_VERSION);

	memcpy(welcome, &nBootloaderVersion, sizeof(uint32_t));

	struct sockaddr_in RemoteAddr;
	socklen_t RemoteAddrLen = sizeof(struct sockaddr_in);

	const int listenSocket = TcpServer_Listen();

	while (listenSocket >= 0) {
		const int SocketTcpChild = accept(listenSocket, (struct sockaddr *) &RemoteAddr, &RemoteAddrLen);

		if (SocketTcpChild == EAGAIN) {
			_SlNonOsMainLoopTask();
			continue;
		} else if (SocketTcpChild < 0) {
			UART_PRINT("Error: %d occured on accept\r\n", SocketTcpChild);
			continue;
		}

		UART_PRINT("Connected TCP Client\r\n");

		if (sizeof(welcome) !=  send(SocketTcpChild, welcome, sizeof(welcome), 0)) {
			close(SocketTcpChild);
			continue;
		}

		const int fwStatus = ReceiveFw(SocketTcpChild);
		close(SocketTcpChild);
		if (0 == fwStatus)
			return;
	}
	UART_PRINT("Socket Error: %d \r\n", listenSocket);
}
