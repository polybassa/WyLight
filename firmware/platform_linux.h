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

#ifndef _PLATFORM_LINUX_H_
#define _PLATFORM_LINUX_H_

/*
 * Don't include this file directly. It is included
 * by the generic platform.h. Include that one instead.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <setjmp.h>
#include <string.h>
#include <unistd.h>

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
    usleep(1000 * (tmms))

#define Platform_Main(x) \
    jmp_buf g_ResetEnvironment; \
    int main(x)
#define Platform_MainLoopSleep(x) usleep(10)
#define Platform_IOInit(x)
#define Platform_OsciInit(x)

#define softReset(x) longjmp(g_ResetEnvironment, 1)
#define softResetJumpDestination(x) setjmp(g_ResetEnvironment)

#define Platform_AllowInterrupts(x)
#define Platform_EnableAllInterrupts()
#define Platform_DisableAllInterrupts()

#define Platform_Thread \
    pthread_t

#define Platform_ThreadFunc \
    void*

#define Platform_CreateThread(routine, stack_size, args, prio, handle) \
    pthread_create(handle, nullptr, routine, args)

#define Platform_Mutex \
    pthread_mutex_t

#define Platform_MutexInit(x) \
    PTHREAD_MUTEX_INITIALIZER

#define Platform_MutexLock(x) \
    pthread_mutex_lock(x)

#define Platform_MutexUnlock(x) \
    pthread_mutex_unlock(x)

#define Platform_FatalError(x) do { \
        printf("%s(): FATAL ERROR -> exiting...\n", __func__); \
        exit(-666); \
} while (false)

#define Platform_CheckInputs(x)

/*** This Function will Disable the Autostart to the Bootloader.
 * At Startup, Bootloader checks the last EEPROM-Cell. If there is
 * 0x01 in the EEPROM-Cell, the Bootloader will go directly to the
 * Application otherwise the Bootloader stays in Bootloader-Mode.
 */
#define Platform_DisableBootloaderAutostart(x)
#define Platform_EnableBootloaderAutostart(x)

#endif /* #ifndef _PLATFORM_LINUX_H_ */
