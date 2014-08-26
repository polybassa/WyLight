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
#include "utils.h"

// common interface includes 
#include "wy_network_if.h"
#include "uart_if.h"
#include "timer_if.h"
#include "gpio_if.h"

#define CONNECTION_TIMEOUT_COUNT  5000  /* 5sec */
#define TOKEN_ARRAY_SIZE          6
#define STRING_TOKEN_SIZE         10
#define AP_SSID_LEN_MAX           33

//
// GLOBAL VARIABLES -- Start
//
unsigned long g_ulStatus = 0; /* SimpleLink Status */
unsigned long g_ulStaIp = 0; /* Station IP address */
unsigned long g_ulGatewayIP = 0; /* Network Gateway IP address */
unsigned char g_ucConnectionSSID[SSID_LEN_MAX + 1]; /* Connection SSID */
unsigned char g_ucConnectionBSSID[BSSID_LEN_MAX]; /* Connection BSSID */
unsigned short g_usConnectIndex; /* Connection time delay index */
unsigned int g_uiDeviceModeConfig; /* ROLE_STA or ROLE_AP, depending on stored profiles and jumper settings */
const char pcDigits[] = "0123456789"; /* variable used by itoa function */

// used for ap provisioning
unsigned char g_ucProfileAdded = 1;
unsigned char g_ucConnectedToConfAP = 0, g_ucProvisioningDone = 0;
unsigned int g_uiIpAddress = 0;
unsigned char g_ucPriority, g_ucIpLeased = 0;
char g_cWlanSSID[AP_SSID_LEN_MAX];
char g_cWlanSecurityKey[50];
SlSecParams_t g_SecParams;
Sl_WlanNetworkEntry_t g_NetEntries[20];
char g_token_get[TOKEN_ARRAY_SIZE][STRING_TOKEN_SIZE] = { "__SL_G_US0", "__SL_G_US1", "__SL_G_US2", "__SL_G_US3", "__SL_G_US4", "__SL_G_US5" };
//
// GLOBAL VARIABLES -- End
//

/* Application specific status/error codes */
typedef enum {
	/* Choosing this number to avoid overlap w/ host-driver's error codes */
	LAN_CONNECTION_FAILED = -0x7D0, CLIENT_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1, DEVICE_NOT_IN_STATION_MODE = CLIENT_CONNECTION_FAILED - 1,

	STATUS_CODE_MAX = -0xBB8
} e_AppStatusCodes;

//
// LOCAL FUNCTION PROTOTYPES -- Start
//
static void ReadDeviceConfiguration();
static void InitializeAppVariables();
static void WlanConnect();
static long ConfigureSimpleLinkAsStation();
static long StartSimpleLinkAsStation();
static long ConfigureSimpleLinkToDefaultState();
static long StartSimpleLinkAsAP();

//
// LOCAL FUNCTION PROTOTYPES -- End
//

#ifdef USE_FREERTOS
//*****************************************************************************
// FreeRTOS User Hook Functions enabled in FreeRTOSConfig.h
//*****************************************************************************

//*****************************************************************************
//
//! \brief Application defined hook (or callback) function - assert
//!
//! \param[in]  pcFile - Pointer to the File Name
//! \param[in]  ulLine - Line Number
//! 
//! \return none
//!
//*****************************************************************************
void vAssertCalled(const char *pcFile, unsigned long ulLine) {
	//Handle Assert here
	while (1) {
	}
}

//*****************************************************************************
//
//! \brief Application defined idle task hook
//! 
//! \param  none
//! 
//! \return none
//!
//*****************************************************************************
void vApplicationIdleHook(void) {
	//Handle Idle Hook for Profiling, Power Management etc
}

//*****************************************************************************
//
//! \brief Application defined malloc failed hook
//! 
//! \param  none
//! 
//! \return none
//!
//*****************************************************************************
void vApplicationMallocFailedHook() {
	//Handle Memory Allocation Errors
	while (1) {
	}
}

//*****************************************************************************
//
//! \brief Application defined stack overflow hook
//! 
//! \param  none
//! 
//! \return none
//!
//*****************************************************************************
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed portCHAR *pcTaskName) {
	//Handle FreeRTOS Stack Overflow
	while (1) {
	}
}

#endif /*USE_FREERTOS */

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
	UART_PRINT("[WLAN EVENT]");
	switch (((SlWlanEvent_t*) pSlWlanEvent)->Event) {
	case SL_WLAN_CONNECT_EVENT: {
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);

		//
		// Information about the connected AP (like name, MAC etc) will be
		// available in 'sl_protocol_wlanConnectAsyncResponse_t'-Applications
		// can use it if required
		//
		//  sl_protocol_wlanConnectAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.STAandP2PModeWlanConnected;
		//

		// Copy new connection SSID and BSSID to global parameters
		memcpy(g_ucConnectionSSID, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
		memcpy(g_ucConnectionBSSID, pSlWlanEvent->EventData.STAandP2PModeWlanConnected.bssid,
		SL_BSSID_LENGTH);

		UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s , BSSID: %x:%x:%x:%x:%x:%x\n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1],
				g_ucConnectionBSSID[2], g_ucConnectionBSSID[3], g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
	}
		break;

	case SL_WLAN_DISCONNECT_EVENT: {
		sl_protocol_wlanConnectAsyncResponse_t* pEventData = NULL;

		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

		pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

		// If the user has initiated 'Disconnect' request,
		//'reason_code' is SL_USER_INITIATED_DISCONNECTION
		if (SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code) {
			UART_PRINT("[WLAN EVENT]Device disconnected from the AP: %s, "
					"BSSID: %x:%x:%x:%x:%x:%x on application's request "
					"\n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1], g_ucConnectionBSSID[2], g_ucConnectionBSSID[3], g_ucConnectionBSSID[4],
					g_ucConnectionBSSID[5]);
		} else {
			UART_PRINT("[WLAN ERROR]Device disconnected from the AP AP: %s,"
					" BSSID: %x:%x:%x:%x:%x:%x on an ERROR..!! \n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1], g_ucConnectionBSSID[2],
					g_ucConnectionBSSID[3], g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
		}
		memset(g_ucConnectionSSID, 0, sizeof(g_ucConnectionSSID));
		memset(g_ucConnectionBSSID, 0, sizeof(g_ucConnectionBSSID));
	}
		break;

	case SL_WLAN_STA_CONNECTED_EVENT: {
		// when device is in AP mode and any client connects to device cc3xxx
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);

		//
		// Information about the connected client (like SSID, MAC etc) will
		// be available in 'slPeerInfoAsyncResponse_t' - Applications
		// can use it if required
		//
		// slPeerInfoAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.APModeStaConnected;
		//

	}
		break;

	case SL_WLAN_STA_DISCONNECTED_EVENT: {
		// when client disconnects from device (AP)
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

		//
		// Information about the connected client (like SSID, MAC etc) will
		// be available in 'slPeerInfoAsyncResponse_t' - Applications
		// can use it if required
		//
		// slPeerInfoAsyncResponse_t *pEventData = NULL;
		// pEventData = &pSlWlanEvent->EventData.APModestaDisconnected;
		//
	}
		break;

	case SL_WLAN_SMART_CONFIG_START_EVENT: {
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_SMARTCONFIG_START);

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
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_SMARTCONFIG_START);

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
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_P2P_DEV_FOUND);

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
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_P2P_REQ_RECEIVED);

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
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);
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
	UART_PRINT("[NETAPP EVENT]");
	switch (pNetAppEvent->Event) {
	case SL_NETAPP_IPV4_ACQUIRED: {
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

		//
		// Information about the IPv4 & IPv6 details (like IP, gateway,dns
		// etc) will be available in 'SlNetAppEventData_u' - Applications
		// can use it if required
		//
		UART_PRINT("[NETAPP EVENT] IP Acquired\r\n");
	}
		break;
	case SL_NETAPP_IPV6_ACQUIRED: {
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

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
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

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
		g_ulStaIp = pEventData->ip_address;
	}
		break;

	case SL_NETAPP_IP_RELEASED: {
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);

		//
		// Information about the IP-Released details (like IP-address, mac
		// etc) will be available in 'SlIpReleasedAsync_t' - Applications
		// can use it if required
		//
		// SlIpReleasedAsync_t *pEventData = NULL;
		// pEventData = &pNetAppEvent->EventData.ipReleased;
		//
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
	UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", pDevEvent->EventData.deviceEvent.status, pDevEvent->EventData.deviceEvent.sender);
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
		case SL_ECLOSE:
			UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
					"failed to transmit all queued packets\n\n", pSock->EventData.sd);
			break;
		default:
			UART_PRINT("[SOCK ERROR] - TX FAILED  :  socket %d , reason "
					"(%d) \n\n", pSock->EventData.sd, pSock->EventData.status);
			break;
		}
		break;

	default:
		UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n", pSock->Event);
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
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse) {
	UART_PRINT("[HTTP EVENT]");
	switch (pSlHttpServerEvent->Event) {
	case SL_NETAPP_HTTPGETTOKENVALUE: {

		if (0 == memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[0], pSlHttpServerEvent->EventData.httpTokenName.len)) {
			if (g_ucConnectedToConfAP == 1) {
				// Important - Connection Status
				memcpy(pSlHttpServerResponse->ResponseData.token_value.data, "TRUE", strlen("TRUE"));
				pSlHttpServerResponse->ResponseData.token_value.len = strlen("TRUE");
			} else {
				// Important - Connection Status
				memcpy(pSlHttpServerResponse->ResponseData.token_value.data, "FALSE", strlen("FALSE"));
				pSlHttpServerResponse->ResponseData.token_value.len = strlen("FALSE");
			}
		}

		if (0 == memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[1], pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[0].ssid, g_NetEntries[0].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[0].ssid_len;
		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[2], pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[1].ssid, g_NetEntries[1].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[1].ssid_len;
		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[3], pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[2].ssid, g_NetEntries[2].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[2].ssid_len;
		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[4], pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[3].ssid, g_NetEntries[3].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[3].ssid_len;
		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpTokenName.data, g_token_get[5], pSlHttpServerEvent->EventData.httpTokenName.len)) {
			// Important - Token value len should be < MAX_TOKEN_VALUE_LEN
			memcpy(pSlHttpServerResponse->ResponseData.token_value.data, g_NetEntries[4].ssid, g_NetEntries[4].ssid_len);
			pSlHttpServerResponse->ResponseData.token_value.len = g_NetEntries[4].ssid_len;
		}

		else break;

	}
		break;

	case SL_NETAPP_HTTPPOSTTOKENVALUE: {

		if ((0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USC", pSlHttpServerEvent->EventData.httpPostData.token_name.len))
				&& (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_value.data, "Add", pSlHttpServerEvent->EventData.httpPostData.token_value.len))) {
			g_ucProfileAdded = 0;

		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USD", pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			memcpy(g_cWlanSSID, pSlHttpServerEvent->EventData.httpPostData.token_value.data, pSlHttpServerEvent->EventData.httpPostData.token_value.len);
			g_cWlanSSID[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
		}

		if (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USE", pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {

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
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USF", pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			memcpy(g_cWlanSecurityKey, pSlHttpServerEvent->EventData.httpPostData.token_value.data, pSlHttpServerEvent->EventData.httpPostData.token_value.len);
			g_cWlanSecurityKey[pSlHttpServerEvent->EventData.httpPostData.token_value.len] = 0;
			g_SecParams.Key = g_cWlanSecurityKey;
			g_SecParams.KeyLen = pSlHttpServerEvent->EventData.httpPostData.token_value.len;
		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_USG", pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			g_ucPriority = pSlHttpServerEvent->EventData.httpPostData.token_value.data[0] - 48;
		}
		if (0 == memcmp(pSlHttpServerEvent->EventData.httpPostData.token_name.data, "__SL_P_US0", pSlHttpServerEvent->EventData.httpPostData.token_name.len)) {
			g_ucProvisioningDone = 1;
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
static void InitializeAppVariables() {
	g_ulStatus = 0;
	g_ulStaIp = 0;
	g_ulGatewayIP = 0;
	memset(g_ucConnectionSSID, 0, sizeof(g_ucConnectionSSID));
	memset(g_ucConnectionBSSID, 0, sizeof(g_ucConnectionBSSID));
}

static long waitForConnectWithTimeout(unsigned int timeout_ms) {
	unsigned int connectTimeoutCounter = 0;
	//waiting for the device to connect to the AP and obtain ip address
	while ((connectTimeoutCounter < timeout_ms) && ((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus)))) {
#ifdef USE_FREERTOS
		// wait till connects to an AP
		osi_Sleep(1);	//waiting for 0,5 secs
#else
				_SlNonOsMainLoopTask();
				MAP_UtilsDelay(100000);
#endif
		if (connectTimeoutCounter % 200 == 0) {
			GPIO_IF_LedToggle(MCU_RED_LED_GPIO);
		}
		connectTimeoutCounter += 1;
	}

	if ((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus))) return ERROR;
	else return SUCCESS;
}

static void WlanConnect() {
//Add Profile
	sl_WlanProfileAdd(g_cWlanSSID, strlen((char*) g_cWlanSSID), 0, &g_SecParams, 0, g_ucPriority, 0);

//Connecting to the Access point
	sl_WlanConnect(g_cWlanSSID, strlen((char*) g_cWlanSSID), 0, &g_SecParams, 0);

	waitForConnectWithTimeout(CONNECTION_TIMEOUT_COUNT);

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

static long ConfigureSimpleLinkAsStation() {
	SlVersionFull ver = { 0 };

	unsigned char ucVal = 1;
	unsigned char ucConfigOpt = 0;
	unsigned char ucConfigLen = 0;
	unsigned char ucPower = 0;

	long lRetVal = -1;
	long lMode = -1;

	lMode = sl_Start(0, 0, 0);
	ASSERT_ON_ERROR(__LINE__, lMode);

// Get the device's version-information
	ucConfigOpt = SL_DEVICE_GENERAL_VERSION;
	ucConfigLen = sizeof(ver);
	lRetVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt, &ucConfigLen, (unsigned char *) (&ver));
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	UART_PRINT("Host Driver Version: %s\n\r", SL_DRIVER_VERSION);
	UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r", ver.NwpVersion[0], ver.NwpVersion[1], ver.NwpVersion[2], ver.NwpVersion[3],
			ver.ChipFwAndPhyVersion.FwVersion[0], ver.ChipFwAndPhyVersion.FwVersion[1], ver.ChipFwAndPhyVersion.FwVersion[2], ver.ChipFwAndPhyVersion.FwVersion[3],
			ver.ChipFwAndPhyVersion.PhyVersion[0], ver.ChipFwAndPhyVersion.PhyVersion[1], ver.ChipFwAndPhyVersion.PhyVersion[2], ver.ChipFwAndPhyVersion.PhyVersion[3]);

// If the device is not in station-mode, try putting it in staion-mode
	if (ROLE_STA != lMode) {
		if (ROLE_AP == lMode) {
			// If the device is in AP mode, we need to wait for this event
			// before doing anything
			while (!IS_IP_ACQUIRED(g_ulStatus)) {
#ifndef SL_PLATFORM_MULTI_THREADED
				_SlNonOsMainLoopTask();
#endif
			}
		}

		// Switch to STA role and restart
		lRetVal = sl_WlanSetMode(ROLE_STA);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		lRetVal = sl_Stop(SL_STOP_TIMEOUT);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		// reset the Status bits
		CLR_STATUS_BIT_ALL(g_ulStatus);

		lRetVal = sl_Start(0, 0, 0);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		// Check if the device is in station again
		if (ROLE_STA != lRetVal) {
			// We don't want to proceed if the device is not coming up in
			// station-mode
			return DEVICE_NOT_IN_STATION_MODE;
		}
	}

//
// Device in station-mode. Disconnect previous connection if any
// The function returns 0 if 'Disconnected done', negative number if already
// disconnected Wait for 'disconnection' event if 0 is returned, Ignore
// other return-codes
//
	lRetVal = sl_WlanDisconnect();
	if (0 == lRetVal) {
		// Wait
		while (IS_CONNECTED(g_ulStatus)) {
#ifndef SL_PLATFORM_MULTI_THREADED
			_SlNonOsMainLoopTask();
#endif
		}
	}

	// Set connection policy to Auto + SmartConfig
	//      (Device's default connection policy)
	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 0), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Enable DHCP client
	lRetVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, 1, &ucVal);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Disable scan
	ucConfigOpt = SL_SCAN_POLICY(0);
	unsigned long intervalInSeconds = 10;
	lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN, ucConfigOpt, (unsigned char *) &intervalInSeconds, sizeof(intervalInSeconds));
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Set Tx power level for station mode
// Number between 0-15, as dB offset from max power - 0 will set max power
	ucPower = 0;
	lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
	WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *) &ucPower);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Set PM policy to normal
	lRetVal = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	lRetVal = sl_Stop(SL_STOP_TIMEOUT);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	InitializeAppVariables();

	return lRetVal; // Success
}

static long StartSimpleLinkAsStation() {
	long lRetVal = ERROR;

	lRetVal = ConfigureSimpleLinkAsStation();
	if (lRetVal < 0) {
		if (DEVICE_NOT_IN_STATION_MODE == lRetVal)
		UART_PRINT("Failed to configure the device in its default state \n\r");
		// TODO: Remove endless loop
		LOOP_FOREVER(__LINE__);
		return ERROR;
	}

	lRetVal = sl_Start(NULL, NULL, NULL);
	if (lRetVal < 0 || lRetVal != ROLE_STA) {
		UART_PRINT("Failed to start the device \n\r");
		// TODO: Remove endless loop
		LOOP_FOREVER(__LINE__);
		return ERROR;
	}

	UART_PRINT("Started SimpleLink Device: STA Mode\n\r");

	return waitForConnectWithTimeout(8000);
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
	unsigned char ucVal = 1;
	unsigned char ucConfigOpt = 0;
	unsigned char ucPower = 0;

	long lRetVal = -1;
	long lMode = -1;

	lMode = sl_Start(0, 0, 0);
	ASSERT_ON_ERROR(__LINE__, lMode);

// If the device is not in station-mode, try putting it in staion-mode
	if (ROLE_STA != lMode) {
		if (ROLE_AP == lMode) {
			// If the device is in AP mode, we need to wait for this event
			// before doing anything
			while (!IS_IP_ACQUIRED(g_ulStatus)) {
#ifndef SL_PLATFORM_MULTI_THREADED
				_SlNonOsMainLoopTask();
#endif
			}
		}

		// Switch to STA role and restart
		lRetVal = sl_WlanSetMode(ROLE_STA);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		lRetVal = sl_Stop(SL_STOP_TIMEOUT);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		CLR_STATUS_BIT_ALL(g_ulStatus);

		lRetVal = sl_Start(0, 0, 0);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		// Check if the device is in station again
		if (ROLE_STA != lRetVal) {
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
	lRetVal = sl_WlanDisconnect();
	if (0 == lRetVal) {
		// Wait
		while (IS_CONNECTED(g_ulStatus)) {
#ifndef SL_PLATFORM_MULTI_THREADED
			_SlNonOsMainLoopTask();
#endif
		}
	}

	// Set connection policy to Auto + SmartConfig
	//      (Device's default connection policy)
	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Remove all profiles
	lRetVal = sl_WlanProfileDel(0xFF);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Enable DHCP client
	lRetVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE, 1, 1, &ucVal);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Disable scan
	ucConfigOpt = SL_SCAN_POLICY(0);
	lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN, ucConfigOpt, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Set Tx power level for station mode
// Number between 0-15, as dB offset from max power - 0 will set max power
	ucPower = 0;
	lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
	WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, sizeof(ucPower), (unsigned char *) &ucPower);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

// Set PM policy to normal
	lRetVal = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	lRetVal = sl_Stop(SL_STOP_TIMEOUT);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	InitializeAppVariables();

	return lRetVal; // Success
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
	long lRetVal = -1;
	InitializeAppVariables();
	lRetVal = ConfigureSimpleLinkToDefaultState();
	if (lRetVal < 0) {
		if (DEVICE_NOT_IN_STATION_MODE == lRetVal)
		UART_PRINT("Failed to configure the device in its default state \n\r");

		LOOP_FOREVER(__LINE__);
		return ERROR;
	}

	UART_PRINT("Device is configured in default state \n\r");

	lRetVal = sl_Start(NULL, NULL, NULL);
	if (lRetVal < 0) {
		UART_PRINT("Failed to start the device \n\r");
		LOOP_FOREVER(__LINE__);
	}

	UART_PRINT("Device started as STATION \n\r");

	if (lRetVal == ROLE_AP) {
		//Device in AP-Mode, Wait for initialization to complete
		while (!IS_IP_ACQUIRED(g_ulStatus)) {
#ifdef USE_FREERTOS
			// wait till connects to an AP
			osi_Sleep(100); //waiting for 0,1 secs
#else
					_SlNonOsMainLoopTask();
					MAP_UtilsDelay(100);
#endif
		}

	}
	if (lRetVal != ROLE_AP) {
		sl_WlanDisconnect();
	}
	sl_WlanSetMode(ROLE_STA);

	// Set connection policy to zero, that no scan in background is performed
	//      (Device's default connection policy)
	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	sl_Stop(SL_STOP_TIMEOUT);
	CLR_STATUS_BIT_ALL(g_ulStatus);

	sl_Start(NULL, NULL, NULL);

	while (!g_ucProvisioningDone) {

		unsigned long intervalInSeconds = 5;
		unsigned char parameterLen = sizeof(intervalInSeconds);
		//Scan AP in STA mode
		lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN, SL_SCAN_POLICY_EN(1), (unsigned char *) &intervalInSeconds, parameterLen);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		// wait for scan to complete
		MAP_UtilsDelay(80000000);

		//Get Scan Result
		lRetVal = sl_WlanGetNetworkList(0, 20, &g_NetEntries[0]);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		char message[100];
		int i;
		for (i = 0; i < lRetVal; i++) {
			snprintf(message, 60, "%d) SSID %s\n\r", i, g_NetEntries[i].ssid);
			UART_PRINT("%d) SSID %s\n\r", i, g_NetEntries[i].ssid);
		}
		//Switch to AP Mode
		sl_WlanSetMode(ROLE_AP);

		unsigned char ssid[] = "WyLightAP";

		lRetVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, 0, strlen((const char *) ssid), ssid);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		sl_WlanDisconnect();
		sl_Stop(SL_STOP_TIMEOUT);
		CLR_STATUS_BIT_ALL(g_ulStatus);

		//Initialize the SLHost Driver
		sl_Start(NULL, NULL, NULL);
		UART_PRINT("Start AP\r\n");
		//Wait for Ip Acquired Event in AP Mode
		while (!IS_IP_ACQUIRED(g_ulStatus)) {
			MAP_UtilsDelay(100);
		}

		// Wait for AP Configuration, Open Browser and Configure AP
		while (g_ucProfileAdded && !g_ucProvisioningDone) {
			MAP_UtilsDelay(100);
		}

		g_ucProfileAdded = 1;

		//Switch to STA Mode
		sl_WlanSetMode(ROLE_STA);

		//AP Configured, Restart in STA Mode
		sl_Stop(SL_STOP_TIMEOUT);

		CLR_STATUS_BIT_ALL(g_ulStatus);

		//MAP_UtilsDelay(10000000);
		sl_Start(NULL, NULL, NULL);

		// Connect to the Configured Access Point
		WlanConnect();

		return SUCCESS;
	}
	return ERROR;

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
void Network_IF_InitDriver(unsigned int uiMode) {
	GPIO_IF_LedConfigure(LED1 | LED2 | LED3);
	InitializeAppVariables();

	long lRetVal = -1;

// Test if AP-Mode jumper is set
	ReadDeviceConfiguration();

	if (g_uiDeviceModeConfig == ROLE_STA) {
		// Reset CC3200 Network State Machine
		lRetVal = StartSimpleLinkAsStation();
		if (lRetVal != SUCCESS) {
			// try again in AP MODE
			g_uiDeviceModeConfig = ROLE_AP;
		}
	}

	if (g_uiDeviceModeConfig == ROLE_AP) {
		// Reset CC3200 Network State Machine
		InitializeAppVariables();
		lRetVal = StartSimpleLinkAsAP();
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
	Network_IF_ResetMCUStateMachine();
}

//*****************************************************************************
//
//! Network_IF_ConnectAP  Connect to an Access Point using the specified SSID
//!
//! \param  pcSsid is a string of the AP's SSID
//!
//! \return none
//
//*****************************************************************************
int Network_IF_ConnectAP(char *pcSsid, SlSecParams_t SecurityParams) {
	char acCmdStore[128];
	unsigned short usConnTimeout;
	int iRetVal;
	unsigned long ulIP = 0;
	unsigned long ulSubMask = 0;
	unsigned long ulDefGateway = 0;
	unsigned long ulDns = 0;
	unsigned char ucRecvdAPDetails;

	Network_IF_DisconnectFromAP();

//
// This triggers the CC3200 to connect to specific AP
//
	sl_WlanConnect(pcSsid, strlen(pcSsid), NULL, &SecurityParams, NULL);

//
// Wait to check if connection to DIAGNOSTIC_AP succeeds
//
	while (g_usConnectIndex < 10) {
		MAP_UtilsDelay(8000000);
		g_usConnectIndex++;
		if (IS_CONNECTED(g_ulStatus)) {
			break;
		}
	}

//
// Check and loop until async event from network processor indicating Wlan
// Connect success was received
//
	while (!(IS_CONNECTED(g_ulStatus))) {
		//
		// Disconnect the previous attempt
		//
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);
		UART_PRINT("Device could not connect to %s\n\r", pcSsid);

		do {
			ucRecvdAPDetails = 0;

			UART_PRINT("\n\r\n\rPlease enter the AP(open) SSID name # ");

			//
			// Get the AP name to connect over the UART
			//
			iRetVal = GetCmd(acCmdStore, sizeof(acCmdStore));
			if (iRetVal > 0) {
				//
				// Parse the AP name
				//
				pcSsid = strtok(acCmdStore, ":");
				if (pcSsid != NULL) {

					ucRecvdAPDetails = 1;

				}
			}
		} while (ucRecvdAPDetails == 0);

		UART_PRINT("\n\rTrying to connect to AP: %s ...\n\r", pcSsid);
		/*UART_PRINT("Key: %s, Len: %d, KeyID: %d, Type: %d\n\r",
		 SecurityParams.Key, SecurityParams.KeyLen
		 , SecurityParams.Type);
		 */

		//
		// Get the current timer tick and setup the timeout accordingly
		//
		usConnTimeout = g_usConnectIndex + 10;

		//
		// This triggers the CC3200 to connect to specific AP
		//
		sl_WlanConnect(pcSsid, strlen(pcSsid), NULL, &SecurityParams, NULL);

		//
		// Wait to check if connection to specifed AP succeeds
		//
		while (!(IS_CONNECTED(g_ulStatus))) {
			if (g_usConnectIndex >= usConnTimeout) {
				break;
			}
			MAP_UtilsDelay(8000000);
			g_usConnectIndex++;
		}
	}

//
// Wait until IP is acquired
//
	while (!(IS_IP_ACQUIRED(g_ulStatus)))
		;

//
// Put message on UART
//
	UART_PRINT("\n\rDevice has connected to %s\n\r", pcSsid);

//
// Get IP address
//
	Network_IF_IpConfigGet(&ulIP, &ulSubMask, &ulDefGateway, &ulDns);

//
// Send the information
//
	UART_PRINT("Device IP Address is %d.%d.%d.%d \n\r\n\r", SL_IPV4_BYTE(ulIP, 3), SL_IPV4_BYTE(ulIP, 2), SL_IPV4_BYTE(ulIP, 1), SL_IPV4_BYTE(ulIP, 0));
	return 0;
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
void Network_IF_DisconnectFromAP() {
	if (IS_CONNECTED(g_ulStatus)) sl_WlanDisconnect();

	while (IS_CONNECTED(g_ulStatus))
		;
}

//*****************************************************************************
//
//! Network_IF_IpConfigGet  Get the IP Address of the device.
//!
//! \param  pulIP IP Address of Device
//! \param  pulSubnetMask Subnetmask of Device
//! \param  pulDefaultGateway Default Gateway value
//! \param  pulDNSServer DNS Server
//!
//! \return none
//
//*****************************************************************************
int Network_IF_IpConfigGet(unsigned long *pulIP, unsigned long *pulSubnetMask, unsigned long *pulDefaultGateway, unsigned long *pulDNSServer) {
	unsigned char isDhcp;
	unsigned char len = sizeof(_NetCfgIpV4Args_t);
	_NetCfgIpV4Args_t ipV4 = { 0 };

	sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO, &isDhcp, &len, (unsigned char *) &ipV4);

	*pulIP = ipV4.ipV4;
	*pulSubnetMask = ipV4.ipV4Mask;
	*pulDefaultGateway = ipV4.ipV4Gateway;
	*pulDefaultGateway = ipV4.ipV4DnsServer;

	return 1;
}

//*****************************************************************************
//
//! Network_IF_GetHostIP
//!
//! \brief  This function obtains the server IP address using a DNS lookup
//!
//! \param  The server hostname
//!
//! \return IP address: Success or 0: Failure.
//!
//
//*****************************************************************************
unsigned long Network_IF_GetHostIP(char* pcHostName) {
	int iStatus = 0;
	unsigned long ulDestinationIP;

	iStatus = sl_NetAppDnsGetHostByName(pcHostName, strlen(pcHostName), &ulDestinationIP, SL_AF_INET);
	if (iStatus == 0) {
		UART_PRINT("Get Host IP succeeded.\n\rHost: %s IP: %d.%d.%d.%d \n\r\n\r", pcHostName, SL_IPV4_BYTE(ulDestinationIP, 3), SL_IPV4_BYTE(ulDestinationIP, 2),
				SL_IPV4_BYTE(ulDestinationIP, 1), SL_IPV4_BYTE(ulDestinationIP, 0));
		return ulDestinationIP;
	} else {
		return 0;
	}
}

//*****************************************************************************
//
//!  \brief  Reset state from the state machine
//!
//!  \param  None
//!
//!  \return none
//!
//*****************************************************************************
void Network_IF_ResetMCUStateMachine() {
	g_ulStatus = 0;
}

//*****************************************************************************
//
//!  \brief  Return the current state bits
//!
//!  \param  None
//!
//!  \return none
//!
//
//*****************************************************************************
unsigned long Network_IF_CurrentMCUState() {
	return g_ulStatus;
}
//*****************************************************************************
//
//!  \brief  sets a state from the state machine
//!
//!  \param  cStat Status of State Machine defined in e_StatusBits
//!
//!  \return none
//!
//*****************************************************************************
void Network_IF_SetMCUMachineState(char cStat) {
	SET_STATUS_BIT(g_ulStatus, cStat);
}

//*****************************************************************************
//
//!  \brief  Unsets a state from the state machine
//!
//!  \param  cStat Status of State Machine defined in e_StatusBits
//!
//!  \return none
//!
//*****************************************************************************
void Network_IF_UnsetMCUMachineState(char cStat) {
	CLR_STATUS_BIT(g_ulStatus, cStat);
}

//*****************************************************************************
//
//! itoa
//!
//!    @brief  Convert integer to ASCII in decimal base
//!
//!     @param  cNum is input integer number to convert
//!     @param  cString is output string
//!
//!     @return number of ASCII parameters
//!
//!
//
//*****************************************************************************
unsigned short itoa(char cNum, char *cString) {
	char* ptr;
	char uTemp = cNum;
	unsigned short length;

// value 0 is a special case
	if (cNum == 0) {
		length = 1;
		*cString = '0';

		return length;
	}

// Find out the length of the number, in decimal base
	length = 0;
	while (uTemp > 0) {
		uTemp /= 10;
		length++;
	}

// Do the actual formatting, right to left
	uTemp = cNum;
	ptr = cString + length;
	while (uTemp > 0) {
		--ptr;
		*ptr = pcDigits[uTemp % 10];
		uTemp /= 10;
	}

	return length;
}

//****************************************************************************
//
//!	\brief Read Force AP GPIO and Configure Mode - 1(Access Point Mode)
//!                                                  - 0 (Station Mode)
//!
//! \return	                	None
//
//****************************************************************************

void ReadDeviceConfiguration() {
	unsigned int uiGPIOPort;
	unsigned char pucGPIOPin;
	unsigned char ucPinValue;

//Read GPIO
	GPIO_IF_GetPortNPin(SH_GPIO_3, &uiGPIOPort, &pucGPIOPin);
	ucPinValue = GPIO_IF_Get(SH_GPIO_3, uiGPIOPort, pucGPIOPin);

//If Connected to VCC, Mode is AP
	if (ucPinValue == 1) {
		//AP Mode
		g_uiDeviceModeConfig = ROLE_AP;
	} else {
		//STA Mode
		g_uiDeviceModeConfig = ROLE_STA;
	}
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

