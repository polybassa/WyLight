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

// Simplelink includes
#include "simplelink.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"

//Common interface includes
#include "wy_network_if.h"
#include "server.h"
//Application Includes
#include "broadcast.h"

//
// GLOBAL VARIABLES -- Start
//

struct __attribute__((__packed__)) BroadcastMessage {
	uint8_t MAC[6];
	uint8_t channel;
	uint8_t rssi;
	uint16_t port;
	uint32_t rtc;
	uint16_t battery;
	uint16_t gpio;
	uint8_t asciiTime[14];
	uint8_t version[28];
	uint8_t deviceId[32];
	uint16_t boottime;
	uint8_t sensors[16];

};

static struct BroadcastMessage g_BroadcastMessage;
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

	while (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
		osi_Sleep(500);
	}
	BroadcastMessage_Init(&g_BroadcastMessage);

	while (1) {

		while (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
			osi_Sleep(500);
		}

		const sockaddr_in sAddr = { .sin_family = AF_INET, .sin_port = htons(BC_PORT_NUM), .sin_addr.s_addr = htonl(
				IP_ADDR) };
		const socklen_t addrLen = sizeof(sockaddr_in);
		int sock;
		int status;

		// creating a UDP socket
		sock = socket(AF_INET, SOCK_DGRAM, 0);
		if (sock < 0) {
			// error
			UART_PRINT("ERROR: Couldn't aquire socket for Broadcast transmit\r\n");
			osi_Sleep(5000);
			continue;
		}
		UART_PRINT("Broadcast Transmitter started \r\n");

		do {
			// Send Broadcast Message
			status = sendto(sock, &g_BroadcastMessage, sizeof(struct BroadcastMessage), 0, (sockaddr *) &sAddr,
					addrLen);

			if (status <= 0) {
				UART_PRINT("ERROR: Failure during Broadcast transmit\r\n");
				break;
			}
			osi_Sleep(1500);
		} while (IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus) && status > 0 && sock >= 0);

		UART_PRINT("Broadcast Transmitter stopped \r\n");
		while (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
			osi_Sleep(500);
		}
		// Close socket in case of any error's and try to open a new socket in the next loop
		close(sock);
	}
}

