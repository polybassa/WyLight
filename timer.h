/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
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

void Timer_Init();

/*To Do: Funktion bekommt einen Wert, anhand diese wertes wird die geschwindigkeit des
Timers festgelegt. Wert kommt aus der berechnung einer anderen Funktion (fade oder run)*/
void Timer_SetForFade(char value);


/**
** Function start the internal Stopwatch. It's realised with the Timer3 Modul.
** The Range of the Stopwatch is from 0.5µSec to 32.7mSec
**/
void Timer_StartStopwatch(void);

/**
** Function terminates the Stopwatch and print out the measured Time over UART
**/
void Timer_StopStopwatch(uns16 *pTmax);

void Timer_PrintCycletime(uns16 *pTmax);

#ifdef __CC8E__
#define Timer1Interrupt(x) TMR1IF = 0;
#define Timer2Interrupt(x) TMR2IF = 0;
#define Timer4Interrupt(x) TMR4IF = 0;
#define Timer4InterruptLock(x) TMR4IE = 0;
#define Timer4InterruptUnlock(x) TMR4IE = 1;
#else
#define Timer1Interrupt(x)
#define Timer2Interrupt(x)
#define Timer4Interrupt(x)
#define Timer4InterruptLock(x)
#define Timer4InterruptUnlock(x)
#endif
#endif
