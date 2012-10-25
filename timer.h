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
#include "wifly_cmd.h"

enum METHODE{
	eMAIN,				//00
	eLedstrip_DoFade,		//01
	eSET_FADE,			//02
	eSET_COLOR,			//03
	eSET_RUN,			//04
	eLedstrip_UpdateRun,		//05
	eLedstrip_UpdateLed,		//06
	eCommandstorage_GetCommands,	//07
	eScriptCtrl_Run,		//08
	ePlatform_CheckInputs,		//09
	eError_Throw,			//10
	eTIMER_WAIT,			//11
	enumSIZE //!!! MUST be the last element of the enum
} enumMethode;

struct CycleTimeBuffer{
	uns16 maxCycleTime[enumSIZE];
	uns16 tempCycleTime[enumSIZE];
};

extern bank3 struct CycleTimeBuffer g_CycleTimeBuffer;

void Timer_Init();
// a day has 86400 seconds, we are updating our counter in 2 sec steps
#define DATE_TIMER_DAY (unsigned short)43200
#define NUM_DATE_EVENTS 1

struct date_event {
	unsigned short wakeup;
	struct led_cmd cmd;
};

unsigned char date_timer_add_event(struct cmd_add_color* pCmd);
void date_timer_callback(void);
void date_timer_do_events(void);

/**
** Function start the internal Stopwatch. It's realised with the Timer3 Modul.
** The Range of the Stopwatch is from 0.5µSec to 32.7mSec
**/
void Timer_StartStopwatch(enum METHODE destMethode);

/**
** Function terminates the Stopwatch and print out the measured Time over UART
**/
void Timer_StopStopwatch(enum METHODE destMethode);

void Timer_PrintCycletime(void);

#ifdef __CC8E__
#define Timer1Interrupt(x) TMR1IF = 0;
#define Timer2Interrupt(x) TMR2IF = 0;
#define Timer3Interrupt(x) TMR3IF = 0;
#define Timer4Interrupt(x) TMR4IF = 0;
#define Timer5Interrupt(x) TMR5IF = 0; TMR5H = 0x63; TMR5L = 0xC0;

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
