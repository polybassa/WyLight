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
#include "ledstrip.h"

void TimerInit()
{
	T1CON = 0b00110111;
	TMR1IE = 1;
    //T4 Interrupt every 2 Millisecound if clock is 64MHz
	//Calculation
	//64000000 Hz / 4 / 16 / 200 / 10
	T4CON = 0b01001111;
	TMR4IE = 1;
	PR4 = 200;
}

void TimerSetForFade(char value)
{
	//Not Implemented yet
}

