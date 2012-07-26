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
	** T4 Interrupt every 2 Millisecound if clock is 64MHz
	** Calculation
	** 64000000 Hz / 4 / 16 / 200 / 10
	*/
	T4CON = 0b01001111;
	TMR4IE = 1;
	PR4 = 200;
	
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

void Timer_StopStopwatch(void)
{
#ifndef X86
	TMR3ON = 0;
	uns16 measuredValue,tempValue;
	uns8 value;
	measuredValue.low8 = TMR3L;
	measuredValue.high8 = TMR3H;
	
	UART_SendString("Measured Time:");
	
	tempValue = measuredValue >> 1;			//rotate right, so there are µS in tempValue
	
	if((tempValue & 0xf000) > 0)
	{
		value = (uns8)(tempValue >> 12);
		value &= 0x0f;
		value += '0';
		UART_Send(value);
	}
	if((tempValue & 0x0f00) > 0)
	{
		value = (uns8)(tempValue >> 8);
		value &= 0x0f;
		value += '0';
		UART_Send(value);
	}
	if((tempValue & 0x00f0) > 0)
	{
		value = (uns8)(tempValue >> 4);
		value &= 0x0f;
		value += '0';
		UART_Send(value);
	}
	if((tempValue & 0xf000) > 0)
	{
		value = (uns8)tempValue;
		value &= 0x0f;
		value += '0';
		UART_Send(value);
	}
	
	if((measuredValue & 0x0001) == 1) 		//check LSB if set
	{
		UART_Send('.');
		UART_Send('5');
		UART_SendString(" µSecounds in HEX");
	}
	
#endif
}
