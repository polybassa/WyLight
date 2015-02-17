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
#include <array>
#include "osi.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "simplelink.h"

class SimplelinkDriver {
    
    static const std::string GET_TOKEN;
    static const std::string POST_TOKEN;
    
    static const uint8_t SSID_LEN_MAX = 32;
    static const uint8_t BSSID_LEN_MAX = 6;
    static const uint8_t SEC_KEY_LEN_MAX = 64;
    static const uint8_t SL_STOP_TIMEOUT = 30;
    static const uint8_t MAX_NUM_NETWORKENTRIES = 10;
    static const uint16_t CONNECT_TIMEOUT = 20000;
    
    struct driverStatus {
        bool networkProcessorOn;
        bool connected;
        bool IPLeased;
        bool IPAcquired;
        bool connectFailed;
        bool pingDone;
        void reset(void);
    };
    
    struct statusInformation {
        unsigned long SimpleLinkStatus;
        unsigned long StationIpAddress;
        unsigned long GatewayIpAddress;
        std::array<char, SSID_LEN_MAX> ConnectionSSID;
        std::array<char, BSSID_LEN_MAX> ConnectionBSSID;
        unsigned short ConnectionTimeDelayIndex;
        void reset(void);
    };
    
    struct provisioningData {
        unsigned char priority;
        std::array<char, SSID_LEN_MAX> wlanSSID;
        uint8_t wlanSSIDLength;
        std::array<char, SEC_KEY_LEN_MAX> wlanSecurityKey;
        SlSecParams_t secParameters;
        Sl_WlanNetworkEntry_t networkEntries[MAX_NUM_NETWORKENTRIES];
        void reset(void);
    };
    
    static struct driverStatus Status;
    static struct statusInformation Info;
    static struct provisioningData ProvisioningData;
    
    static xSemaphoreHandle ProvisioningDataSemaphore;
    static xSemaphoreHandle ConnectionLostSemaphore;
    
    static void reset(void);

    static void responseNetworkEntries(const unsigned long entryNumber, SlHttpServerResponse_t *response);
    static long extractTokenNumber(SlHttpServerEvent_t const * const event);
    static char extractTokenParameter(SlHttpServerEvent_t const * const event);
    static void setSecurityKey(SlHttpServerEvent_t const * const event);
    static void setSecurityType(SlHttpServerEvent_t const * const event);
    
    static long configureAsAccesspoint(void);
    static long scanForAccesspoints(void);
    static long waitForConnectWithTimeout(const unsigned int timeout_ms);
    static long configureSimpleLinkToDefaultState(void);
    static long startAsStation(void);
    static long startAsAccesspoint(void);
    static void disconnect(void);
    static long addNewProfile(void);
    
public:
    
    SimplelinkDriver(const bool accesspointMode);
    SimplelinkDriver(const SimplelinkDriver&) = delete;
    SimplelinkDriver& operator=(const SimplelinkDriver&) = delete;
    SimplelinkDriver(SimplelinkDriver&&) = delete;
    ~SimplelinkDriver(void);
    
    void waitForNewProvisioningData(void) const;
    void waitUntilConnectionLost(void) const;
    
    operator bool() const;
    bool isConnected(void) const;
    
    static void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent);
    static void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
    static void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);
    static void SimpleLinkSockEventHandler(SlSockEvent_t *pSock);
    static void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse);
};

#endif
