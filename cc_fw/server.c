/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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
#include "RingBuf.h"
#include "wy_firmware.h"
//
// GLOBAL VARIABLES -- Start
//

static xTaskHandle g_TcpServerTaskHandle;
static xSemaphoreHandle g_TcpServerStartSemaphore;
static xSemaphoreHandle g_TcpServerStoppedSemaphore;

static xTaskHandle g_UdpServerTaskHandle;
static xSemaphoreHandle g_UdpServerStartSemaphore;
static xSemaphoreHandle g_UdpServerStoppedSemaphore;

OsiTaskHandle *TcpServerTaskHandle = &g_TcpServerTaskHandle;
OsiTaskHandle *UdpServerTaskHandle = &g_UdpServerTaskHandle;

static tBoolean g_KillTcpServer;
static tBoolean g_KillUdpServer;

static xSemaphoreHandle g_WriteDataSemaphore;

//
// GLOBAL VARIABLES -- End
//

void TcpServer_TaskInit(void) {
	osi_SyncObjCreate(&g_TcpServerStartSemaphore);
	osi_SyncObjCreate(&g_TcpServerStoppedSemaphore);
	osi_SyncObjCreate(&g_WriteDataSemaphore);
	osi_SyncObjSignal(&g_WriteDataSemaphore);
}

inline void TcpServer_TaskRun(void) {
	osi_SyncObjSignal(&g_TcpServerStartSemaphore);
}

void TcpServer_TaskQuit(void) {
	g_KillTcpServer = true;
	osi_SyncObjWait(&g_TcpServerStoppedSemaphore, OSI_WAIT_FOREVER);
	UART_PRINT("TcpServer stopped\r\n");
}

void UdpServer_TaskInit(void) {
	osi_SyncObjCreate(&g_UdpServerStartSemaphore);
	osi_SyncObjCreate(&g_UdpServerStoppedSemaphore);
}

inline void UdpServer_TaskRun(void) {
	osi_SyncObjSignal(&g_UdpServerStartSemaphore);
}

void UdpServer_TaskQuit(void) {
	g_KillUdpServer = true;
	osi_SyncObjWait(&g_UdpServerStoppedSemaphore, OSI_WAIT_FOREVER);
	UART_PRINT("UdpServer stopped \r\n");
}

#define BUFFERSIZE 256

static void TcpServer_Receive(const int childSock) {
	uint8_t buffer[BUFFERSIZE];

	while (!g_KillTcpServer) {

		unsigned int bytesToSend = 0;
		while(!RingBuf_IsEmpty(&g_RingBuf_Tx)){
			buffer[bytesToSend++] = RingBuf_Get(&g_RingBuf_Tx);
		}
		if (bytesToSend && (bytesToSend != send(childSock, buffer, bytesToSend, 0))) {
			UART_PRINT("Tcp => Error during transmit\r\n");
		}

		int bytesReceived = recv(childSock, buffer, sizeof(buffer), 0);

		if (EAGAIN == bytesReceived) {
			// if we don't recveived data, we can sleep a little bit
			osi_Sleep(20);
			continue;
		}

		if (bytesReceived <= 0) {
			// Error or close occured on child socket
			return;
		}

		UART_PRINT(",");
		int i = 0;
		osi_SyncObjWait(&g_WriteDataSemaphore, OSI_WAIT_FOREVER);
		for (; i < bytesReceived; i++) {
			RingBuf_Put(&g_RingBuf, buffer[i]);
		}
		osi_SyncObjSignal(NewDataAvailableSemaphore);
		osi_SyncObjSignal(&g_WriteDataSemaphore);
	}
}

static void TcpServer_Run(const int serverSock) {
	sockaddr_in childAddr;
	socklen_t childAddrLen = sizeof(sockaddr_in);

	for (;;) {
		// non blocking accept
		int childSock = accept(serverSock, (sockaddr *) &childAddr, &childAddrLen);

		if (g_KillTcpServer) {
			if (childSock >= 0) {
				close(childSock);
			}
			return;
		}
		if (EAGAIN == childSock) {
			osi_Sleep(100);
			continue;
		}
		if (childSock < 0) {
			UART_PRINT("Accept error: %d\n\r", childSock);
			continue;
		}

		int nonBlocking = 1;
		setsockopt(childSock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

		UART_PRINT("TCP Client connected\r\n");
		TcpServer_Receive(childSock);
		close(childSock);
		childSock = ERROR;
		UART_PRINT("TCP Client disconnected\r\n");
	}
}

void TcpServer_Task(void *pvParameters) {
	const sockaddr_in LocalAddr =
			{ .sin_family = AF_INET, .sin_port = htons(SERVER_PORT), .sin_addr.s_addr = INADDR_ANY };

	for (;;) {
		osi_SyncObjWait(&g_TcpServerStartSemaphore, OSI_WAIT_FOREVER);
		g_KillTcpServer = false;

		int serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (serverSock < 0) {
			UART_PRINT(" Socket Error: %d", serverSock);
			osi_SyncObjSignal(&g_TcpServerStoppedSemaphore);
			continue;
		}

		if (SUCCESS != bind(serverSock, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
			UART_PRINT(" Bind Error\n\r");
			close(serverSock);
			osi_SyncObjSignal(&g_TcpServerStoppedSemaphore);
			continue;
		}

		// Backlog = 1 to accept maximal 1 connection
		if (SUCCESS != listen(serverSock, 1)) {
			UART_PRINT(" Listen Error\n\r");
			close(serverSock);
			osi_SyncObjSignal(&g_TcpServerStoppedSemaphore);
			continue;
		}

		int nonBlocking = 1;
		setsockopt(serverSock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
		UART_PRINT("TcpServer started\r\n");
		TcpServer_Run(serverSock);
		close(serverSock);
		osi_SyncObjSignal(&g_TcpServerStoppedSemaphore);
	}
}

static void UdpServer_Receive(const int serverSock) {
	socklen_t RemoteAddrLen = sizeof(sockaddr_in);
	uint8_t buffer[BUFFERSIZE];
	sockaddr_in remoteAddr;

	while (!g_KillUdpServer) {
		const int bytesReceived = recvfrom(serverSock, buffer, sizeof(buffer), 0, (sockaddr *) &remoteAddr,
				&RemoteAddrLen);

		if (EAGAIN == bytesReceived) {
			osi_Sleep(15);
			continue;
		}

		if (bytesReceived <= 0) {
			return;
		}

		UART_PRINT(".");
		osi_SyncObjWait(&g_WriteDataSemaphore, OSI_WAIT_FOREVER);
		int i = 0;
		for (i = 0; i < bytesReceived; i++) {
			RingBuf_Put(&g_RingBuf, buffer[i]);
		}
		osi_SyncObjSignal(NewDataAvailableSemaphore);
		osi_SyncObjSignal(&g_WriteDataSemaphore);
	}
}

void UdpServer_Task(void *pvParameters) {
	const sockaddr_in LocalAddr = { .sin_family = AF_INET, .sin_port = htons(SERVER_PORT), .sin_addr.s_addr =
	htonl(INADDR_ANY) };

	for (;;) {
		osi_SyncObjWait(&g_UdpServerStartSemaphore, OSI_WAIT_FOREVER);
		g_KillUdpServer = false;
		int serverSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (serverSock < 0) {
			UART_PRINT("Udp Socket Error\r\n");
			osi_Sleep(100);
			osi_SyncObjSignal(&g_UdpServerStoppedSemaphore);
			continue;
		}
		int nonBlocking = 1;
		setsockopt(serverSock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));

		if (SUCCESS != bind(serverSock, (sockaddr *) &LocalAddr, sizeof(LocalAddr))) {
			UART_PRINT(" Bind Error\n\r");
			close(serverSock);
			osi_SyncObjSignal(&g_UdpServerStoppedSemaphore);
			continue;
		}

		UART_PRINT("UDP Server started \r\n");
		UdpServer_Receive(serverSock);
		close(serverSock);
		UART_PRINT("UDP Server stopped \r\n");
		osi_SyncObjSignal(&g_UdpServerStoppedSemaphore);
	}
}
