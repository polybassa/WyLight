//*****************************************************************************
// network_if.h
//
// Networking interface functions for CC3200 device
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Simplelink includes 
#include "simplelink.h"

// driverlib includes 
#include "hw_types.h"
#include "timer.h"
#include "rom.h"
#include "rom_map.h"

// common interface includes 
#include "wy_network_if.h"
#include "uart_if.h"
#include "timer_if.h"
#include "gpio_if.h"

#define CONNECTION_TIMEOUT  10000  /* 5sec */
#define TOKEN_ARRAY_SIZE          6
#define STRING_TOKEN_SIZE         10

//
// GLOBAL VARIABLES -- Start
//
struct wifiStatusInformation g_WifiStatusInformation;

// used for ap provisioning
unsigned char g_ucProfileAdded = 0;
unsigned int g_uiIpAddress = 0;
unsigned char g_ucPriority = 7;
char g_cWlanSSID[SSID_LEN_MAX + 1];
char g_cWlanSecurityKey[SEC_KEY_LEN_MAX];
SlSecParams_t g_SecParams;
Sl_WlanNetworkEntry_t g_NetEntries[20];
char g_token_get[TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = { "__SL_G_US0", "__SL_G_US1", "__SL_G_US2", "__SL_G_US3",
		"__SL_G_US4", "__SL_G_US5" };
//
// GLOBAL VARIABLES -- End
//

/* Application specific status/error codes */
typedef enum {
	/* Choosing this number to avoid overlap w/ host-driver's error codes */
	LAN_CONNECTION_FAILED = -0x7D0,
	CLIENT_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
	DEVICE_NOT_IN_STATION_MODE = CLIENT_CONNECTION_FAILED - 1,

	STATUS_CODE_MAX = -0xBB8
} e_AppStatusCodes;

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
	switch (((SlWlanEvent_t*) pSlWlanEvent)->Event) {
	case SL_WLAN_CONNECT_EVENT: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION_FAILED);

		//
		// Information about the connected AP (like name, MAC etc) will be
		// available in 'sl_protocol_wlanConnectAsyncResponse_t'-Applications
		// can use it if required
		//
		//  sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.STAandP2PModeWlanConnected;
		//

		// Copy new connection SSID and BSSID to global parameters
		memcpy(g_WifiStatusInformation.ConnectionSSID, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name,
				pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
		memcpy(g_WifiStatusInformation.ConnectionBSSID, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.bssid,
		SL_BSSID_LENGTH);

		UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s , BSSID: %x:%x:%x:%x:%x:%x\n\r",
				g_WifiStatusInformation.ConnectionSSID, g_WifiStatusInformation.ConnectionBSSID[0],
				g_WifiStatusInformation.ConnectionBSSID[1], g_WifiStatusInformation.ConnectionBSSID[2],
				g_WifiStatusInformation.ConnectionBSSID[3], g_WifiStatusInformation.ConnectionBSSID[4],
				g_WifiStatusInformation.ConnectionBSSID[5]);
	}
		break;

	case SL_WLAN_DISCONNECT_EVENT: {
		sl_protocol_wlanConnectAsyncResponse_t* pEventData = NULL;

		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_AQUIRED);

		pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

		// If the user has initiated 'Disconnect' request,
		//'reason_code' is SL_USER_INITIATED_DISCONNECTION
		if (SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code) {
			UART_PRINT("[WLAN EVENT] Device disconnected from AP: %s , BSSID: %x:%x:%x:%x:%x:%x\n\r",
					g_WifiStatusInformation.ConnectionSSID, g_WifiStatusInformation.ConnectionBSSID[0],
					g_WifiStatusInformation.ConnectionBSSID[1], g_WifiStatusInformation.ConnectionBSSID[2],
					g_WifiStatusInformation.ConnectionBSSID[3], g_WifiStatusInformation.ConnectionBSSID[4],
					g_WifiStatusInformation.ConnectionBSSID[5]);
		} else {
			UART_PRINT("[WLAN ERROR] Device disconnected from AP: %s , BSSID: %x:%x:%x:%x:%x:%x\n\r",
					g_WifiStatusInformation.ConnectionSSID, g_WifiStatusInformation.ConnectionBSSID[0],
					g_WifiStatusInformation.ConnectionBSSID[1], g_WifiStatusInformation.ConnectionBSSID[2],
					g_WifiStatusInformation.ConnectionBSSID[3], g_WifiStatusInformation.ConnectionBSSID[4],
					g_WifiStatusInformation.ConnectionBSSID[5]);
		}
		memset(g_WifiStatusInformation.ConnectionSSID, 0, sizeof(g_WifiStatusInformation.ConnectionSSID));
		memset(g_WifiStatusInformation.ConnectionBSSID, 0, sizeof(g_WifiStatusInformation.ConnectionBSSID));
	}
		break;

	case SL_WLAN_STA_CONNECTED_EVENT: {
		// when device is in AP mode and any client connects to device cc3xxx
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION_FAILED);

		//
		// Information about the connected client (like SSID, MAC etc) will
		// be available in 'slPeerInfoAsyncResponse_t' - Applications
		// can use it if required
		//
		slPeerInfoAsyncResponse_t *pEventData = NULL;
		pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
		const unsigned char BUFFERSIZE = 33;
		unsigned char buffer[BUFFERSIZE];
		memset(buffer, 0, BUFFERSIZE);
		memcpy(buffer, pEventData->go_peer_device_name, pEventData->go_peer_device_name_len);
		UART_PRINT("[WLAN EVENT] Client connected: %s\r\n", buffer);
	}
		break;

	case SL_WLAN_STA_DISCONNECTED_EVENT: {
		// when client disconnects from device (AP)
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_LEASED);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_AQUIRED);

		//
		// Information about the connected client (like SSID, MAC etc) will
		// be available in 'slPeerInfoAsyncResponse_t' - Applications
		// can use it if required
		//
		slPeerInfoAsyncResponse_t *pEventData = NULL;
		pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
		const unsigned char BUFFERSIZE = 33;
		unsigned char buffer[BUFFERSIZE];
		memset(buffer, 0, BUFFERSIZE);
		memcpy(buffer, pEventData->go_peer_device_name, pEventData->go_peer_device_name_len);
		UART_PRINT("[WLAN EVENT] Client disconnected: %s\r\n", buffer);
	}
		break;

	case SL_WLAN_SMART_CONFIG_START_EVENT: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_SMARTCONFIG_START);

		//
		// Information about the SmartConfig details (like Status, SSID,
		// Token etc) will be available in 'slSmartConfigStartAsyncResponse_t'
		// - Applications can use it if required
		//
		//  slSmartConfigStartAsyncResponse_t *pEventData = NULL;
		//  pEventData = &pSlWlanEvent->EventData.smartConfigStartResponse;
		//

	}
		break;

	case SL_WLAN_SMART_CONFIG_STOP_EVENT: {
		// SmartConfig operation finished
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_SMARTCONFIG_START);

		//
		// Information about the SmartConfig details (like Status, padding
		// etc) will be available in 'slSmartConfigStopAsyncResponse_t' -
		// Applications can use it if required
		//
		// slSmartConfigStopAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.smartConfigStopResponse;
		//
	}
		break;

	case SL_WLAN_P2P_DEV_FOUND_EVENT: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_P2P_DEV_FOUND);

		//
		// Information about P2P config details (like Peer device name, own
		// SSID etc) will be available in 'slPeerInfoAsyncResponse_t' -
		// Applications can use it if required
		//
		// slPeerInfoAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.P2PModeDevFound;
		//
	}
		break;

	case SL_WLAN_P2P_NEG_REQ_RECEIVED_EVENT: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_P2P_REQ_RECEIVED);

		//
		// Information about P2P Negotiation req details (like Peer device
		// name, own SSID etc) will be available in 'slPeerInfoAsyncResponse_t'
		//  - Applications can use it if required
		//
		// slPeerInfoAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.P2PModeNegReqReceived;
		//
	}
		break;

	case SL_WLAN_CONNECTION_FAILED_EVENT: {
		// If device gets any connection failed event
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION_FAILED);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_AQUIRED);

		UART_PRINT("[WLAN EVENT] Connection failed\r\n");
	}
		break;

	default: {
		UART_PRINT("[WLAN EVENT] Unexpected event \n\r");
	}
		break;
	}

}

//*****************************************************************************
//
//! \brief This function handles network events such as IP acquisition, IP
//!           leased, IP released etc.
//!
//! \param pNetAppEvent - Pointer indicating device acquired IP
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent) {
	switch (pNetAppEvent->Event) {
	case SL_NETAPP_IPV4_ACQUIRED: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_AQUIRED);

		//
		// Information about the IPv4 & IPv6 details (like IP, gateway,dns
		// etc) will be available in 'SlNetAppEventData_u' - Applications
		// can use it if required
		//
		UART_PRINT("[NETAPP EVENT] IP Acquired\r\n");
	}
		break;

	case SL_NETAPP_IPV6_ACQUIRED: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_AQUIRED);

		//
		// Information about the IPv4 & IPv6 details (like IP, gateway,dns
		// etc) will be available in 'SlNetAppEventData_u' - Applications
		// can use it if required
		//
		// SlNetAppEventData_u *pEventData = NULL;
		// pEventData = &pNetAppEvent->EventData.ipAcquiredV6;
		UART_PRINT("[NETAPP EVENT] IP Acquired\r\n");

	}
		break;

	case SL_NETAPP_IP_LEASED: {
		SET_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_LEASED);

		//
		// Information about the IP-Leased details(like IP-Leased,lease-time,
		// mac etc) will be available in 'SlIpLeasedAsync_t' - Applications
		// can use it if required
		//
		// SlIpLeasedAsync_t *pEventData = NULL;
		// pEventData = &pNetAppEvent->EventData.ipLeased;
		//

		SlIpLeasedAsync_t *pEventData = NULL;
		pEventData = &pNetAppEvent->EventData.ipLeased;
		g_WifiStatusInformation.StationIpAddress = pEventData->ip_address;
		UART_PRINT("[NETAPP EVENT] IP Leased\r\n");

	}
		break;

	case SL_NETAPP_IP_RELEASED: {
		CLR_STATUS_BIT(g_WifiStatusInformation.SimpleLinkStatus, STATUS_BIT_IP_LEASED);

		//
		// Information about the IP-Released details (like IP-address, mac
		// etc) will be available in 'SlIpReleasedAsync_t' - Applications
		// can use it if required
		//
		// SlIpReleasedAsync_t *pEventData = NULL;
		// pEventData = &pNetAppEvent->EventData.ipReleased;
		//
		g_WifiStatusInformation.StationIpAddress = 0;
		UART_PRINT("[NETAPP EVENT] IP Released\r\n");

	}
		break;

	case SL_NETAPP_SOCKET_TX_FAILED: {
		UART_PRINT("[NETAPP EVENT] Socket Error # %d \n\r", pNetAppEvent->EventData.sd);

	}
		break;

	default: {
		UART_PRINT("[NETAPP EVENT] Unexpected event \n\r");
	}
		break;
	}
}

//*****************************************************************************
//
//! \brief This function handles General Events
//!
//! \param[in]     pDevEvent - Pointer to General Event Info 
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
	//
	// Most of the general errors are not FATAL are are to be handled
	// appropriately by the application
	//
	UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", pDevEvent->EventData.deviceEvent.status,
			pDevEvent->EventData.deviceEvent.sender);
}

//*****************************************************************************
//
//! \brief This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock) {
	//
	// This application doesn't work w/ socket - Events are not expected
	//
	UART_PRINT("[SOCK EVENT]");
	switch (pSock->Event) {
	case SL_NETAPP_SOCKET_TX_FAILED:
		switch (pSock->EventData.status) {
		case SL_ECLOSE: {
			UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
					"failed to transmit all queued packets\n\n", pSock->EventData.sd);
		}
			break;

		default: {
			UART_PRINT("[SOCK ERROR] - TX FAILED  :  socket %d , reason "
					"(%d) \n\n", pSock->EventData.sd, pSock->EventData.status);
		}
			break;
		}
		break;

	default: {
		UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n", pSock->Event);
	}
		break;
	}
}

//*****************************************************************************
//
//! \brief This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pHttpServerEvent Pointer indicating http server event
//! \param pHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent,
		SlHttpServerResponse_t *pSlHttpServerResponse) {
	UART_PRINT("[HTTP EVENT]");
	switch (pSlHttpServerEvent->Event) {
	case SL_NETAPP_HTTPGETTOKENVALUE: {

		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[1],
						pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[0].ssid,
					g_NetEntries[0].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[0].ssid_len;
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[2],
						pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[1].ssid,
					g_NetEntries[1].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[1].ssid_len;
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[3],
						pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[2].ssid,
					g_NetEntries[2].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[2].ssid_len;
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[4],
						pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[3].ssid,
					g_NetEntries[3].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[3].ssid_len;
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[5],
						pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[4].ssid,
					g_NetEntries[4].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[4].ssid_len;
		}

		else break;

	}
		break;

	case SL_NETAPP_HTTPPOSTTOKENVALUE: {

		UART_PRINT(" token_name: %s ", pSlHttpServerEvent->EventData.httpPostData.token_name.data);
		UART_PRINT(" token_data: %s \r\n", pSlHttpServerEvent->EventData.httpPostData.token_value.data);

		if ((0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USC",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len))
				&& (0
						== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_value.data, "Add",
								pSlHttpServerEvent->EventData.httpPostData.token_value.len))) {
			g_ucProfileAdded = 1;

		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USD",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			memcpy(g_cWlanSSID, pSlHttpServerEvent->EventData.httpPostData.token_value.data,
					pSlHttpServerEvent->EventData.httpPostData.token_value.len);
			g_cWlanSSID[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
		}

		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USE",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {

			if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '0') {
				g_SecParams.Type = SL_SEC_TYPE_OPEN;	//SL_SEC_TYPE_OPEN

			} else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '1') {
				g_SecParams.Type = SL_SEC_TYPE_WEP;	//SL_SEC_TYPE_WEP

			} else if (pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] == '2') {
				g_SecParams.Type = SL_SEC_TYPE_WPA;	//SL_SEC_TYPE_WPA

			} else {
				g_SecParams.Type = SL_SEC_TYPE_OPEN;	//SL_SEC_TYPE_OPEN
			}
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USF",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			memcpy(g_cWlanSecurityKey, pSlHttpServerEvent->EventData.httpPostData.token_value.data,
					pSlHttpServerEvent->EventData.httpPostData.token_value.len);
			g_cWlanSecurityKey[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
			g_SecParams.Key = g_cWlanSecurityKey;
			g_SecParams.KeyLen = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
			UART_PRINT("new key: %s", g_cWlanSecurityKey);
		}
		if (0
				== memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USG",
						pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			g_ucPriority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - 48;
		}
	}
		break;
	default:
		break;
	}
}

//****************************************************************************
//
//!    \brief This function initializes the application variables
//!
//!    \param[in]  None
//!
//!    \return     0 on success, negative error-code on error
//
//****************************************************************************
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
	while ((connectTimeoutCounter < timeout_ms)
			&& ((!IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus))
					|| (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)))) {
		// wait till connects to an AP
		osi_Sleep(1);	//waiting for 0,5 secs

		if (connectTimeoutCounter % 200 == 0) {
			GPIO_IF_LedToggle(MCU_RED_LED_GPIO);
		}
		connectTimeoutCounter += 1;
	}

	if ((!IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus))
			|| (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus))) return ERROR;
	else return SUCCESS;
}

//*****************************************************************************
//! \brief This function puts the device in its default state. It:
//!           - Set the mode to STATION
//!           - Configures connection policy to Auto and AutoSmartConfig
//!           - Deletes all the stored profiles
//!           - Enables DHCP
//!           - Disables Scan policy
//!           - Sets Tx power to maximum
//!           - Sets power policy to normal
//!           - TBD - Unregister mDNS services
//!
//! \param   none
//! \return  On success, zero is returned. On error, negative is returned
//*****************************************************************************
static long ConfigureSimpleLinkToDefaultState() {

	long retRes = ERROR;
	int Mode = ERROR;

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
			return DEVICE_NOT_IN_STATION_MODE;
		}
	}

//
// Device in station-mode. Disconnect previous connection if any
// The function returns 0 if 'Disconnected done', negative number if already
// disconnected Wait for 'disconnection' event if 0 is returned, Ignore
// other return-codes
//
	retRes = sl_WlanDisconnect();
	if (0 == retRes) {
		// Wait
		while (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus)) {
			osi_Sleep(50);
		}
	}

// Set connection policy to Auto + SmartConfig
//      (Device's default connection policy)
	retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
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

	if (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus)) {
		sl_WlanDisconnect();
	}

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

	UART_PRINT("Started SimpleLink Device in STA Mode\n\r");

	return waitForConnectWithTimeout(CONNECTION_TIMEOUT);
}

//****************************************************************************
//
//! Task function implements the ProvisioningAP functionality
//!
//! \param none
//!
//! This function
//!    1. Starts Device in STA Mode
//!    2. Scans and Stores all the AP
//!    3. Switch to AP Mode and Wait for AP Configuration from Browser
//!    4. Switch to STA Mode and Connect to Configured AP
//!
//! \return None.
//
//****************************************************************************
static long StartSimpleLinkAsAP() {
	long retRes = ERROR;

	retRes = ConfigureSimpleLinkToDefaultState();
	ASSERT_ON_ERROR(__LINE__, retRes);

	UART_PRINT("Device is configured in default state \n\r");

	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	UART_PRINT("Device started \n\r");

	sl_WlanDisconnect();

	// Set connection policy to zero, that no scan in background is performed
	//      (Device's default connection policy)
	retRes = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// Remove all profiles
	retRes = sl_WlanProfileDel(0xFF);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// Restart Simplelink
	sl_Stop(SL_STOP_TIMEOUT);

	CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	unsigned long intervalInSeconds = 2;
	unsigned char parameterLen = sizeof(intervalInSeconds);
	//Scan AP in STA mode
	retRes = sl_WlanPolicySet(SL_POLICY_SCAN, SL_SCAN_POLICY_EN(1), (unsigned char *) &intervalInSeconds, parameterLen);
	ASSERT_ON_ERROR(__LINE__, retRes);

	// wait for scan to complete
	osi_Sleep(5000);

	//Get Scan Result
	retRes = sl_WlanGetNetworkList(0, 20, &g_NetEntries[0]);
	ASSERT_ON_ERROR(__LINE__, retRes);

	int i;
	for (i = 0; i < retRes; i++) {
		UART_PRINT("%d) SSID %s\n\r", i, g_NetEntries[i].ssid);
	}
	//Switch to AP Mode
	sl_WlanSetMode(ROLE_AP);

	unsigned char ssid[] = "WyLightAP";
	retRes = sl_WlanSet(SL_WLAN_CFG_AP_ID, 0, sizeof(ssid), ssid);
	ASSERT_ON_ERROR(__LINE__, retRes);

	sl_WlanDisconnect();
	sl_Stop(SL_STOP_TIMEOUT);
	CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);

	//Initialize the SLHost Driver
	retRes = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, retRes);

	UART_PRINT("Start AP\r\n");
	//Wait for Ip Acquired Event in AP Mode
	while (!IS_IP_ACQUIRED(g_WifiStatusInformation.SimpleLinkStatus)) {
		osi_Sleep(10);
	}
	g_ucProfileAdded = 0;

	return SUCCESS;
}

//*****************************************************************************
//
//! Disconnect  Disconnects from an Access Point
//!
//! \param  none
//!
//! \return none
//
//*****************************************************************************
static void Network_IF_DisconnectFromAP(void) {
	if (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus)) {
		if (sl_WlanDisconnect() == 0) {
			while (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus))
				osi_Sleep(10);
		}
	}

}

//*****************************************************************************
//
//! Network_IF_DeInitDriver
//! The function de-initializes a CC3200 device
//!  
//! \param  None
//!  
//! \return none
//
//*****************************************************************************
void Network_IF_DeInitDriver(void) {
	UART_PRINT("SL Disconnect...\n\r");

//
// Disconnect from the AP
//
	Network_IF_DisconnectFromAP();

//
// Stop the simplelink host
//
	sl_Stop(SL_STOP_TIMEOUT);

//
// Reset the state to uninitialized
//
	CLR_STATUS_BIT_ALL(g_WifiStatusInformation.SimpleLinkStatus);
}

//*****************************************************************************
//
//! Network_IF_InitDriver
//! The function initializes a CC3200 device and triggers it to start operation
//!
//! \param  uiMode (device mode in which device will be configured)
//!
//! \return none
//
//*****************************************************************************
long Network_IF_InitDriver(unsigned int uiMode) {

	long lRetVal = -1;
	unsigned int retry = 1;

	if (uiMode == ROLE_STA) {
		do {
			lRetVal = StartSimpleLinkAsStation();
			if (lRetVal == SUCCESS) {
				return SUCCESS;
			}
			Network_IF_DeInitDriver();
		} while (retry--);
		return ERROR;
	} else if (uiMode == ROLE_AP) {
		return StartSimpleLinkAsAP();
	} else {
		return ERROR;
	}
}

//****************************************************************************
//
//!	\brief Read Force AP GPIO and Configure Mode - 1(Access Point Mode)
//!                                                  - 0 (Station Mode)
//!
//! \return	                	None
//
//****************************************************************************
unsigned char Network_IF_ReadDeviceConfigurationPin(void) {
	unsigned int uiGPIOPort;
	unsigned char pucGPIOPin;

//Read GPIO
	GPIO_IF_GetPortNPin(SH_GPIO_3, &uiGPIOPort, &pucGPIOPin);
	return GPIO_IF_Get(SH_GPIO_3, uiGPIOPort, pucGPIOPin);
}

long Network_IF_CheckForNewProfile(void) {
	if (!g_ucProfileAdded) {
		return ERROR;
	}
	long retRes = ERROR;
	retRes = sl_WlanProfileAdd(g_cWlanSSID, strlen((char*) g_cWlanSSID), 0, &g_SecParams, 0, g_ucPriority, 0);
	ASSERT_ON_ERROR(__LINE__, retRes);

	Network_IF_DeInitDriver();
	g_ucProfileAdded = 0;
	return SUCCESS;
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

