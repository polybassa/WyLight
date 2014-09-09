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

//Application Includes
#include "sniffer.h"

//
// GLOBAL VARIABLES -- Start
//

static xTaskHandle g_SnifferTaskHandle;
static xSemaphoreHandle g_SnifferStartSemaphore;
static xSemaphoreHandle g_SnifferStoppedSemaphore;

OsiTaskHandle SnifferTaskHandle = &g_SnifferTaskHandle;

static tBoolean g_KillSniffer;

//
// GLOBAL VARIABLES -- End
//

void Sniffer_TaskInit(void) {
	osi_SyncObjCreate(&g_SnifferStartSemaphore);
	osi_SyncObjCreate(&g_SnifferStoppedSemaphore);
}

inline void Sniffer_TaskRun(void) {
	osi_SyncObjSignal(&g_SnifferStartSemaphore);
}

void Sniffer_TaskQuit(void) {
	g_KillSniffer = true;
	osi_SyncObjWait(&g_SnifferStoppedSemaphore, OSI_WAIT_FOREVER);
	UART_PRINT("Sniffer stopped\r\n");
}

#define BUFFERSIZE 256
#define CHANNEL 0

static void Sniffer_Receive(const int sock) {
	uint8_t buffer[BUFFERSIZE];

	while (!g_KillSniffer) {
		int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);

		if (EAGAIN == bytesReceived) {
			// if we don't recveived data, we can sleep a little bit
			osi_Sleep(10);
			continue;
		}

		if (bytesReceived <= 0) {
			// Error or close occured on child socket
			return;
		}

		buffer[bytesReceived] = 0;
		UART_PRINT("Sniffer => Received %d bytes:%s\r\n", bytesReceived, buffer);
	}
}

void Sniffer_Task(void *pvParameters) {
	for (;;) {
		osi_SyncObjWait(&g_SnifferStartSemaphore, OSI_WAIT_FOREVER);
		g_KillSniffer = false;

		int sock = socket(AF_RF, SOCK_RAW, CHANNEL);
		if (sock < 0) {
			UART_PRINT("Socket Error: %d", sock);
			osi_SyncObjSignal(&g_SnifferStoppedSemaphore);
			continue;
		}

		int nonBlocking = 1;
		setsockopt(sock, SOL_SOCKET, SO_NONBLOCKING, &nonBlocking, sizeof(nonBlocking));
		UART_PRINT("Sniffer started\r\n");
		Sniffer_Receive(sock);
		close(sock);
		osi_SyncObjSignal(&g_SnifferStoppedSemaphore);
	}
}
