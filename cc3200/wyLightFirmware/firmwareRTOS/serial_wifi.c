//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
//
// All rights reserved. Property of Texas Instruments Incorporated.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
// The program may not be used without the written permission of
// Texas Instruments Incorporated or against the terms and conditions
// stipulated in the agreement under which this program has been supplied,
// and under no circumstances can it be used with non-TI connectivity device.
//
//*****************************************************************************

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "utils.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

//Common interface includes
#include "network_if.h"

//Application Includes
#define extern
#include "serial_wifi.h"
#undef extern

//*****************************************************************************
//
//! \addtogroup serial_wifi
//! @{
//
//*****************************************************************************

//
// GLOBAL VARIABLES -- Start
//
extern unsigned long g_ulStatus;
//
// GLOBAL VARIABLES -- End
//

#define CLR_STATUS_BIT_ALL(status_variable)  (status_variable = 0)

//****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//****************************************************************************
void Main_Task(void *pvParameters);
static void ConfigureSimpleLink(unsigned int uiMode);
static long WlanConnect();

extern void InitializeAppVariables();

//*****************************************************************************
//
//! Interpreter_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the Serial-WiFi functionality
//
//*****************************************************************************

void Main_Task(void *pvParameters) {

	SlrxFilterPrePreparedFiltersMask_t FilterPrePreparedFiltersMask;
	long lRetVal = -1;

	//powering on the CC3200 NWP
	ConfigureSimpleLink(ROLE_STA);

	//remove all filters
	memset(FilterPrePreparedFiltersMask, 0, sizeof(FilterPrePreparedFiltersMask));
	FilterPrePreparedFiltersMask[0] = 0x00;

	WlanConnect();

	while (1) {

	}
}

//****************************************************************************
//
//! \brief Connecting to a WLAN Accesspoint
//!
//!  This function connects to the required AP (SSID_NAME) with Security
//!  parameters specified in te form of macros at the top of this file
//!
//! \param  Status value
//!
//! \return  None
//!
//! \warning    If the WLAN connection fails or we don't aquire an IP
//!            address, It will be stuck in this function forever.
//
//****************************************************************************
static long WlanConnect() {
	SlSecParams_t secParams = { 0 };
	long lRetVal = 0;

	secParams.Key = SECURITY_KEY;
	secParams.KeyLen = strlen(SECURITY_KEY);
	secParams.Type = SECURITY_TYPE;

	lRetVal = sl_WlanConnect(SSID_NAME, strlen(SSID_NAME), 0, &secParams, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Wait for WLAN Event
	while ((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus))) {
		// wait till connects to an AP
		MAP_UtilsDelay(24000000); //waiting for 3 secs
	}

	return SUCCESS;

}


//*****************************************************************************
//
//! InitDevice
//!
//! @param  none
//!
//! @return none
//!
//! @brief  Initialise the device to default state without deleting the stored
//!			profiles.
//
//*****************************************************************************

static long InitDevice() {
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

	// Set connection policy to Auto + SmartConfig (Device's default connection policy)
	lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// If the device is not in station-mode, try putting it in staion-mode
	if (ROLE_STA != lMode) {
		if (ROLE_AP == lMode) {
			// If the device is in AP mode, we need to wait for this event before doing anything
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

		lRetVal = sl_Start(0, 0, 0);
		ASSERT_ON_ERROR(__LINE__, lRetVal);

		// Check if the device is in station again
		if (ROLE_STA != lRetVal) {
			// We don't want to proceed if the device is not coming up in station-mode
			return -1;
		}
	}

	//
	// Device in station-mode. Disconnect previous connection if any
	// The function returns 0 if 'Disconnected done', negative number if already disconnected
	// Wait for 'disconnection' event if 0 is returned, Ignore other return-codes
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
	// Number between 0-15, as dB offset from max power - 0 will set maximum power
	ucPower = 0;
	lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *) &ucPower);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Set PM policy to normal
	lRetVal = sl_WlanPolicySet(SL_POLICY_PM, SL_NORMAL_POLICY, NULL, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Unregister mDNS services

	lRetVal = sl_Stop(SL_STOP_TIMEOUT);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	InitializeAppVariables();

	return lRetVal; // Success
}

//*****************************************************************************
//
//! ConfigureSimpleLinkForSerialWifi
//!
//! @param  uiMode
//!
//! @return none
//!
//! @brief  Configuring the state of the CC3200 simplelink device for Serial Wifi
//
//*****************************************************************************
static void ConfigureSimpleLink(unsigned int uiMode) {
	long retVal = -1;
	// Reset CC3200 Network State Machine
	InitializeAppVariables();

	retVal = InitDevice();
	if (retVal < 0) {
		if (-1 == retVal)
		UART_PRINT("Failed to configure the device in its default state \n\r");

		LOOP_FOREVER(__LINE__);
	}

	UART_PRINT("Device is configured in default state \n\r");

	CLR_STATUS_BIT_ALL(g_ulStatus);

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
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
