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
#include "simplelink.h"
#include "server.h"
#include "BroadcastTransmitter.h"
#include "osi.h"

void CC3200BroadcastMessage::refresh() {
	memset(this, 0, sizeof(*this));

	// Get MAC-Address for Broadcast Message
	unsigned char macAddressLen = SL_MAC_ADDR_LEN;
	sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, (unsigned char *) &(this->mac));

	// Set Client Port
	this->port = htons(SERVER_PORT);

	// Set Device ID
	memset(&(this->deviceId), 0, sizeof(this->deviceId));
	const char tempDeviceId[] = "WyLightCC3200";
	memcpy(&(this->deviceId), (void *) tempDeviceId, sizeof(this->deviceId));

	// Set Version
	const char tempVersion[] = "wifly-EZX Ver 4.00.1, Apr 19";
	memcpy(&(this->version), (void *) tempVersion, sizeof(this->version));

	this->rssi = 0;
	this->rtc = 0;
}

void BroadcastTransmitter::run(void) {
	Task::run();
}

void BroadcastTransmitter::stop(void) {
	Task::stop();
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

BroadcastTransmitter::BroadcastTransmitter(void) : Task((const char *)"Broadcast", OSI_STACK_SIZE, 5, [&](const tBoolean& stopFlag){
	
	const sockaddr_in destaddr(AF_INET, htons(BC_PORT_NUM), htonl(INADDR_BROADCAST));
	const int sock = socket(AF_INET, SOCK_DGRAM, 0);
	const socklen_t addrLen = sizeof(sockaddr_in);
	int status;
	
	this->mMsg.refresh();
	
	if (sock < 0) {
		UART_PRINT("ERROR: Couldn't aquire socket for Broadcast transmit\r\n");
		return;
	}
	
	UART_PRINT("Broadcast Transmitter started \r\n");
	do {
		osi_Sleep(1500);
		// Send Broadcast Message
		status = sendto(sock, &this->mMsg, sizeof(WyLight::BroadcastMessage), 0,
						(sockaddr *) &destaddr, addrLen);
		
	} while (status > 0 && !stopFlag);
	
	UART_PRINT("Broadcast Transmitter stopped \r\n");
	// Close socket in case of any error's and try to open a new socket in the next loop
	close(sock);
	
}) {}
