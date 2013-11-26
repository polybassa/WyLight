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

#ifndef _TIMER_H_
#define _TIMER_H_
#include "platform.h"

enum CYCLETIME_METHODE {
	eMAIN,                          //00
	eLedstrip_DoFade,               //01
	eSET_FADE,                      //02
	eSET_COLOR,                     //03
	e_EMPTY_METHODE_0,                      //04
	e_EMPTY_METHODE_1,              //05
	eLedstrip_UpdateLed,            //06
	eCommandIO_GetCommands, //07
	eScriptCtrl_Run,                //08
	ePlatform_CheckInputs,          //09
	eError_Throw,                   //10
	eTIMER_WAIT,                    //11
	CYCLETIME_METHODE_ENUM_SIZE //!!! MUST be the last element of the enum
};

extern enum CYCLETIME_METHODE enumMethode;

struct CycleTimeBuffer {
	uns16 maxCycleTime[CYCLETIME_METHODE_ENUM_SIZE];
	uns16 tempCycleTime[CYCLETIME_METHODE_ENUM_SIZE];
};

extern struct CycleTimeBuffer g_CycleTimeBuffer;

void Timer_Init();

/**
** Function start the internal Stopwatch. It's realised with the Timer3 Modul.
** The Range of the Stopwatch is from 0.5µSec to 32.7mSec
**/
void Timer_StartStopwatch(const enum CYCLETIME_METHODE destMethode);

/**
** Function terminates the Stopwatch and print out the measured Time over UART
**/
void Timer_StopStopwatch(const enum CYCLETIME_METHODE destMethode);

uns8 Timer_PrintCycletime(uns16 *pArray, const uns16 arraySize);

#ifdef __CC8E__
#define Timer1Interrupt(x) TMR1IF = 0;
#define Timer2Interrupt(x) TMR2IF = 0;
#define Timer3Interrupt(x) TMR3IF = 0;
#define Timer4Interrupt(x) TMR4IF = 0;
#define Timer5Interrupt(x) TMR5IF = 0; TMR5H = 0xEC; TMR5L = 0x78;

#define Timer1Enable(x) TMR1ON = 1;
#define Timer1Disable(x) TMR1ON = 0;

#define Timer4Enable(x) TMR4ON = 1;
#define Timer4Disable(x) TMR4ON = 0;

#define Timer4InterruptLock(x) TMR4IE = 0;
#define Timer4InterruptUnlock(x) TMR4IE = 1;

#define Timer5InterruptLock(x) TMR5IE = 0;
#define Timer5InterruptUnlock(x) TMR5IE = 1;

#else
#define Timer1Interrupt(x)
#define Timer2Interrupt(x)
#define Timer3Interrupt(x)
#define Timer4Interrupt(x)
#define Timer5Interrupt(x)
#define Timer1Enable(x)
#define Timer1Disable(x)
#define Timer4Enable(x)
#define Timer4Disable(x)
#define Timer4InterruptLock(x)
#define Timer4InterruptUnlock(x)
#define Timer5InterruptLock(x)
#define Timer5InterruptUnlock(x)
#endif
#endif
