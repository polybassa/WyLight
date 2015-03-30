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

// Simplelink includes
#include "simplelink.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Application Includes
#include "wy_firmware.h"
#include "CommandIO.h"
#include "RingBuf.h"
#include "ScriptCtrl.h"
#include "ledstrip.h"
#include "trace.h"
//
// GLOBAL VARIABLES -- Start
//

xSemaphoreHandle g_FirmwareCanAccessFileSystemSemaphore;
xSemaphoreHandle g_NewDataAvailableSemaphore;
xSemaphoreHandle g_AccessScriptBufferMutex;

xTaskHandle g_WyLightFirmwareTaskHandle;
xTaskHandle g_WyLightGetCommandsTaskHandle;

//
// GLOBAL VARIABLES -- End
//

void WyLightFirmware_TaskInit(void)
{
    vSemaphoreCreateBinary(g_FirmwareCanAccessFileSystemSemaphore);
    xSemaphoreTake(g_FirmwareCanAccessFileSystemSemaphore, 0);

    vSemaphoreCreateBinary(g_NewDataAvailableSemaphore);
    xSemaphoreTake(g_NewDataAvailableSemaphore, 0);

    g_AccessScriptBufferMutex = xSemaphoreCreateMutex();

    RingBuf_Init(&g_RingBuf_Tx);
    RingBuf_Init(&g_RingBuf);
    Ledstrip_Init();
}

void WyLightFirmware_Task(void* pvParameters)
{
    CommandIO_Init();
    ScriptCtrl_Init();
    for ( ; ; ) {
        if (xSemaphoreTake(g_AccessScriptBufferMutex, 0)) {
            ScriptCtrl_Run();
            xSemaphoreGive(g_AccessScriptBufferMutex);
        }
        ScriptCtrl_DecrementWaitValue();
        Ledstrip_DoFade();
        Ledstrip_UpdateLed();
        vTaskDelay(10 / portTICK_RATE_MS);
    }
}

void WyLightGetCommands_Task(void* pvParameters)
{
    for ( ; ; ) {
        xSemaphoreTake(g_NewDataAvailableSemaphore, portMAX_DELAY);
        xSemaphoreTake(g_AccessScriptBufferMutex, portMAX_DELAY);
        CommandIO_GetCommands();
        xSemaphoreGive(g_AccessScriptBufferMutex);
    }
}
