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
#ifndef __NETWORK_DRIVER__H__
#define __NETWORK_DRIVER__H__

#include <stdint.h>
#include <string>

class NetworkDriver {
    
    static std::string GET_TOKEN;

    struct driverStatus {
        bool networkProcessorOn;
        bool connected;
        bool IPLeased;
        bool IPAcquired;
        bool connectFailed;
        bool pingDone;
    };
    
    struct statusInformation {
        unsigned long SimpleLinkStatus;
        unsigned long StationIpAddress;
        unsigned long GatewayIpAddress;
        std::string ConnectionSSID;
        std::string ConnectionBSSID;
        unsigned short ConnectionTimeDelayIndex;
    };
    
    struct provisioningData {
        unsigned char priority;
        char wlanSSID[SSID_LEN_MAX];
        char wlanSecurityKey[SEC_KEY_LEN_MAX];
        SlSecParams_t secParameters;
        Sl_WlanNetworkEntry_t networkEntries[MAX_NUM_NETWORKENTRIES];
    };
    
    struct driverStatus mStatus;
    struct statusInformation mInfo;
    struct provisioningData mProvisioningData;
    
public:
    static const uint8_t SSID_LEN_MAX;
    static const uint8_t BSSID_LEN_MAX;
    static const uint8_t SEC_KEY_LEN_MAX;
    static const uint8_t SL_STOP_TIMEOUT;
    static const uint8_t MAX_NUM_NETWORKENTRIES;
    static const uint16_t CONNECT_TIMEOUT_MS;
    
    NetworkDriver(const bool accesspointMode);
    NetworkDriver(const NetworkDriver&) = delete;
    NetworkDriver& operator=(const NetworkDriver&) = delete;
    NetworkDriver(NetworkDriver&&) = delete;
    
    void waitForNewProvisioningData(void);
    
    void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent);
    void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
    void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);
    void SimpleLinkSockEventHandler(SlSockEvent_t *pSock);
    void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse);
};

#endif
