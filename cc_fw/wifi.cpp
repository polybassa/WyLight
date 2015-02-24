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
#include "SimplelinkDriver.h"
#include "wy_firmware.h"
#include "SimplelinkCustomer.h"
#include "firmware/trace.h"
#include "wifi.h"
#include "gpio_if.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static xTaskHandle g_WlanSupportTaskHandle;
OsiTaskHandle* WlanSupportTaskHandle = &g_WlanSupportTaskHandle;

void WlanSupport_Task(void* pvParameters)
{
    long retRes = ERROR;

    retRes = (long)GPIO_IF_ReadDeviceConfigurationPin();
    while (true) {
        if (retRes == ROLE_STA) {
            Trace(ZONE_INFO, "Attempting to auto connect to AP\r\n");

            SimplelinkDriver simplelinkChip(false);
            if (simplelinkChip) {
                osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
                SimplelinkCustomer::provideService();

                simplelinkChip.waitUntilConnectionLost();

                osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_WAIT_FOREVER);
                SimplelinkCustomer::stopService();
            }
            Trace(ZONE_INFO, "Not connected to AP\r\n");
        }
        {
            SimplelinkDriver simplelinkChip(true);
            if (simplelinkChip) {
                osi_SyncObjSignal(FirmwareCanAccessFileSystemSemaphore);
                SimplelinkCustomer::provideService();

                simplelinkChip.waitForNewProvisioningData();

                osi_SyncObjWait(FirmwareCanAccessFileSystemSemaphore, OSI_WAIT_FOREVER);
                SimplelinkCustomer::stopService();
            }
        }
        retRes = ROLE_STA;
    }
}