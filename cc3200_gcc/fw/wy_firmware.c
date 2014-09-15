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
#include "CommandIO.h"
#include "RingBuf.h"
#include "ScriptCtrl.h"
#include "ledstrip.h"
//
// GLOBAL VARIABLES -- Start
//

static xSemaphoreHandle g_FirmwareCanAccessFileSystemSemaphore;
OsiSyncObj_t FirmwareCanAccessFileSystemSemaphore = &g_FirmwareCanAccessFileSystemSemaphore;

static xTaskHandle g_WyLightFirmwareTaskHandle;
OsiTaskHandle WyLightFirmwareTaskHandle = &g_WyLightFirmwareTaskHandle;

//
// GLOBAL VARIABLES -- End
//

void WyLightFirmware_TaskInit(void) {
	osi_SyncObjCreate(FirmwareCanAccessFileSystemSemaphore);
	RingBuf_Init(&g_RingBuf_Tx);
	RingBuf_Init(&g_RingBuf);

}

void WyLightFirmware_Task(void *pvParameters) {
	CommandIO_Init();
	ScriptCtrl_Init();
	for (;;) {
		CommandIO_GetCommands();
		ScriptCtrl_Run();
		if(gScriptBuf.waitValue) gScriptBuf.waitValue--;
		Ledstrip_DoFade();
		Ledstrip_UpdateLed();
		osi_Sleep(20);
	}

	//Ledstrip_UpdateLed(); /* every x ms */
	//ScriptCtrl_Run();
	//Ledstrip_DoFade(); /* every y ms */

	//WyLightFirmware_SendMessages();

}
