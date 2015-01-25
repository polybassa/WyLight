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

#include "hw_types.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

//Common interface includes
#include "wy_network_if.h"
#include "wy_firmware.h"
#include "SimplelinkCustomer.h"

#include "wifi.h"
//
// GLOBAL VARIABLES -- Start
//

static xSemaphoreHandle g_WlanSupportProvisioningDataAddedSemaphore;
static xTaskHandle g_WlanSupportTaskHandle;

OsiSyncObj_t *WlanSupportProvisioningDataAddedSemaphore = &g_WlanSupportProvisioningDataAddedSemaphore;
OsiTaskHandle *WlanSupportTaskHandle = &g_WlanSupportTaskHandle;

//
// GLOBAL VARIABLES -- End
//

void WlanSupport_TaskInit(void) {
	osi_SyncObjCreate(WlanSupportProvisioningDataAddedSemaphore);
}

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
	long retRes = ERROR;

	retRes = (long) Network_IF_ReadDeviceConfigurationPin();
	while (true) {
		if (retRes == ROLE_STA) {
			UART_PRINT(ATTEMPTING_TO_CONNECT_TO_AP);

			if (SUCCESS == Network_IF_InitDriver(ROLE_STA)) {

				osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
				SimplelinkCustomer::provideService();

				while (IS_CONNECTED(g_WifiStatusInformation.SimpleLinkStatus)) {
					osi_Sleep(200);
				}

				osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_WAIT_FOREVER);
				SimplelinkCustomer::stopService();

				Network_IF_DeInitDriver();
			}
			UART_PRINT(NOT_CONNECTED_TO_AP);
		}

		if (SUCCESS == Network_IF_InitDriver(ROLE_AP)) {

			osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
			SimplelinkCustomer::provideService();

			do {
				osi_SyncObjWait(WlanSupportProvisioningDataAddedSemaphore, OSI_WAIT_FOREVER);
			} while (Network_IF_AddNewProfile() != SUCCESS);

			osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_WAIT_FOREVER);
			SimplelinkCustomer::stopService();

			Network_IF_DeInitDriver();
		}
		retRes = ROLE_STA;
	}
}
