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
#include "usart.h"

struct CycleTimeBuffer g_CycleTimeBuffer;

void Timer_Init()
{
#ifdef __CC8E__
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
	T3CON = 0b00110110;
	TMR3ON = 1;
#endif /* #ifdef __CC8E__ */
}

void Timer_SetForFade(char value)
{
	//Not Implemented yet
}

void Timer_StartStopwatch(enum METHODE destMethode)
{
	uns16 tempTime;

	Platform_ReadPerformanceCounter(tempTime);
	
	g_CycleTimeBuffer.tempCycleTime[destMethode] = tempTime;
}

void Timer_StopStopwatch(enum METHODE destMethode)
{
	uns16 tempTime,temp16;
	
	Platform_ReadPerformanceCounter(tempTime);
	
	if(g_CycleTimeBuffer.tempCycleTime[destMethode] < tempTime)
	{
		tempTime = tempTime - g_CycleTimeBuffer.tempCycleTime[destMethode];
	}
	else
	{
		temp16 = 0xffff - g_CycleTimeBuffer.tempCycleTime[destMethode];
		tempTime += temp16;
	}
	
	if(tempTime > g_CycleTimeBuffer.maxCycleTime[destMethode])
	{
		g_CycleTimeBuffer.maxCycleTime[destMethode] = tempTime;
	}
	g_CycleTimeBuffer.tempCycleTime[destMethode] = 0;
}

void Timer_PrintCycletime(void)
{
	uns8 i;
	uns16 temp16;
	for(i = 0; i < enumSIZE; i++)
	{
		temp16 = g_CycleTimeBuffer.maxCycleTime[i]; 
		UART_SendString("Zeitwert ");
		UART_SendNumber(i,':');
#ifdef X86
		UART_SendNumber((uns8)(temp16 >> 8),'H');
		UART_SendNumber((uns8)(temp16 & 0xff),'L');
#else
		UART_SendNumber(temp16.high8,'H');
		UART_SendNumber(temp16.low8,'L');
#endif /* #ifdef X86_SRC */
		UART_SendString(" µS in HEX ");
		UART_Send(0x0d);
		UART_Send(0x0a);
		
		g_CycleTimeBuffer.maxCycleTime[i] = 0;
	}	
}
