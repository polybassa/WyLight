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

//Nils Weiﬂ 
//08.05.2012
//Compiler CC5x

#include "platform.h"

extern char gTimecounter;

#if X86
void* timer_interrupt(void* unused);
#endif

void TimerInit();
//To Do: Funktion bekommt einen Wert, anhand diese wertes wird die geschwindigkeit des
//Timers festgelegt. Wert kommt aus der berechnung einer anderen Funktion (fade oder run)
void TimerSetForFade(char value);

#define Timer2Interrupt(x)  		\
{									\
	TMR2IF = 0;						\
}

#define Timer4Interrupt(x)  		\
{									\
	TMR4IF = 0;						\
}

#endif
