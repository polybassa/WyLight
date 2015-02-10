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

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static NetworkDriver* g_CurrentNetworkDriver;

const uint8_t NetworkDriver::SSID_LEN_MAX = 32;
const uint8_t NetworkDriver::BSSID_LEN_MAX = 6;
const uint8_t NetworkDriver::SEC_KEY_LEN_MAX = 64;
const uint8_t NetworkDriver::SL_STOP_TIMEOUT = 30;
const uint8_t NetworkDriver::MAX_NUM_NETWORKENTRIES = 10;
const uint16_t NetworkDriver::CONNECT_TIMEOUT = 20000;
const std::string NetworkDriver::GET_TOKEN = "__SL_G_US";

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
    if (g_CurrentNetworkDriver) {
        g_CurrentNetworkDriver->SimpleLinkWlanEventHandler(pSlWlanEvent);
    }
}
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
    if (g_CurrentNetworkDriver) {
        g_CurrentNetworkDriver->SimpleLinkNetAppEventHandler(pNetAppEvent);
    }
}
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
    if (g_CurrentNetworkDriver) {
        g_CurrentNetworkDriver->SimpleLinkGeneralEventHandler(pDevEvent);
    }
}
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {
    if (g_CurrentNetworkDriver) {
        g_CurrentNetworkDriver->SimpleLinkSockEventHandler(pSock);
    }
}
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse) {
    if (g_CurrentNetworkDriver) {
        g_CurrentNetworkDriver->SimpleLinkHttpServerCallback(pSlHttpServerEvent, pSlHttpServerResponse);
    }
}

void NetworkDriver::SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
	switch (((SlWlanEvent_t*) pSlWlanEvent)->Event) {
	case SL_WLAN_CONNECT_EVENT:
        this->mStatus.connected = true;
        this->mStatus.connectFailed = false;
        this->mInfo.ConnectionSSID = std::string(pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
		this->mInfo.ConnectionBSSID = std::string(pSlWlanEvent->EventData.STAandP2PModeWlanConnected.bssid, SL_BSSID_LENGTH);
		Trace(ZONE_INFO,"[WLAN EVENT] STA Connected to the AP: %s\n\r", this->mInfo.ConnectionSSID.data());
		break;

	case SL_WLAN_DISCONNECT_EVENT:
        this->mStatus.connected = false;
        this->mStatus.IPAcquired = false;
        Trace(ZONE_INFO,"[WLAN EVENT] Device disconnected from AP: %s\n\r", this->mInfo.ConnectionSSID.data());
        this->mInfo.ConnectionSSID.clear();
        this->mInfo.ConnectionBSSID.clear();
		break;

	case SL_WLAN_STA_CONNECTED_EVENT:
		// when device is in AP mode and any client connects to device cc3xxx
        this->mStatus.connected = true;
        this->mStatus.connectFailed = false;
		slPeerInfoAsyncResponse_t *pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
		Trace(ZONE_INFO,"[WLAN EVENT] Client connected: %x:%x:%x:%x:%x:%x\r\n", pEventData->mac[0], pEventData->mac[1],
				pEventData->mac[2], pEventData->mac[3], pEventData->mac[4], pEventData->mac[5]);
		break;

	case SL_WLAN_STA_DISCONNECTED_EVENT:
		// when client disconnects from device (AP)
        this->mStatus.connected = false;
        this->mStatus.IPAcquired = false;
        this->mStatus.IPLeased = false;
		slPeerInfoAsyncResponse_t *pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
		Trace(ZONE_INFO,"[WLAN EVENT] Client disconnected: %x:%x:%x:%x:%x:%x\r\n", pEventData->mac[0], pEventData->mac[1],
				pEventData->mac[2], pEventData->mac[3], pEventData->mac[4], pEventData->mac[5]);
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

	case SL_NETAPP_IP_LEASED:
        this->mStatus.IPLeased = true;
		SlIpLeasedAsync_t *pEventData = &pNetAppEvent->EventData.ipLeased;
		this->mStatus.StationIpAddress = pEventData->ip_address;
		Trace(ZONE_INFO,"[NETAPP EVENT] IP Leased\r\n");
		break;

	case SL_NETAPP_IP_RELEASED:
        this->mStatus.IPLeased = false;
		this->mStatus.StationIpAddress = 0;
		Trace(ZONE_INFO,"[NETAPP EVENT] IP Released\r\n");
		break;

	case SL_NETAPP_SOCKET_TX_FAILED:
		Trace(ZONE_INFO,"[NETAPP EVENT] Socket Error # %d \n\r", pNetAppEvent->EventData.sd);
		break;

	default:
		Trace(ZONE_INFO,"[NETAPP EVENT] Unexpected event \n\r");
		break;
	}
}

void NetworkDriver::SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
	Trace(ZONE_INFO,"[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", pDevEvent->EventData.deviceEvent.status, pDevEvent->EventData.deviceEvent.sender);
}

void NetworkDriver::SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {
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
        std::string serverGetToken(pSlHttpServerEvent->EventData.httpTokenName.data, pSlHttpServerEvent->EventData.httpTokenName.len);
        
        if (serverGetToken.find(GET_TOKEN) == std::string::npos)
            return;
        
        const int getTokenNumber = std::stoi(serverGetToken.substr(GET_TOKEN.length()));
        if (getTokenNumber < 0 || getTokenNumber > MAX_NUM_NETWORKENTRIES)
            return;
        
		memcpy(pSlHttpServerResponse->ResponseData.token_value.data, this->mProvisioningData.networkEntries[getTokenNumber].ssid,
				this->mProvisioningData.networkEntries[getTokenNumber].ssid_len);
		pSlHttpServerResponse->ResponseData.token_value.len =
				this->mProvisioningData.networkEntries[getTokenNumber].ssid_len;

	}
	else if (pSlHttpServerEvent->Event ==  SL_NETAPP_HTTPPOSTTOKENVALUE) {

		Trace(ZONE_VERBOSE," token_name: %s ", pSlHttpServerEvent->EventData.httpPostData.token_name.data);
		Trace(ZONE_VERBOSE," token_data: %s \r\n", pSlHttpServerEvent->EventData.httpPostData.token_value.data);

		if ((0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USC",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len))
				&& (0
						== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_value.data, "Add",
								pSlHttpServerEvent->EventData.httpPostData.token_value.len))) {
			osi_SyncObjSignalFromISR(WlanSupportProvisioningDataAddedSemaphore);

		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USD",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			memcpy(g_ApProvisioningData.wlanSSID, pSlHttpServerEvent->EventData.httpPostData.token_value.data,
					pSlHttpServerEvent->EventData.httpPostData.token_value.len);
			g_ApProvisioningData.wlanSSID[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
		}

		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USE",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {

			if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '0') {
				g_ApProvisioningData.secParameters.Type = SL_SEC_TYPE_OPEN;	//SL_SEC_TYPE_OPEN

			} else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '1') {
				g_ApProvisioningData.secParameters.Type = SL_SEC_TYPE_WEP;	//SL_SEC_TYPE_WEP

			} else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '2') {
				g_ApProvisioningData.secParameters.Type = SL_SEC_TYPE_WPA;	//SL_SEC_TYPE_WPA

			} else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '3') {
				g_ApProvisioningData.secParameters.Type = SL_SEC_TYPE_WPS_PBC;	//SL_SEC_TYPE_WPA

			} else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '4') {
				g_ApProvisioningData.secParameters.Type = SL_SEC_TYPE_WPS_PIN;	//SL_SEC_TYPE_WPA

			} else {
				g_ApProvisioningData.secParameters.Type = SL_SEC_TYPE_OPEN;	//SL_SEC_TYPE_OPEN
			}
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USF",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			memcpy(g_ApProvisioningData.wlanSecurityKey, pSlHttpServerEvent->EventData.httpPostData.token_value.data,
					pSlHttpServerEvent->EventData.httpPostData.token_value.len);
			g_ApProvisioningData.wlanSecurityKey[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
			g_ApProvisioningData.secParameters.Key = g_ApProvisioningData.wlanSecurityKey;
			g_ApProvisioningData.secParameters.KeyLen = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USG",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			g_ApProvisioningData.priority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - '0';
		}
	}
		break;
	default:
		break;
	}
}

static void InitializeAppVariables(void) {
	g_WifiStatusInformation.SimpleLinkStatus = 0;
	g_WifiStatusInformation.StationIpAddress = 0;
	g_WifiStatusInformation.GatewayIpAddress = 0;
	memset(g_WifiStatusInformation.ConnectionSSID, 0, sizeof(g_WifiStatusInformation.ConnectionSSID));
	memset(g_WifiStatusInformation.ConnectionBSSID, 0, sizeof(g_WifiStatusInformation.ConnectionBSSID));
}

static long waitForConnectWithTimeout(unsigned int timeout_ms) {
	unsigned int connectTimeoutCounter = 0;
	//waiting for the device to connect to the AP and obtain ip address
	while ((connectTimeoutCounter < timeout_ms) && !IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
		// wait till connects to an AP
		osi_Sleep(5);	//waiting for 0,5 secs
		connectTimeoutCounter += 5;
	}

	if ((!IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus))
			|| (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus))) {
		Trace(ZONE_ERROR,"Connecting failed \r\n");
		return ERROR;
	} else return SUCCESS;
}

static long ConfigureSimpleLinkToDefaultState() {
	long retRes = ERROR;
	int Mode = ERROR;

	InitializeAppVariables();

	Mode = sl_Start(0, 0, 0);
	ASSERT_ON_ERROR(__LINE__, Mode);

	// If the device is not in station-mode, try putting it in staion-mode
	if (ROLE_STA != Mode) {
		if (ROLE_AP == Mode) {
			// If the device is in AP mode, we need to wait for this event
			// before doing anything
			while (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
				osi_Sleep(10);
			}
		}

		// Switch to STA role and restart
		retRes = sl_WlanSetMode(ROLE_STA);
		ASSERT_ON_ERROR(__LINE__, retRes);

		retRes = sl_Stop(SL_STOP_TIMEOUT);
		ASSERT_ON_ERROR(__LINE__, retRes);

		CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

		retRes = sl_Start(0, 0, 0);
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
	if (0 == sl_WlanDisconnect()) {
		// Wait
		while (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus)) {
			osi_Sleep(50);
		}
	}

// Set connection policy to Auto + Fast Connection
//      (Device's default connection policy)
	retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 1, 0, 0, 0), NULL, 0);
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

	InitializeAppVariables();

	return retRes; // Success
}

static long StartSimpleLinkAsStation() {
	long retRes = ERROR;
	retRes = ConfigureSimpleLinkToDefaultState();
	ASSERT_ON_ERROR(__LINE__, retRes);

	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Started SimpleLink Device in STA Mode\n\r");

	return waitForConnectWithTimeout(CONNECTION_TIMEOUT);
}

static long StartSimpleLinkAsAP() {
	long retRes = ERROR;

	retRes = ConfigureSimpleLinkToDefaultState();
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

	CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

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
	retRes = sl_WlanGetNetworkList(0, MAX_NUM_NETWORKENTRIES, &g_ApProvisioningData.networkEntries[0]);
	ASSERT_ON_ERROR(__LINE__, retRes);

	int i;
	for (i = 0; i < retRes; i++) {
		Trace(ZONE_VERBOSE,"%d) SSID %s\n\r", i, g_ApProvisioningData.networkEntries[i].ssid);
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
	CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

	//Initialize the SLHost Driver
	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Trace(ZONE_VERBOSE,"Start AP\r\n");
	//Wait for Ip Acquired Event in AP Mode
	while (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
		osi_Sleep(10);
	}

	return SUCCESS;
}

static void Network_IF_DisconnectFromAP(void) {
	if (sl_WlanDisconnect() == 0) {
		while (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus))
			osi_Sleep(10);
	}
}

void Network_IF_DeInitDriver(void) {
	Trace(ZONE_VERBOSE,"SL Disconnect...\n\r");
    
	Network_IF_DisconnectFromAP();
	sl_Stop(SL_STOP_TIMEOUT);
	CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);
}

long Network_IF_InitDriver(unsigned int uiMode) {
	if (uiMode == ROLE_STA) {
		unsigned int retry = 2;
		while (retry--) {
			if (SUCCESS == StartSimpleLinkAsStation()) {
				return SUCCESS;
			}
			Network_IF_DeInitDriver();
		}
		return ERROR;
	} else if (uiMode == ROLE_AP) {
		return StartSimpleLinkAsAP();
	} else {
		return ERROR;
	}
}

unsigned char Network_IF_ReadDeviceConfigurationPin(void) {
	unsigned int uiGPIOPort;
	unsigned char pucGPIOPin;

//Read GPIO
	GPIO_IF_GetPortNPin(SH_GPIO_3, &uiGPIOPort, &pucGPIOPin);
	return GPIO_IF_Get(SH_GPIO_3, uiGPIOPort, pucGPIOPin);
}

long Network_IF_AddNewProfile(void) {
	// WPS ?
	if (g_ApProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PBC) {
		g_ApProvisioningData.secParameters.KeyLen = 0;
		g_ApProvisioningData.secParameters.Key = "";

		CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

		sl_WlanConnect(g_ApProvisioningData.wlanSSID, strlen((char*) g_ApProvisioningData.wlanSSID), 0,
				&g_ApProvisioningData.secParameters, 0);
		return waitForConnectWithTimeout(CONNECTION_TIMEOUT * 4);

	} else if (g_ApProvisioningData.secParameters.Type == SL_SEC_TYPE_WPS_PIN) {

		CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

		sl_WlanConnect(g_ApProvisioningData.wlanSSID, strlen((char*) g_ApProvisioningData.wlanSSID), 0,
				&g_ApProvisioningData.secParameters, 0);
		return waitForConnectWithTimeout(CONNECTION_TIMEOUT * 4);

	} else {
		long retRes = ERROR;
		retRes = sl_WlanProfileAdd(g_ApProvisioningData.wlanSSID, strlen((char*) g_ApProvisioningData.wlanSSID), 0,
				&g_ApProvisioningData.secParameters, 0, g_ApProvisioningData.priority, 0);
		if (retRes < 0) {
			// Remove all profiles
			retRes = sl_WlanProfileDel(0xFF);
			ASSERT_ON_ERROR(__LINE__, retRes);
			// and try again
			retRes = sl_WlanProfileAdd(g_ApProvisioningData.wlanSSID, strlen((char*) g_ApProvisioningData.wlanSSID), 0,
					&g_ApProvisioningData.secParameters, 0, g_ApProvisioningData.priority, 0);
			ASSERT_ON_ERROR(__LINE__, retRes);
		}
		Trace(ZONE_VERBOSE,"Added Profile at index %d \r\n", retRes);
	}
	return SUCCESS;
}
