/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

 This file is part of WyLight.

 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

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
// Simplelink includes
#include "simplelink.h"

// driverlib includes
#include "hw_types.h"
#include "rom.h"
#include "rom_map.h"
#include "utils.h"
#include <stdio.h>

// common interface includes
#include "wy_bl_network_if.h"
#include "uart_if.h"

static unsigned long g_ulStatus = 0; /* SimpleLink Status */

void wifi_status_disconnected() {
	CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
	CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);
}

// Callback function to handle SimpleLink WLAN events
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent) {
	switch (pSlWlanEvent->Event) {
	case SL_WLAN_STA_CONNECTED_EVENT: // when device is in AP mode and any client connects to device cc3xxx
	case SL_WLAN_CONNECT_EVENT:
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);
		break;

	case SL_WLAN_STA_DISCONNECTED_EVENT: // when client disconnects from device (AP)
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
		/** fallthru */
	case SL_WLAN_DISCONNECT_EVENT:
		wifi_status_disconnected();
		break;

	case SL_WLAN_CONNECTION_FAILED_EVENT:
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION_FAILED);
		wifi_status_disconnected();
		break;

	default:
		UART_PRINT("[WLAN EVENT] Unexpected event \n\r");
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
	case SL_NETAPP_IPV4_ACQUIRED:
	case SL_NETAPP_IPV6_ACQUIRED:
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);
		UART_PRINT("[NETAPP EVENT] IP Acquired\r\n");
		break;

	case SL_NETAPP_IP_LEASED:
		SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
		break;

	case SL_NETAPP_IP_RELEASED:
		CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_LEASED);
		break;

	case SL_NETAPP_SOCKET_TX_FAILED:
		UART_PRINT("[NETAPP EVENT] Socket Error # %d \n\r", pNetAppEvent->EventData.sd);
		break;

	default:
		UART_PRINT("[NETAPP EVENT] Unexpected event \n\r");
		break;
	}
}

// Callback function to handle general SimpleLink events

void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent) {
	// Most of the general errors are not FATAL are are to be handled
	// appropriately by the application
#ifdef DEBUG
	const sl_DeviceReport report = pDevEvent->EventData.deviceEvent;
	UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n", report.status, report.sender);
#endif
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
	UART_PRINT("[SOCK EVENT] Eventtype: %d\r\n", pSock->Event);
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
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pSlHttpServerEvent,
		SlHttpServerResponse_t *pSlHttpServerResponse) {
	UART_PRINT("[HTTP EVENT]\r\n");

}

//*****************************************************************************
//! \brief This function puts the device in its default state. It:
//!           - Set the mode to AP
//!           - Configures connection policy to Auto and AutoSmartConfig
//!           - Disables Scan policy
//!           - Sets Tx power to maximum
//!           - Sets power policy to normal
//!           - TBD - Unregister mDNS services
//!
//! \param   none
//! \return  On success, zero is returned. On error, negative is returned
//*****************************************************************************
static long ConfigureSimpleLink() {
	long retVal = ERROR;
	long mode = ERROR;
	CLR_STATUS_BIT_ALL(g_ulStatus);

	mode = sl_Start(NULL, NULL, NULL);
	ASSERT_ON_ERROR(__LINE__, mode);

	if (ROLE_AP == mode) {
		// If the device is in AP mode, we need to wait for this event
		// before doing anything
		while (!IS_IP_ACQUIRED(g_ulStatus)) {
			_SlNonOsMainLoopTask();
		}
	} else {

		// If the device is not in AP-mode, try putting it in AP-mode

		// Switch to STA role and restart
		retVal = sl_WlanSetMode(ROLE_AP);
		ASSERT_ON_ERROR(__LINE__, retVal);

		retVal = sl_Stop(SL_STOP_TIMEOUT);
		ASSERT_ON_ERROR(__LINE__, retVal);

		CLR_STATUS_BIT_ALL(g_ulStatus);

		retVal = sl_Start(NULL, NULL, NULL);
		ASSERT_ON_ERROR(__LINE__, retVal);

		// Check if the device is in station again
		if (ROLE_AP != retVal) {
			// We don't want to proceed if the device is not up in STA-mode
			return ERROR;
		} else {
			// If the device is in AP mode, we need to wait for this event
			// before doing anything
			while (!IS_IP_ACQUIRED(g_ulStatus)) {
				_SlNonOsMainLoopTask();
			}
		}
	}

	// Get the device's version-information
	SlVersionFull ver = { { 0 } };
	unsigned char configOpt = SL_DEVICE_GENERAL_VERSION;
	unsigned char configLen = sizeof(ver);
	retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (unsigned char *) (&ver));
	ASSERT_ON_ERROR(__LINE__, retVal);

	UART_PRINT("Host Driver Version: %s\n\r", SL_DRIVER_VERSION);
	UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r", ver.NwpVersion[0], ver.NwpVersion[1], ver.NwpVersion[2], ver.NwpVersion[3], ver.ChipFwAndPhyVersion.FwVersion[0], ver.ChipFwAndPhyVersion.FwVersion[1], ver.ChipFwAndPhyVersion.FwVersion[2], ver.ChipFwAndPhyVersion.FwVersion[3], ver.ChipFwAndPhyVersion.PhyVersion[0], ver.ChipFwAndPhyVersion.PhyVersion[1], ver.ChipFwAndPhyVersion.PhyVersion[2], ver.ChipFwAndPhyVersion.PhyVersion[3]);

	sl_WlanDisconnect();

	// get current Time
	SlDateTime_t dateTime;
	unsigned char option = SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME, length = sizeof(SlDateTime_t);
	retVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &option, &length, (unsigned char *) &dateTime);
	ASSERT_ON_ERROR(__LINE__, retVal);

	// compute random channel from current time
	unsigned char channel = (dateTime.sl_tm_sec % 13) + 1; // to avoid channel 0
	UART_PRINT("Accesspoint channel: %d\r\n", channel);

	retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_CHANNEL, 1, &channel);
	ASSERT_ON_ERROR(__LINE__, retVal);

	// Set Tx power level for station mode
	// Number between 0ERROR5, as dB offset from max power - 0 will set max power
	unsigned char power = 0;
	retVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID,
	WLAN_GENERAL_PARAM_OPT_AP_TX_POWER, sizeof(power), (unsigned char *) &power);
	ASSERT_ON_ERROR(__LINE__, retVal);

	// Set PM policy to normal
	retVal = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, retVal);
    
    // MAC Adress
    char mac[SL_MAC_ADDR_LEN];
    unsigned char macLen = SL_MAC_ADDR_LEN;
    sl_NetCfgGet(SL_MAC_ADDRESS_GET, NULL, &macLen, (unsigned char *)mac);

	const char ssid[] = "WyLightBootloaderAP-";
    char ssidMac[sizeof(ssid) + 4];
    sprintf(ssidMac, "%s%02x%02x", ssid, mac[SL_MAC_ADDR_LEN - 2], mac[SL_MAC_ADDR_LEN - 1]);
	retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, 0, strlen((const char *) ssidMac), (unsigned char *)ssidMac);
	ASSERT_ON_ERROR(__LINE__, retVal);

	unsigned char val = SL_SEC_TYPE_OPEN;
	retVal = sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SECURITY_TYPE, 1, (unsigned char *) &val);
	ASSERT_ON_ERROR(__LINE__, retVal);

	retVal = sl_Stop(SL_STOP_TIMEOUT);
	ASSERT_ON_ERROR(__LINE__, retVal);

	CLR_STATUS_BIT_ALL(g_ulStatus);
	return retVal; // Success
}

//****************************************************************************
//
//! Task function implements the ProvisioningAP functionality
//!
//! \param none
//!
//! This function
//!    1. Starts Device
//!    2. Switch to AP Mode
//!	   3. Sets SSID and Security Type
//!    4. Starts AP
//!
//! \return None.
//
//****************************************************************************
long Network_IF_StartSimpleLinkAsAP(void) {
	long retVal = ERROR;
	CLR_STATUS_BIT_ALL(g_ulStatus);
	retVal = sl_Start(NULL, NULL, NULL);
	if (retVal < 0) {
		UART_PRINT("Failed to start the device \n\r");
		LOOP_FOREVER(__LINE__);
	}

	if (retVal == ROLE_AP) {
		UART_PRINT("Device started as Bootloader AP \n\r");
		//Device in AP-Mode, Wait for initialization to complete
		while (!IS_IP_ACQUIRED(g_ulStatus)) {
			_SlNonOsMainLoopTask();
		}
	} else {
		return ERROR;
	}

	return SUCCESS;
}

//*****************************************************************************
//
//! Network_IF_InitDriver
//! The function initializes a CC3200 device and triggers it to start operation
//!
//! \return none
//
//*****************************************************************************
void Network_IF_InitDriver(void) {
	long retVal = ERROR;

	retVal = ConfigureSimpleLink();
	if (retVal < 0) {
		LOOP_FOREVER(__LINE__);
		//return;
	}
	UART_PRINT("Device is configured in default state \n\r");
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

	// Stop the simplelink host
	sl_Stop(SL_STOP_TIMEOUT);

	// Reset the state to uninitialized
	CLR_STATUS_BIT_ALL(g_ulStatus);
}
