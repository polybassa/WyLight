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
#include "hw_types.h"

// Simplelink includes
#include "simplelink.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//WyLight adaption includes
//#include "RingBuf.h"

//Application Includes
#include "server.h"

//
// GLOBAL VARIABLES -- Start
//

static xTaskHandle g_TcpServerTaskHandle;
static xSemaphoreHandle g_TcpServerStartSemaphore;
static xSemaphoreHandle g_TcpServerStoppedSemaphore;

static xTaskHandle g_UdpServerTaskHandle;
static xSemaphoreHandle g_UdpServerStartSemaphore;
static xSemaphoreHandle g_UdpServerStoppedSemaphore;

OsiSyncObj_t TcpServerStartSemaphore = &g_TcpServerStartSemaphore;
static OsiSyncObj_t TcpServerStoppedSemaphore = &g_TcpServerStoppedSemaphore;
OsiTaskHandle TcpServerTaskHandle = &g_TcpServerTaskHandle;

OsiSyncObj_t UdpServerStartSemaphore = &g_UdpServerStartSemaphore;
static OsiSyncObj_t UdpServerStoppedSemaphore = &g_UdpServerStoppedSemaphore;
OsiTaskHandle UdpServerTaskHandle = &g_UdpServerTaskHandle;

static tBoolean killTcpServer;
static tBoolean killUdpServer;

//
// GLOBAL VARIABLES -- End
//

void TcpServer_TaskInit(void) {
	osi_SyncObjCreate(TcpServerStartSemaphore);
	osi_SyncObjCreate(TcpServerStoppedSemaphore);
}

void TcpServer_TaskQuit(void) {
	killTcpServer = true;
	osi_SyncObjWait(TcpServerStoppedSemaphore, OSI_WAIT_FOREVER);
	UART_PRINT("TcpServer stopped\r\n");
}

void UdpServer_TaskInit(void) {
	osi_SyncObjCreate(UdpServerStartSemaphore);
	osi_SyncObjCreate(UdpServerStoppedSemaphore);
}

void UdpServer_TaskQuit(void) {
	killUdpServer = true;
	osi_SyncObjWait(UdpServerStoppedSemaphore, OSI_WAIT_FOREVER);
	UART_PRINT("UdpServer stopped \r\n");
}
//*****************************************************************************
//
//! TcpServer_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the TcpServer Socket
//
//*****************************************************************************

#define BUFFERSIZE 256

void TcpServer_Task(void *pvParameters) {

	sockaddr_in RemoteAddr;
	socklen_t RemoteAddrLen = sizeof(sockaddr_in);
	int SocketTcpServer, SocketTcpChild;
	uint8_t buffer[BUFFERSIZE];
	const sockaddr_in LocalAddr =
			{ .sin_family = AF_INET, .sin_port = htons(SERVER_PORT), .sin_addr.s_addr = INADDR_ANY };

	while (1) {
		osi_SyncObjWait(TcpServerStartSemaphore, OSI_WAIT_FOREVER);
		killTcpServer = false;
		UART_PRINT("TcpServer started\r\n");

		SocketTcpServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (SocketTcpServer < 0) {
			UART_PRINT(" Socket Error: %d", SocketTcpServer);
			osi_SyncObjSignal(TcpServerStoppedSemaphore);
			continue;
		}

		if (SUCCESS != bind(SocketTcpServer, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
			UART_PRINT(" Bind Error\n\r");
			close(SocketTcpServer);
			osi_SyncObjSignal(TcpServerStoppedSemaphore);
			continue;
		}

		// Backlog = 1 to accept maximal 1 connection
		if (SUCCESS != listen(SocketTcpServer, 1)) {
			UART_PRINT(" Listen Error\n\r");
			close(SocketTcpServer);
			osi_SyncObjSignal(TcpServerStoppedSemaphore);
			continue;
		}

		int nonBlocking = 1;
		setsockopt(SocketTcpServer, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

		while (SocketTcpServer > 0) {
			// non blocking accept
			SocketTcpChild = accept(SocketTcpServer, (sockaddr *) &RemoteAddr, &RemoteAddrLen);
			if (killTcpServer) {
				if (SocketTcpChild >= 0) {
					close(SocketTcpChild);
					SocketTcpChild = ERROR;
				}
				close(SocketTcpServer);
				SocketTcpServer = ERROR;
				osi_SyncObjSignal(TcpServerStoppedSemaphore);
				break;
			}
			if (SocketTcpChild == EAGAIN) {
				osi_Sleep(100);
				continue;
			}
			if (SocketTcpChild < 0) {
				UART_PRINT("accept error: %d\n\r", SocketTcpChild);
				continue;
			}

			int nonBlocking = 1;
			setsockopt(SocketTcpChild, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

			int bytesReceived = 0;
			UART_PRINT("TCP Client connected\r\n");
			while (SocketTcpChild > 0) {
				memset(buffer, sizeof(buffer), 0);

				bytesReceived = recv(SocketTcpChild, buffer, sizeof(buffer), 0);

				if (killTcpServer) {
					close(SocketTcpChild);
					SocketTcpChild = ERROR;
					break;
				}

				if (bytesReceived == EAGAIN) {
					// if we don't recveived data, we can sleep a little bit
					osi_Sleep(100);
					continue;
				}

				if (bytesReceived > 0) {
					// Received some bytes
					// TODO: Save bytes anywhere for wylight adaption
					buffer[bytesReceived] = 0;
					UART_PRINT("Tcp => Received %d bytes:%s\r\n", bytesReceived, buffer);
				} else {
					// Error or close occured on child socket
					close(SocketTcpChild);
					SocketTcpChild = ERROR;
					break;
				}
				UART_PRINT("TCP Client disconnected\r\n");
			}
		}
	}
}

//*****************************************************************************
//
//! UdpServer_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the TcpServer Socket
//
//*****************************************************************************
#if 0
void UdpServer_Task(void *pvParameters) {
	sockaddr_in RemoteAddr;
	socklen_t RemoteAddrLen = sizeof(sockaddr_in);
	int SocketUdpServer;
	unsigned char buffer[BUFFERSIZE];

	const sockaddr_in LocalAddr = { .sin_family = AF_INET, .sin_port = htons(SERVER_PORT), .sin_addr.s_addr =
	htonl(INADDR_ANY) };

	while (1) {

		SocketUdpServer = sl_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(SocketUdpServe < 0) {
			UART_PRINT("Udp Socket Error\r\n");
			osi_Sleep(100);
			continue;
		}
		int nonBlocking = 1;
		setsockopt(SocketUdpServer, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

		if (SUCCESS != bind(SocketUdpServer, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
			UART_PRINT(" Bind Error\n\r");
			close(SocketUdpServer);
			continue;
		}

		while (SocketUdpServer > 0) {
			UART_PRINT(" UDP Server started \r\n");
			memset(buffer, sizeof(buffer), 0);
			int bytesReceived = recvfrom(SocketUdpServer, buffer, sizeof(buffer) - 1, 0,
					(sockaddr *) &RemoteAddr, &RemoteAddrLen);
			if (bytesReceived > 0 && IS_CONNECTED(g_ulStatus)) {
				// Received some bytes
				// TODO: Write received Bytes in global Buffer
				UART_PRINT("Received %d bytes:%s\r\n", bytesReceived, buffer);
			}

			if (!IS_CONNECTED(g_ulStatus)) {
				sl_Close(SocketUdpServer);
				SocketUdpServer = 0;
				break;
			}
		}
	}
}
#endif
