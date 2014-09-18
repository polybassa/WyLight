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

static xSemaphoreHandle g_NewDataAvailableSemaphore;
OsiSyncObj_t NewDataAvailableSemaphore = &g_NewDataAvailableSemaphore;

static xSemaphoreHandle g_AccessScriptBufferMutex;

static xTaskHandle g_WyLightFirmwareTaskHandle;
static xTaskHandle g_WyLightGetCommandsTaskHandle;

OsiTaskHandle WyLightGetCommandsTaskHandle = &g_WyLightGetCommandsTaskHandle;
OsiTaskHandle WyLightFirmwareTaskHandle = &g_WyLightFirmwareTaskHandle;

//
// GLOBAL VARIABLES -- End
//

void WyLightFirmware_TaskInit(void) {
	osi_SyncObjCreate(FirmwareCanAccessFileSystemSemaphore);
	osi_SyncObjCreate(NewDataAvailableSemaphore);
	osi_LockObjCreate(&g_AccessScriptBufferMutex);
	RingBuf_Init(&g_RingBuf_Tx);
	RingBuf_Init(&g_RingBuf);
}

void WyLightFirmware_Task(void *pvParameters) {
	CommandIO_Init();
	ScriptCtrl_Init();
	for (;;) {
		if (OSI_OK == osi_LockObjLock(&g_AccessScriptBufferMutex,OSI_NO_WAIT)){
			ScriptCtrl_Run();
			osi_LockObjUnlock(&g_AccessScriptBufferMutex);
		}
		ScriptCtrl_DecrementWaitValue();
		Ledstrip_DoFade();
		Ledstrip_UpdateLed();
		osi_Sleep(10);
	}
}

void WyLightGetCommands_Task(void *pvParameters) {
	for (;;) {
		osi_SyncObjWait(NewDataAvailableSemaphore, OSI_WAIT_FOREVER);
		osi_LockObjLock(&g_AccessScriptBufferMutex, OSI_WAIT_FOREVER);
		CommandIO_GetCommands();
		osi_LockObjUnlock(&g_AccessScriptBufferMutex);
	}
}
