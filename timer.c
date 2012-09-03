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
#include "USART.h"

void Timer_Init()
{
	T1CON = 0b00110111;
	TMR1IE = 1;
	
    /* 
	** T4 Interrupt every 1 Millisecound if clock is 64MHz
	** Calculation
	** 64000000 Hz / 4 / 16 / 100 / 10
	*/
	T4CON = 0b01001111;
	TMR4IE = 1;
	PR4 = 100;
	
	/* 
	** T2 Interrupt every 0.5 Millisecound if clock is 64MHz
	** Calculation
	** 64000000 Hz / 4 / 16 / 75 / 10
	*/
	T2CON = 0b01001111;
	TMR2IE = 1;
	PR2 = 75;
	
	/*
	** T3 Modul count with a frequency of 2MHz
	** Calculation:
	** 64MHz / 4 / 8
	*/
	T3CON = 0b00110100;
}

void Timer_SetForFade(char value)
{
	//Not Implemented yet
}

void Timer_StartStopwatch(void)
{
#ifndef X86
	TMR3H = 0;
	TMR3L = 0;
	
	TMR3ON = 1;
#endif
}

void Timer_StopStopwatch(uns16 *pTmax)
{
#ifndef X86

	TMR3ON = 0;
	uns16 measuredValue;
	measuredValue.low8 = TMR3L;
	measuredValue.high8 = TMR3H;
	
	measuredValue = measuredValue >> 1;			//rotate right, so there are µS in tempValue
	
	if(measuredValue > *pTmax)
	{
		*pTmax = measuredValue;
	}
	
#endif
}

void Timer_PrintCycletime(uns16 *pTmax)
{
	uns16 temp16 = *pTmax;
	UART_SendString("Measured Time:");
	UART_SendNumber(temp16.high8,'H');
	UART_SendNumber(temp16.low8,'L');
	UART_SendString(" µS in HEX ");
	UART_Send(0x0d);
	UART_Send(0x0a);
	*pTmax = 0;
}
