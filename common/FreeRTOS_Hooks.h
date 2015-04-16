/*
   Copyright (C) 2015 Nils Weiss, Patrick Bruenn.

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

#ifndef COMMON_FREERTOS_HOOKS_H_
#define COMMON_FREERTOS_HOOKS_H_

#ifdef USE_FREERTOS
#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif
//*****************************************************************************
// FreeRTOS User Hook Functions enabled in FreeRTOSConfig.h
//*****************************************************************************
void vAssertCalled(const char* pcFile, unsigned long ulLine)
{
    //Handle Assert here
    while (1) {}
}

void vApplicationIdleHook(void)
{
    //Handle Idle Hook for Profiling, Power Management etc
}

void vApplicationMallocFailedHook()
{
    //Handle Memory Allocation Errors
    while (1) {}
}

void vApplicationStackOverflowHook(xTaskHandle*     pxTask,
                                   signed portCHAR* pcTaskName)
{
    //Handle FreeRTOS Stack Overflow
    while (1) {}
}

#ifdef __cplusplus
}
#endif

#endif /* USE_FREERTOS */

#endif /* COMMON_FREERTOS_HOOKS_H_ */
