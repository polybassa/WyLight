/**
   Copyright (C) 2012 - 2016 Nils Weiss, Patrick Bruenn.

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

#include "ScriptCtrl.h"
#include "timer.h"
#include "trace.h"

extern uns8 g_UpdateLedStrip;
extern uns8 g_UpdateLed;

static Platform_ThreadFunc timer1_interrupt(void* unused)
{
    for ( ; ; ) {
        Platform_sleep_ms(10);
        ++g_UpdateLedStrip;
        ScriptCtrl_CheckAndDecrementWaitValue();
    }
}

static Platform_ThreadFunc timer5_interrupt(void* unused)
{
    for ( ; ; ) {
        Platform_sleep_ms(5);
        ++g_UpdateLed;
        ScriptCtrl_DecrementWaitValue();
    }
}

uns16 Timer_PrintCycletime(uns16* pArray, const uns16 arraySize)
{
    //TODO implemtent
    return arraySize;
}

void Timer_Init()
{
    static Platform_Thread timer1_10ms;
    static Platform_Thread timer5_5ms;

    Platform_CreateThread(&timer1_interrupt, 256, NULL, 2, &timer1_10ms);
    Platform_CreateThread(&timer5_interrupt, 256, NULL, 2, &timer5_5ms);
}
