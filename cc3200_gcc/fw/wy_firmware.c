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

#include <stdint.h>
#include "hw_types.h"

// Simplelink includes
#include "simplelink.h"

//Free_rtos/ti-rtos includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Application Includes
#include "wy_firmware.h"
//
// GLOBAL VARIABLES -- Start
//

static xTaskHandle g_WyLightFirmwareTaskHandle;

OsiTaskHandle WyLightFirmwareTaskHandle = &g_WyLightFirmwareTaskHandle;

//
// GLOBAL VARIABLES -- End
//

void WyLightFirmware_TaskInit(void) {
	
}

void WyLightFirmware_Task(void *pvParameters) {
	
}
