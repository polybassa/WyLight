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

#include <string>
#include "simplelink.h"
#include "SimplelinkServers.h"
#include "BroadcastTransmitter.h"
#include "trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

void WyLight::CC3200BroadcastMessage::refresh()
{
    memset(this, 0, sizeof(*this));

    // Get MAC-Address for Broadcast Message
    unsigned char macAddressLen = SL_MAC_ADDR_LEN;
    sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macAddressLen, (unsigned char*)&(this->mac));

    // Set Client Port
    this->port = htons(TcpServer::port);

    // Set Device ID
    // TODO save DeviceID global
    memset(&(this->deviceId), 0, sizeof(this->deviceId));
    const char tempDeviceId[] = "WyLightCC3200";
    memcpy(&(this->deviceId), (void*)tempDeviceId, sizeof(this->deviceId));

    // Set Version
    memcpy(&(this->version), (void*)CC3200_VERSION.data(), sizeof(this->version));
}

void BroadcastTransmitter::run(void)
{
    Task::run();
}

void BroadcastTransmitter::stop(void)
{
    Task::stop();
}

BroadcastTransmitter::BroadcastTransmitter(void) : Task((const char*)"Broadcast", OSI_STACK_SIZE, 5,
                                                        [&](const bool& stopFlag){
    const sockaddr_in destaddr(AF_INET, htons(this->port),
                               htonl(INADDR_BROADCAST));
    const int sock = socket(AF_INET, SOCK_DGRAM, 0);
    const socklen_t addrLen = sizeof(sockaddr_in);
    int status;

    WyLight::CC3200BroadcastMessage mMsg;
    mMsg.refresh();

    if (sock < 0) {
        Trace(ZONE_ERROR,
              "ERROR: Couldn't aquire socket for Broadcast transmit\r\n");
        return;
    }

    Trace(ZONE_INFO, "BC Transmitter started\r\n");
    do {
        osi_Sleep(1500);
        // Send Broadcast Message
        status =
            sendto(sock, &mMsg,
                   sizeof(WyLight::BroadcastMessage), 0,
                   (sockaddr*)&destaddr, addrLen);
    } while (status > 0 && !stopFlag);

    Trace(ZONE_INFO, "BC Transmitter stopped\r\n");
    // Close socket in case of any error's and try to open a new socket in the next loop
    close(sock);
}) {}
