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
#include "network_if.h"
#include "uart_if.h"
#include "timer_if.h"

//
// GLOBAL VARIABLES -- Start
//
unsigned long g_ulStatus = 0; /* SimpleLink Status */
unsigned long g_ulStaIp = 0; /* Station IP address */
unsigned long g_ulGatewayIP = 0; /* Network Gateway IP address */
unsigned char g_ucConnectionSSID[SSID_LEN_MAX + 1]; /* Connection SSID */
unsigned char g_ucConnectionBSSID[BSSID_LEN_MAX]; /* Connection BSSID */
unsigned short g_usConnectIndex; /* Connection time delay index */
const char pcDigits[] = "0123456789"; /* variable used by itoa function */
//
// GLOBAL VARIABLES -- End
//

/* Application specific status/error codes */
typedef enum{
     /* Choosing this number to avoid overlap w/ host-driver's error codes */
    LAN_CONNECTION_FAILED = -0x7D0,
    CLIENT_CONNECTION_FAILED = LAN_CONNECTION_FAILED - 1,
    DEVICE_NOT_IN_STATION_MODE = CLIENT_CONNECTION_FAILED - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;


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
#endif //USE_FREERTOS//*****************************************************************************// SimpleLink Asynchronous Event Handlers -- Start//*****************************************************************************//*****************************************************************************//
//! \brief The Function Handles WLAN Events
//!
//! \param[in]  pSlWlanEvent - Pointer to WLAN Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
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

		UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s , BSSID: "
				"%x:%x:%x:%x:%x:%x\n\r", g_ucConnectionSSID, g_ucConnectionBSSID[0], g_ucConnectionBSSID[1], g_ucConnectionBSSID[2], g_ucConnectionBSSID[3],
				g_ucConnectionBSSID[4], g_ucConnectionBSSID[5]);
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
//! This function handles socket events indication
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
	switch (pSock->Event) {
	case SL_NETAPP_SOCKET_TX_FAILED:
		switch (pSock->EventData.status) {
		case SL_ECLOSE:
			UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
			 "failed to transmit all queued packets\n\n",
			 pSock->EventData.sd);
			break;
		default:
			UART_PRINT("[SOCK ERROR] - TX FAILED  :  socket %d , reason "
			 "(%d) \n\n",
			 pSock->EventData.sd, pSock->EventData.status);
			break;
		}
		break;

	default:
		UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",pSock->Event);
		break;
	}
}

//*****************************************************************************
//
//! This function gets triggered when HTTP Server receives Application
//! defined GET and POST HTTP Tokens.
//!
//! \param pHttpServerEvent Pointer indicating http server event
//! \param pHttpServerResponse Pointer indicating http server response
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent, SlHttpServerResponse_t *pSlHttpServerResponse) {
	// Unused in this application
}

//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- End
//*****************************************************************************

//****************************************************************************
//
//!    \brief This function initializes the application variables
//!
//!    \param[in]  None
//!
//!    \return     0 on success, negative error-code on error
//
//****************************************************************************
void InitializeAppVariables() {
	g_ulStatus = 0;
	g_ulStaIp = 0;
	g_ulGatewayIP = 0;
	memset(g_ucConnectionSSID, 0, sizeof(g_ucConnectionSSID));
	memset(g_ucConnectionBSSID, 0, sizeof(g_ucConnectionBSSID));
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

long ConfigureSimpleLinkToDefaultState() {
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

	// Set connection policy to Auto + SmartConfig (Device's default connection
	// policy)
	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Remove all profiles
	lRetVal = sl_WlanProfileDel(0xFF);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

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
	long retVal = -1;
	// Reset CC3200 Network State Machine
	InitializeAppVariables();

	//
	// Following function configure the device to default state by cleaning
	// the persistent settings stored in NVMEM (viz. connection profiles &
	// policies, power policy etc)
	//
	// Applications may choose to skip this step if the developer is sure
	// that the device is in its default state at start of application
	//
	// Note that all profiles and persistent settings that were done on the
	// device will be lost
	//
	retVal = ConfigureSimpleLinkToDefaultState();
	if (retVal < 0) {
		if (DEVICE_NOT_IN_STATION_MODE == retVal)
		UART_PRINT("Failed to configure the device in its default state \n\r");

		LOOP_FOREVER(__LINE__);
	}

	UART_PRINT("Device is configured in default state \n\r");

	//
	// Assumption is that the device is configured in station mode already
	// and it is in its default state
	//

	retVal = sl_Start(NULL, NULL, NULL);

	if (retVal < 0 || retVal != uiMode) {
		UART_PRINT("Failed to start the device \n\r");
		LOOP_FOREVER(__LINE__);
	}

	UART_PRINT("Started SimpleLink Device: STA Mode\n\r");

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

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

