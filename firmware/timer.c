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

void Timer_Init()
{
#ifdef __CC8E__
    /*
     * T1 Interrupt every 10 Millisecounds if clock is 64MHz
     * Calculation
     * 64000000 Hz / 4 / 4 / 65536
     * T1 Interrupt occures with a frequency of 60 Hz.
     * This is used to update the ledstrip with the current colorvalue
     */
    T1CON = 0x27;
    TMR1IE = TRUE;

    /*
     * T5 Interrupt every 5 Millisecounds if clock is 64MHz
     * Calculation
     * 64000000 Hz / 4 / 8 / 5000
     */
    T5CON = 0x37;
    TMR5IE = TRUE;
    TMR5H = 0xEC;
    TMR5L = 0x78;
    /*
    ** T4 Interrupt every 4 Millisecound if clock is 64MHz
    ** Calculation
    ** 64000000 Hz / 4 / 16 / 250 / 16
    */
    T4CON = 0x7F;
    TMR4IE = FALSE;
    PR4 = 250;

    /*
    ** T2 Interrupt every 0.5 Millisecound if clock is 64MHz
    ** Calculation
    ** 64000000 Hz / 4 / 16 / 75 / 10
    */
    T2CON = 0x4f;
    TMR2ON = 0;
    TMR2IE = 0;
    PR2 = 75;

    /*
    ** T3 Modul count with a frequency of 2MHz
    ** T3 is used as PerformanceCounter
    ** Calculation:
    ** 64MHz / 4 / 8
    */
    T3CON = 0x36;
    TMR3ON = 1;
#endif /* #ifdef __CC8E__ */
}

#if defined(DEBUG) && defined(__CC8E__)
static struct CycleTimeBuffer g_CycleTimeBuffer;
static enum CYCLETIME_METHODE enumMethode;

#ifndef __SDCC_pic16
#define Platform_ReadPerformanceCounter(x) do { \
        x.low8 = TMR3L; \
        x.high8 = TMR3H; \
} while (0)
#else
#define Platform_ReadPerformanceCounter(x) do { \
        x = (((uint16_t)TMR3H) << 8) | TMR3L; \
} while (0)
#endif

void Timer_StartStopwatch(const enum CYCLETIME_METHODE destMethode)
{
    uns16 tempTime;

    Platform_ReadPerformanceCounter(tempTime);
    g_CycleTimeBuffer.tempCycleTime[destMethode] = tempTime;
}

void Timer_StopStopwatch(const enum CYCLETIME_METHODE destMethode)
{
    uns16 tempTime, temp16;

    Platform_ReadPerformanceCounter(tempTime);

    if (g_CycleTimeBuffer.tempCycleTime[destMethode] < tempTime) {
        tempTime = tempTime - g_CycleTimeBuffer.tempCycleTime[destMethode];
    } else {
        temp16 = 0xffff - g_CycleTimeBuffer.tempCycleTime[destMethode];
        tempTime += temp16;
    }

    if (tempTime > g_CycleTimeBuffer.maxCycleTime[destMethode])
        g_CycleTimeBuffer.maxCycleTime[destMethode] = tempTime;
    g_CycleTimeBuffer.tempCycleTime[destMethode] = 0;
}

uns16 Timer_PrintCycletime(uns16* pArray, uns16 arraySize)
{
    uns16 i, temp16, limit;

    arraySize /= sizeof(g_CycleTimeBuffer.maxCycleTime[0]);

    if (arraySize < CYCLETIME_METHODE_ENUM_SIZE)
        limit = arraySize;
    else
        limit = CYCLETIME_METHODE_ENUM_SIZE;

    for (i = 0; i < limit; i++) {
        temp16 = g_CycleTimeBuffer.maxCycleTime[i];
        temp16 = htons(temp16);
        *pArray = temp16;
        pArray++;
        g_CycleTimeBuffer.maxCycleTime[i] = 0;
    }
    return i + i;
}
#endif /* #if defined(DEBUG) && defined(__CC8E__) */
