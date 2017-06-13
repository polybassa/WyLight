/*
   Copyright (C) 2012 - 2017 Nils Weiss, Patrick Bruenn.

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

#ifndef _PLATFORM_ESP_H_
#define _PLATFORM_ESP_H_

/*
 * Don't include this file directly. It is included
 * by the generic platform.h. Include that one instead.
 */

#include <stdint.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>
#include "semphr.h"
#include "lwip/sockets.h"

typedef int8_t bit;
typedef uint8_t uns8;
typedef uint16_t uns16;

//global variables
extern uns8 g_UpdateFade;
extern jmp_buf g_ResetEnvironment;

#define bank1
#define bank2
#define bank3
#define bank5
#define bank6
#define bank7
#define bank10
#define clearRAM(x)

#define InitFactoryRestoreWLAN(x)
#define InitFET(x)

#define Platform_sleep_ms(tmms) \
    do { \
        vTaskDelay((tmms) / portTICK_PERIOD_MS); \
        taskYIELD(); \
    } while (false)

#define Platform_Main(x) \
    void run_main(void* unused)
#define Platform_MainLoopSleep(x) \
    taskYIELD();

#define Platform_IOInit(x)
#define Platform_OsciInit(x)

#define softReset(x)
#define softResetJumpDestination(x)

#define Platform_AllowInterrupts(x)
#define Platform_EnableAllInterrupts()
#define Platform_DisableAllInterrupts()

#define Platform_Thread \
    TaskHandle_t

#define Platform_ThreadFunc \
    void

#define Platform_CreateThread(routine, stack_size, args, prio, handle) \
    xTaskCreate(routine, #routine, stack_size, args, prio, handle)

#define Platform_Mutex \
    SemaphoreHandle_t

#define Platform_MutexInit(x) \
    xSemaphoreCreateMutex()

#define Platform_MutexLock(x) \
    xSemaphoreTake(*x, 10)

#define Platform_MutexUnlock(x) \
    xSemaphoreGive(*x)

#define Platform_FatalError(x) do { \
        printf("%s(): FATAL ERROR -> exiting...\n", __FUNCTION__); \
        Platform_sleep_ms(10000); \
} while (true)

#define Platform_CheckInputs(x)

/*** This Function will Disable the Autostart to the Bootloader.
 * At Startup, Bootloader checks the last EEPROM-Cell. If there is
 * 0x01 in the EEPROM-Cell, the Bootloader will go directly to the
 * Application otherwise the Bootloader stays in Bootloader-Mode.
 */
#define Platform_DisableBootloaderAutostart(x)
#define Platform_EnableBootloaderAutostart(x)

#endif /* #ifndef _PLATFORM_ESP_H_ */
