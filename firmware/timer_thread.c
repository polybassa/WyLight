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

#include "timer.h"
#include "trace.h"

static void timer1_interrupt(void* unused)
{
    for ( ; ; ) {
        Platform_sleep_ms(100);
        Ledstrip_UpdateLed();
    }
}

static void timer4_interrupt(void* unused)
{
    for ( ; ; ) {
        Platform_sleep_ms(100);
        g_UpdateLed++;
        ScriptCtrl_DecrementWaitValue();
    }
}

void Timer_Init()
{
    xTaskCreate(&timer1_interrupt, "timer1_interrupt", 256, NULL, 3, NULL);
    xTaskCreate(&timer4_interrupt, "timer4_interrupt", 256, NULL, 2, NULL);
}
