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
// Simplelink includes
#include "simplelink.h"

//Common interface includes
#include "server.h"
//Application Includes
#include "broadcast.h"
#include "osi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

//
// GLOBAL VARIABLES -- Start
//

static xSemaphoreHandle g_BroadcastStoppedSemaphore;
static xSemaphoreHandle g_BroadcastStartSemaphore;
static xTaskHandle g_BroadcastTaskHandle;
static tBoolean g_StopBroadcastTask;

OsiSyncObj_t BroadcastStartSemaphore = &g_BroadcastStartSemaphore;
static OsiSyncObj_t BroadcastStoppedSemaphore= &g_BroadcastStoppedSemaphore;
OsiTaskHandle BroadcastTaskHandle = &g_BroadcastTaskHandle;

//
// GLOBAL VARIABLES -- End
//

static void BroadcastMessage_Init(struct BroadcastMessage *pMessage) {
	memset(pMessage, 0, sizeof(struct BroadcastMessage));

	// Get MAC-Address for Broadcast Message
	unsigned char macAddressLen = SL_MAC_ADDR_LEN;
	sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, (unsigned char *) &(pMessage->MAC));

	// Set Client Port
	pMessage->port = htons(SERVER_PORT);

	// Set Device ID
	memset(&(pMessage->deviceId), 0, sizeof(pMessage->deviceId));
	const char tempDeviceId[] = "WyLightCC3200";
	mem_copy(&(pMessage->deviceId), (void *) tempDeviceId, sizeof(pMessage->deviceId));

	// Set Version
	const char tempVersion[] = "wifly-EZX Ver 4.00.1, Apr 19";
	mem_copy(&(pMessage->version), (void *) tempVersion, sizeof(pMessage->version));

	pMessage->rssi = 0;
	pMessage->rtc = 0;
}

void Broadcast_TaskInit(void) {
	osi_SyncObjCreate(BroadcastStoppedSemaphore);
	osi_SyncObjCreate(BroadcastStartSemaphore);

	g_StopBroadcastTask = false;
}

void Broadcast_TaskQuit(void) {
	g_StopBroadcastTask = true;
	osi_SyncObjWait(BroadcastStoppedSemaphore, OSI_WAIT_FOREVER);
}

//*****************************************************************************
//
//! Broadcast_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the Broadcast Messages
//
//*****************************************************************************
void Broadcast_Task(void *pvParameters) {

	const sockaddr_in destaddr = { .sin_family = AF_INET, .sin_port = htons(BC_PORT_NUM), .sin_addr.s_addr = htonl(
	INADDR_BROADCAST) };
	const socklen_t addrLen = sizeof(sockaddr_in);
	int status;

	struct BroadcastMessage tempBroadcastMessage;

	while (1) {
		osi_SyncObjWait(BroadcastStartSemaphore, OSI_WAIT_FOREVER);

		BroadcastMessage_Init(&tempBroadcastMessage);

		// creating a UDP socket
		const int sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock < 0) {
			// error
			UART_PRINT("ERROR: Couldn't aquire socket for Broadcast transmit\r\n");
			osi_SyncObjSignal(BroadcastStoppedSemaphore);
			continue;
		}

		UART_PRINT("Broadcast Transmitter started \r\n");

		do {
			osi_Sleep(1500);
			// Send Broadcast Message
			status = sendto(sock, &tempBroadcastMessage, sizeof(struct BroadcastMessage), 0, (sockaddr *) &destaddr,
					addrLen);

		} while (status > 0 && !g_StopBroadcastTask);

		g_StopBroadcastTask = false;

		UART_PRINT("Broadcast Transmitter stopped \r\n");
		// Close socket in case of any error's and try to open a new socket in the next loop
		close(sock);
		osi_SyncObjSignal(BroadcastStoppedSemaphore);
	}
}

