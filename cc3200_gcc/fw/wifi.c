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

//Driverlib includes
#include "rom_map.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"

//Common interface includes
#include "wy_network_if.h"

#include "wifi.h"

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
void WlanSupport_Task(void *pvParameters) {

	SlrxFilterPrePreparedFiltersMask_t FilterPrePreparedFiltersMask;
	long lRetVal = -1;

	//powering on the CC3200 NWP
	Network_IF_InitDriver(ROLE_STA);

	//remove all filters
	memset(FilterPrePreparedFiltersMask, 0, sizeof(FilterPrePreparedFiltersMask));
	FilterPrePreparedFiltersMask[0] = 0x00;

	//lRetVal = WlanConnect();
	//TODO: make something with this return Value

	while (1) {
		osi_Sleep(500);
		Network_IF_CheckForNewProfile();
	}
}
