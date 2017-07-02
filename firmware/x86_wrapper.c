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

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "platform.h"
#include "ledstrip.h"
#include "RingBuf.h"
#include "ScriptCtrl.h"
#include "timer.h"
#include "Version.h"

struct RingBuffer g_TraceBuf;
extern uns8 g_UpdateLed;

void I2C_Init(){}
void I2C_Write(const uns8 slaveaddr, const uns8 dataaddr, const uns8 data){}
uns8 I2C_Read(const uns8 slaveaddr, const uns8 readaddr){return 0; }
uns8 I2C_DetectSlave(const uns8 slaveaddr){return 0; }

void* timer1_interrupt(void* unused)
{
    for ( ; ; ) {
        usleep(1000);
        Ledstrip_UpdateLed();
    }
}
void* timer4_interrupt(void* unused)
{
    for ( ; ; ) {
        usleep(1000);
        g_UpdateLed++;
        ScriptCtrl_DecrementWaitValue();
    }
}

void Rtc_Init() {}
void Rtc_Ctl(enum RTC_request req, struct rtc_time* pRtcTime) {}

uns16 Timer_PrintCycletime(uns16* pArray, const uns16 arraySize)
{
    return arraySize;
}

void Platform_ExtraInit()
{}
