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

// Simplelink includes 
#include "simplelink.h"
#include "wlan.h"

// driverlib includes 
#include "hw_types.h"
#include "timer.h"
#include "rom.h"
#include "rom_map.h"

// common interface includes 
#include "NetworkDriver.h"
#include "timer_if.h"
#include "gpio_if.h"
#include "wifi.h"

// oslib include
#include "osi.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "firmware/trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

#define ASSERT_ON_ERROR(line_number, error_code) \
{ \
    if (error_code < 0){ \
        Trace(ZONE_ERROR, "%d: Assert Error: %d\r\n", line_number, error_code); \
        return error_code; \
    } \
}

#define SUCCESS 0

const std::string NetworkDriver::GET_TOKEN = "__SL_G_US";
const std::string NetworkDriver::POST_TOKEN = "__SL_P_US";
NetworkDriver* NetworkDriver::g_Instance = nullptr;

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
    if (NetworkDriver::g_Instance) {
        NetworkDriver::g_Instance->SimpleLinkWlanEventHandler(pSlWlanEvent);
    }
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
    if (NetworkDriver::g_Instance) {
        NetworkDriver::g_Instance->SimpleLinkNetAppEventHandler(pNetAppEvent);
    }
}

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
    if (NetworkDriver::g_Instance) {
        NetworkDriver::g_Instance->SimpleLinkGeneralEventHandler(pDevEvent);
    }
}

void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {
    if (NetworkDriver::g_Instance) {
        NetworkDriver::g_Instance->SimpleLinkSockEventHandler(pSock);
    }
}

void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse) {
    if (NetworkDriver::g_Instance) {
        NetworkDriver::g_Instance->SimpleLinkHttpServerCallback(pSlHttpServerEvent, pSlHttpServerResponse);
    }
}

void NetworkDriver::responseNetworkEntries(const unsigned long entryNumber, SlHttpServerResponse_t *response) const {
    memcpy(response->ResponseData.token_value.data, this->mProvisioningData.networkEntries[entryNumber].ssid, this->mProvisioningData.networkEntries[entryNumber].ssid_len);
    response->ResponseData.token_value.len = this->mProvisioningData.networkEntries[entryNumber].ssid_len;
}

long NetworkDriver::extractTokenNumber(SlHttpServerEvent_t const * const event) const {
    std::string serverGetToken((const char*)event->EventData.httpTokenName.data, (size_t)event->EventData.httpTokenName.len);
    if (serverGetToken.find(GET_TOKEN) == std::string::npos) return -1;
    return (*(--serverGetToken.end())) - '0';
}

char NetworkDriver::extractTokenParameter(SlHttpServerEvent_t const * const event) const {
    std::string serverPostToken((const char*)event->EventData.httpPostData.token_name.data, event->EventData.httpPostData.token_name.len);
    if (serverPostToken.find(POST_TOKEN) == std::string::npos) return 0;
    return *(--serverPostToken.end());
}

void NetworkDriver::setSecurityKey(SlHttpServerEvent_t const * const event) {
    this->mProvisioningData.wlanSecurityKey = std::string((const char*)event->EventData.httpPostData.token_value.data,
                                                          event->EventData.httpPostData.token_value.len);
    this->mProvisioningData.secParameters.Key = (char *)this->mProvisioningData.wlanSecurityKey.data();
    this->mProvisioningData.secParameters.KeyLen = this->mProvisioningData.wlanSecurityKey.length();
}

void NetworkDriver::setSecurityType(SlHttpServerEvent_t const * const event) {
    switch (event->EventData.httpPostData.token_value.data[0]) {
        case '1':
            this->mProvisioningData.secParameters.Type = SL_SEC_TYPE_WEP;
            break;
        case '2':
            this->mProvisioningData.secParameters.Type = SL_SEC_TYPE_WPA;
            break;
        case '3':
            this->mProvisioningData.secParameters.Type = SL_SEC_TYPE_WPS_PBC;
            break;
        case '4':
            this->mProvisioningData.secParameters.Type = SL_SEC_TYPE_WPS_PIN;
            break;
        default:
            this->mProvisioningData.secParameters.Type = SL_SEC_TYPE_OPEN;
            break;
    }
}

void NetworkDriver::SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
	switch (((SlWlanEvent_t*) pSlWlanEvent)->Event) {
	case SL_WLAN_CONNECT_EVENT:
        this->mStatus.connected = true;
        this->mStatus.connectFailed = false;
        this->mInfo.ConnectionSSID = std::string((const char*)pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
		this->mInfo.ConnectionBSSID = std::string((const char*)pSlWlanEvent->EventData.STAandP2PModeWlanConnected.bssid, SL_BSSID_LENGTH);
		Trace(ZONE_INFO,"[WLAN EVENT] STA Connected to the AP: %s\n\r", this->mInfo.ConnectionSSID.data());
		break;

	case SL_WLAN_DISCONNECT_EVENT:
        this->mStatus.connected = false;
        this->mStatus.IPAcquired = false;
        Trace(ZONE_INFO,"[WLAN EVENT] Device disconnected from AP: %s\n\r", this->mInfo.ConnectionSSID.data());
        this->mInfo.ConnectionSSID.clear();
        this->mInfo.ConnectionBSSID.clear();
		break;

    case SL_WLAN_STA_CONNECTED_EVENT: {
		// when device is in AP mode and any client connects to device cc3xxx
        this->mStatus.connected = true;
        this->mStatus.connectFailed = false;
		slPeerInfoAsyncResponse_t *pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
		Trace(ZONE_INFO,"[WLAN EVENT] Client connected: %x:%x:%x:%x:%x:%x\r\n", pEventData->mac[0], pEventData->mac[1],
				pEventData->mac[2], pEventData->mac[3], pEventData->mac[4], pEventData->mac[5]);
        }
        break;

    case SL_WLAN_STA_DISCONNECTED_EVENT: {
		// when client disconnects from device (AP)
        this->mStatus.connected = false;
        this->mStatus.IPAcquired = false;
        this->mStatus.IPLeased = false;
		slPeerInfoAsyncResponse_t *pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
		Trace(ZONE_INFO,"[WLAN EVENT] Client disconnected: %x:%x:%x:%x:%x:%x\r\n", pEventData->mac[0], pEventData->mac[1],
				pEventData->mac[2], pEventData->mac[3], pEventData->mac[4], pEventData->mac[5]);
    }
		break;

	case SL_WLAN_CONNECTION_FAILED_EVENT:
		// If device gets any connection failed event
        this->mStatus.connected = false;
        this->mStatus.IPAcquired = false;
        this->mStatus.connectFailed = true;
		Trace(ZONE_INFO,"[WLAN EVENT] Connection failed\r\n");
		break;

	default:
		Trace(ZONE_INFO,"[WLAN EVENT] Unexpected event \n\r");
		break;
	}

}

void NetworkDriver::SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
	switch (pNetAppEvent->Event) {
	case SL_NETAPP_IPV4_ACQUIRED:
    case SL_NETAPP_IPV6_ACQUIRED:
        this->mStatus.IPAcquired = true;
        Trace(ZONE_INFO,"[NETAPP EVENT] IP Acquired\r\n");
		break;

    case SL_NETAPP_IP_LEASED: {
        this->mStatus.IPLeased = true;
		SlIpLeasedAsync_t *pEventData = &pNetAppEvent->EventData.ipLeased;
		this->mInfo.StationIpAddress = pEventData->ip_address;
		Trace(ZONE_INFO,"[NETAPP EVENT] IP Leased\r\n");
    }
        break;

    case SL_NETAPP_IP_RELEASED: {
        this->mStatus.IPLeased = false;
		this->mInfo.StationIpAddress = 0;
		Trace(ZONE_INFO,"[NETAPP EVENT] IP Released\r\n");
    }
        break;

    case SL_NETAPP_SOCKET_TX_FAILED: {
		Trace(ZONE_INFO,"[NETAPP EVENT] Socket Error # %d \n\r", pNetAppEvent->EventData.sd);
    }
        break;

	default:
		Trace(ZONE_INFO,"[NETAPP EVENT] Unexpected event \n\r");
		break;
	}
}

void NetworkDriver::SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) const {
	Trace(ZONE_INFO,"[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", pDevEvent->EventData.deviceEvent.status, pDevEvent->EventData.deviceEvent.sender);
}

void NetworkDriver::SimpleLinkSockEventHandler(SlSockEvent_t *pSock) const {
    if (pSock->Event == SL_NETAPP_SOCKET_TX_FAILED) {
        if (pSock->EventData.status == SL_ECLOSE)
			Trace(ZONE_ERROR,"[SOCK ERROR] - close socket (%d) operation failed to transmit all queued packets\n\n", pSock->EventData.sd);
		else
			Trace(ZONE_ERROR,"[SOCK ERROR] - TX FAILED  :  socket (%d) , reason (%d)\n\n", pSock->EventData.sd, pSock->EventData.status);
    }
    else
		Trace(ZONE_INFO,"[SOCK EVENT] - Unexpected Event [%x0x]\n\n", pSock->Event);
}

void NetworkDriver::SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse) {
	if (pSlHttpServerEvent->Event == SL_NETAPP_HTTPGETTOKENVALUE) {
        const unsigned long getTokenNumber = this->extractTokenNumber(pSlHttpServerEvent);
        if (getTokenNumber < 0 || getTokenNumber > MAX_NUM_NETWORKENTRIES)
            return;
        this->responseNetworkEntries(getTokenNumber, pSlHttpServerResponse);
	}
	else if (pSlHttpServerEvent->Event ==  SL_NETAPP_HTTPPOSTTOKENVALUE) {

		Trace(ZONE_VERBOSE," token_name: %s ", pSlHttpServerEvent->EventData.httpPostData.token_name.data);
		Trace(ZONE_VERBOSE," token_data: %s \r\n", pSlHttpServerEvent->EventData.httpPostData.token_value.data);
        
        const char postTokenParameter = this->extractTokenParameter(pSlHttpServerEvent);
        
        switch (postTokenParameter) {
            case 'C':
                if (std::string((const char*)pSlHttpServerEvent->EventData.httpPostData.token_value.data, pSlHttpServerEvent->EventData.httpPostData.token_value.len) == "Add")
                        osi_SyncObjSignalFromISR(&this->mProvisioningDataSemaphore);
                break;
            case 'D':
                this->mProvisioningData.wlanSSID = std::string((const char*)pSlHttpServerEvent->EventData.httpPostData.token_value.data, pSlHttpServerEvent->EventData.httpPostData.token_value.len);
                break;
            case 'E':
                this->setSecurityType(pSlHttpServerEvent);
                break;
            case 'F':
                this->setSecurityKey(pSlHttpServerEvent);
                break;
            case 'G':
                this->mProvisioningData.priority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - '0';
                break;
                
            default:
                break;
		}
	}
}

long NetworkDriver::waitForConnectWithTimeout(const unsigned int timeout_ms) const {
    static const unsigned int INTERVAL = 5;
    unsigned int timeoutCounter = 0;

    while ((timeoutCounter < timeout_ms) && !this->mStatus.IPAcquired) {
        osi_Sleep(INTERVAL);
        timeoutCounter += INTERVAL;
    }
    
    if (!this->mStatus.connected || !this->mStatus.IPAcquired) {
        Trace(ZONE_ERROR,"Connecting failed \r\n");
        return ERROR;
    } else
        return SUCCESS;
}

void NetworkDriver::driverStatus::reset(void) {
    memset(this, 0, sizeof(struct NetworkDriver::driverStatus));
}

void NetworkDriver::statusInformation::reset(void) {
    memset(this, 0, sizeof(struct NetworkDriver::statusInformation));
}

void NetworkDriver::provisioningData::reset(void) {
    memset(this, 0, sizeof(struct NetworkDriver::provisioningData));
}

long NetworkDriver::configureSimpleLinkToDefaultState(void) {
    
    this->mStatus.reset();
    this->mInfo.reset();
    this->mProvisioningData.reset();

	int Mode = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, Mode);

	// If the device is not in station-mode, try putting it in staion-mode
	if (ROLE_STA != Mode) {
		if (ROLE_AP == Mode) {
			// If the device is in AP mode, we need to wait for this event before doing anything
            while (!this->mStatus.IPAcquired) {
				osi_Sleep(10);
			}
		}

		// Switch to STA role and restart
		long retRes = sl_WlanSetMode(ROLE_STA);
		ASSERT_ON_ERROR(__LINE__, retRes);

		retRes = sl_Stop(SL_STOP_TIMEOUT);
		ASSERT_ON_ERROR(__LINE__, retRes);

        this->mStatus.reset();
        
		retRes = sl_Start(NULL, NULL, NULL);
		ASSERT_ON_ERROR(__LINE__, retRes);

		// Check if the device is in station again
		if (ROLE_STA != retRes) {
			// We don't want to proceed if the device is not up in STA-mode
			return ERROR;
		}
	}

    // Device in station-mode. Disconnect previous connection if any
    // The function returns 0 if 'Disconnected done', negative number if already
    // disconnected Wait for 'disconnection' event if 0 is returned, Ignore
    // other return-codes
    this->disconnect();

    // Set connection policy to Auto + Fast Connection
    //      (Device's default connection policy)
	long retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 1, 0, 0, 0), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, retRes);

    // Enable DHCP client
	unsigned char value = 1;
	retRes = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, sizeof(value), &value);
	ASSERT_ON_ERROR(__LINE__, retRes);

    // Disable scan
	unsigned char configOpt = SL_SCAN_POLICY(0);
	retRes = sl_WlanPolicySet(SL_POLICY_SCAN, configOpt, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, retRes);

    // Set Tx power level for station mode
    // Number between 0-15, as dB offset from max power - 0 will set max power
	unsigned char power = 0;
	retRes = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
	WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, sizeof(power), (unsigned char *) &power);
	ASSERT_ON_ERROR(__LINE__, retRes);

    // Set PM policy to normal
	retRes = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// Set URN-Name
	unsigned char urnName[] = "WyLight";
	retRes = sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DEVICE_URN, sizeof(urnName),
			urnName);
	ASSERT_ON_ERROR(__LINE__, retRes);

	retRes = sl_Stop(SL_STOP_TIMEOUT);
	ASSERT_ON_ERROR(__LINE__, retRes);

    this->mStatus.reset();
    this->mInfo.reset();
    this->mProvisioningData.reset();
    
	return retRes;
}

long NetworkDriver::startAsStation(void) {
	long retRes = this->configureSimpleLinkToDefaultState();
	ASSERT_ON_ERROR(__LINE__, retRes);

	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Started SimpleLink Device in STA Mode\n\r");

    return this->waitForConnectWithTimeout(CONNECT_TIMEOUT);
}

long NetworkDriver::startAsAccesspoint(void) {
	long retRes = ERROR;

    retRes = this->configureSimpleLinkToDefaultState();
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Device is configured in default state \n\r");

	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Device started \n\r");

	sl_WlanDisconnect();

	// Set connection policy to zero, that no scan in background is performed
	//      (Device's default connection policy)
	retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// Restart Simplelink
	sl_Stop(SL_STOP_TIMEOUT);

    this->mStatus.reset();

	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	unsigned long intervalInSeconds = 4;
	unsigned char parameterLen = sizeof(intervalInSeconds);
	//Scan AP in STA mode
	retRes = sl_WlanPolicySet(SL_POLICY_SCAN, SL_SCAN_POLICY_EN(1), (unsigned char *) &intervalInSeconds, parameterLen);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Scanning for SSID's\r\n----------------------------------------------\r\n");
	// wait for scan to complete
	osi_Sleep(8000);

	//Get Scan Result
	retRes = sl_WlanGetNetworkList(0, MAX_NUM_NETWORKENTRIES, &this->mProvisioningData.networkEntries[0]);
	ASSERT_ON_ERROR(__LINE__, retRes);

	int i;
	for (i = 0; i < retRes; i++) {
		Trace(ZONE_VERBOSE,"%d) SSID %s\n\r", i, this->mProvisioningData.networkEntries[i].ssid);
	}
	Trace(ZONE_VERBOSE,"----------------------------------------------\r\n");
	//Switch to AP Mode
	sl_WlanSetMode(ROLE_AP);

	//Disable ROM WebPages
	unsigned char disable = 0;
	retRes = sl_NetAppSet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_ROM_PAGES_ACCESS, 1, &disable);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// set domain name
	unsigned char domainName[] = "wylight.config";
	retRes = sl_NetAppSet(SL_NET_APP_DEVICE_CONFIG_ID, NETAPP_SET_GET_DEV_CONF_OPT_DOMAIN_NAME, sizeof(domainName),
			domainName);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// set Accesspoint SSID
	unsigned char ssid[] = "WyLightAP";
	retRes = sl_WlanSet(SL_WLAN_CFG_AP_ID, 0, sizeof(ssid), ssid);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// get current Time
	SlDateTime_t dateTime;
	unsigned char option = SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME, length = sizeof(SlDateTime_t);
	retRes = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &option, &length, (unsigned char *) &dateTime);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// compute random channel from current time
	unsigned char channel = (dateTime.sl_tm_sec % 13) + 1; // to avoid channel 0
	Trace(ZONE_VERBOSE,"Accesspoint channel: %d\r\n", channel);

	retRes = sl_WlanSet(SL_WLAN_CFG_AP_ID, 3, 1, &channel);
	ASSERT_ON_ERROR(__LINE__, retRes);

	sl_Stop(SL_STOP_TIMEOUT);
    this->mStatus.reset();
    
	//Initialize the SLHost Driver
	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Start AP\r\n");
	//Wait for Ip Acquired Event in AP Mode
	while (!this->mStatus.IPAcquired) {
		osi_Sleep(10);
	}

	return SUCCESS;
}

void NetworkDriver::disconnect(void) const {
    Trace(ZONE_VERBOSE, "Disconnect...\n\r");
	if (sl_WlanDisconnect() == 0) {
		while (this->mStatus.connected)
			osi_Sleep(10);
	}
}

NetworkDriver::~NetworkDriver(void) {
    this->disconnect();
	sl_Stop(SL_STOP_TIMEOUT);
    NetworkDriver::g_Instance = nullptr;
}

NetworkDriver::NetworkDriver(const bool accesspointMode) {
    osi_SyncObjCreate(&this->mProvisioningDataSemaphore);
    
    if (accesspointMode && (SUCCESS == this->startAsAccesspoint())) {
        NetworkDriver::g_Instance = this;
    } else {
        unsigned int RETRY = 2;
        long retVal = ERROR;
        while (RETRY-- && (retVal == ERROR)) {
            retVal = this->startAsStation();
        }
        
        if (retVal == ERROR) {
            this->disconnect();
            sl_Stop(SL_STOP_TIMEOUT);
        } else {
            NetworkDriver::g_Instance = this;
        }
    }
}

NetworkDriver::operator bool() const
{
    return this == NetworkDriver::g_Instance;
}

bool NetworkDriver::isConnected(void) const {
    return this->mStatus.connected;
}


void NetworkDriver::waitForNewProvisioningData(void) {
    do {
        osi_SyncObjWait(&this->mProvisioningDataSemaphore, OSI_WAIT_FOREVER);
    } while (this->addNewProfile() != SUCCESS);
}

long NetworkDriver::addNewProfile(void) {
	if (this->mProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PBC) {
		this->mProvisioningData.secParameters.KeyLen = 0;
		this->mProvisioningData.secParameters.Key = nullptr;

        this->mStatus.reset();
        
		sl_WlanConnect((char *)this->mProvisioningData.wlanSSID.data(), this->mProvisioningData.wlanSSID.length(), 0, &this->mProvisioningData.secParameters, 0);
		return this->waitForConnectWithTimeout(CONNECT_TIMEOUT * 4);

	} else if (this->mProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PIN) {

        this->mStatus.reset();

        sl_WlanConnect((char *)this->mProvisioningData.wlanSSID.data(), this->mProvisioningData.wlanSSID.length(), 0, &this->mProvisioningData.secParameters, 0);
        return this->waitForConnectWithTimeout(CONNECT_TIMEOUT * 4);

	} else {
		long retRes = ERROR;
		retRes = sl_WlanProfileAdd((char *)this->mProvisioningData.wlanSSID.data(), this->mProvisioningData.wlanSSID.length(), 0,	&this->mProvisioningData.secParameters, 0, this->mProvisioningData.priority, 0);
		if (retRes < 0) {
			// Remove all profiles
			retRes = sl_WlanProfileDel(0xFF);
			ASSERT_ON_ERROR(__LINE__, retRes);
			// and try again
			retRes = sl_WlanProfileAdd((char *)this->mProvisioningData.wlanSSID.data(), this->mProvisioningData.wlanSSID.length(), 0,	&this->mProvisioningData.secParameters, 0, this->mProvisioningData.priority, 0);
			ASSERT_ON_ERROR(__LINE__, retRes);
		}
		Trace(ZONE_VERBOSE,"Added Profile at index %d \r\n", retRes);
	}
	return SUCCESS;
}
