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


//*****************************************************************************
//
//! \addtogroup wylight
//! @{
//
//*****************************************************************************

// Simplelink includes
#include "simplelink.h"

//Driverlib includes
#include "utils.h"
#include "rom_map.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#ifdef USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

//Common interface includes
#include "network_if.h"

#include "wifi.h"

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

#ifdef USE_FREERTOS
//*****************************************************************************
//
//! Main_Task
//!
//!  \param  pvParameters
//!
//!  \return none
//!
//!  \brief Task handler function to handle the WiFi functionality
//
//*****************************************************************************
void Main_Task(void *pvParameters) {

	SlrxFilterPrePreparedFiltersMask_t FilterPrePreparedFiltersMask;
	long lRetVal = -1;

	//powering on the CC3200 NWP
	Network_IF_InitDriver(ROLE_STA);

	//remove all filters
	memset(FilterPrePreparedFiltersMask, 0, sizeof(FilterPrePreparedFiltersMask));
	FilterPrePreparedFiltersMask[0] = 0x00;

	lRetVal = WlanConnect();
	//TODO: make something with this return Value

	while (1) {
		osi_Sleep(500);
	}
}
#endif

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
long WlanConnect() {
	SlSecParams_t secParams = { 0 };
	long lRetVal = 0;

	secParams.Key = SECURITY_KEY;
	secParams.KeyLen = strlen(SECURITY_KEY);
	secParams.Type = SECURITY_TYPE;

	lRetVal = sl_WlanConnect(SSID_NAME, strlen(SSID_NAME), 0, &secParams, 0);
	ASSERT_ON_ERROR(__LINE__, lRetVal);

	// Wait for WLAN Event
	while ((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus))) {
#ifdef USE_FREERTOS
		// wait till connects to an AP
		MAP_UtilsDelay(24000000); //waiting for 3 secs
#else
		_SlNonOsMainLoopTask();
#endif
	}

	return SUCCESS;

}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
