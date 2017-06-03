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
#include <arpa/inet.h>
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
#define Platform_EnableBootloaderAutostart(x)
#define clearRAM(x)
#define Platform_AllowInterrupts(x)
#define Platform_EnableAllInterrupts()
#define Platform_DisableAllInterrupts()
#define Platform_CheckInputs(x)
#define Platform_DisableBootloaderAutostart(x)
#define Platform_Main(x) \
    jmp_buf g_ResetEnvironment; \
    int main(x)
#define Platform_MainLoopSleep(x) usleep(10)
#define InitFactoryRestoreWLAN(x)
#define InitFET(x)
void Platform_ExtraInit(void);
#define Platform_IOInit(x)
#define Platform_OsciInit(x)
#define softReset(x) longjmp(g_ResetEnvironment, 1)
#define softResetJumpDestination(x) setjmp(g_ResetEnvironment)

#endif /* #ifndef _PLATFORM_LINUX_H_ */
