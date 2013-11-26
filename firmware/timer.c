/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

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

struct CycleTimeBuffer g_CycleTimeBuffer;
enum CYCLETIME_METHODE enumMethode;

void Timer_Init()
{
#ifdef __CC8E__
	/*
	 * T1 Interrupt every 10 Millisecounds if clock is 64MHz
	 * Calculation
	 * 64000000 Hz / 4 / 8 / 65536
	 * T1 Interrupt occures with a frequency of 30 Hz.
	 * This is used to update the ledstrip with the current colorvalue
	 */
	T1CON = 0b00110111;
	TMR1IE = TRUE;

	/*
	 * T5 Interrupt every 5 Millisecounds if clock is 64MHz
	 * Calculation
	 * 64000000 Hz / 4 / 8 / 5000
	 */
	T5CON = 0b00110111;
	TMR5IE = TRUE;
	TMR5H = 0xEC;
	TMR5L = 0x78;
	/*
	** T4 Interrupt every 4 Millisecound if clock is 64MHz
	** Calculation
	** 64000000 Hz / 4 / 16 / 250 / 16
	*/
	T4CON = 0b01111111;
	TMR4IE = FALSE;
	PR4 = 250;

	/*
	** T2 Interrupt every 0.5 Millisecound if clock is 64MHz
	** Calculation
	** 64000000 Hz / 4 / 16 / 75 / 10
	*/
	T2CON = 0b01001111;
	TMR2ON = 0;
	TMR2IE = 0;
	PR2 = 75;

	/*
	** T3 Modul count with a frequency of 2MHz
	** T3 is used as PerformanceCounter
	** Calculation:
	** 64MHz / 4 / 8
	*/
	T3CON = 0b00110110;
	TMR3ON = 1;
#endif /* #ifdef __CC8E__ */
}

#ifdef DEBUG
void Timer_StartStopwatch(const enum CYCLETIME_METHODE destMethode)
{
	uns16 tempTime;

	Platform_ReadPerformanceCounter(tempTime);

	g_CycleTimeBuffer.tempCycleTime[destMethode] = tempTime;
}

void Timer_StopStopwatch(const enum CYCLETIME_METHODE destMethode)
{
	uns16 tempTime,temp16;

	Platform_ReadPerformanceCounter(tempTime);

	if(g_CycleTimeBuffer.tempCycleTime[destMethode] < tempTime) {
		tempTime = tempTime - g_CycleTimeBuffer.tempCycleTime[destMethode];
	} else {
		temp16 = 0xffff - g_CycleTimeBuffer.tempCycleTime[destMethode];
		tempTime += temp16;
	}

	if(tempTime > g_CycleTimeBuffer.maxCycleTime[destMethode]) {
		g_CycleTimeBuffer.maxCycleTime[destMethode] = tempTime;
	}
	g_CycleTimeBuffer.tempCycleTime[destMethode] = 0;
}

uns8 Timer_PrintCycletime(uns16 *pArray, const uns16 arraySize)
{
	uns16 i, temp16;
	for(i = 0; i < CYCLETIME_METHODE_ENUM_SIZE && i < arraySize; i++) {
		temp16 = g_CycleTimeBuffer.maxCycleTime[i];
		temp16 = htons(temp16);
		*pArray = temp16;
		pArray++;
		g_CycleTimeBuffer.maxCycleTime[i] = 0;
	}
	return i + i;
}
#else

void Timer_StartStopwatch(const enum CYCLETIME_METHODE destMethode)
{}

void Timer_StopStopwatch(const enum CYCLETIME_METHODE destMethode)
{}

uns8 Timer_PrintCycletime(uns16 *pArray, const uns16 arraySize)
{
	return 0;
}

#endif /*DEBUG*/