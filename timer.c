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
#include "trace.h"
#include "usart.h"
#include "ScriptCtrl.h"

bank3 struct CycleTimeBuffer g_CycleTimeBuffer;

unsigned short gDateTimer;
struct date_event gDateEvents[NUM_DATE_EVENTS];

unsigned char date_timer_add_event(struct cmd_add_color* pCmd)
{
	int i;
	for(i = 0; i < NUM_DATE_EVENTS; i++)
	{
		if(0xffff == gDateEvents[i].wakeup)
		{
//TODO			uns16 hour = (uns16)pCmd->hour * 1800;
			uns16 minute = (uns16)pCmd->minute * 30;
			uns16 second = pCmd->second / 2;
			gDateEvents[i].wakeup = DATE_TIMER_DAY;
//TODO			gDateEvents[i].wakeup -= hour;
			gDateEvents[i].wakeup -= minute;
			gDateEvents[i].wakeup -= second;
			gDateEvents[i].cmd.cmd = SET_COLOR;
//TODO memcpy((char*)&gDateEvents[i].cmd, (char*)pCmd, sizeof(struct led_cmd));
			gDateEvents[i].cmd.data.set_color.addr[0] = 0xff;
			gDateEvents[i].cmd.data.set_color.addr[1] = 0xff;
			gDateEvents[i].cmd.data.set_color.addr[2] = 0xff;
			gDateEvents[i].cmd.data.set_color.addr[3] = 0xff;
			uns8 red = pCmd->red;
			gDateEvents[i].cmd.data.set_color.red = red;
			uns8 green = pCmd->green;
			gDateEvents[i].cmd.data.set_color.green = green;
			uns8 blue = pCmd->blue;
			gDateEvents[i].cmd.data.set_color.blue = blue;
			return TRUE;
		}
	}
	return FALSE;
}
		

//this function has to be called each 2 seconds!
void date_timer_callback(void)
{
	if(0 == --gDateTimer) gDateTimer = DATE_TIMER_DAY;
}

void date_timer_do_events(void)
{
	int i;
	for(i = 0; i < NUM_DATE_EVENTS; i++)
	{
		if(gDateEvents[i].wakeup == gDateTimer)
		{
			/* TODO implement a more generic function
			 * maybe make led_cmd a class and add a function exec() or run() */
			if(SET_COLOR == gDateEvents[i].cmd.cmd)
			{
				Ledstrip_SetColor(&gDateEvents[i].cmd.data.set_color);
			}
			else
			{
				Trace_String("Unsupported command in date event ");
				Trace_Hex(gDateEvents[i].cmd.cmd);
				Trace_String("\n");
			}
		}
	}
}

void date_timer_init(void)
{
	gDateTimer = DATE_TIMER_DAY;
	uns8* ptr = (uns8*)gDateEvents;
	uns8 i = sizeof(gDateEvents);
	do 
	{
		i--;
		ptr[i] = 0xff;
	} while(0 != i);
}


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
	T1CON = 0b00100111;
	TMR1IE = TRUE;
	
	/*
	 * T5 Interrupt every 5 Millisecounds if clock is 64MHz
	 * Calculation
	 * 64000000 Hz / 8 / 40000
	 */
	T5CON = 0b01110111;
	TMR5IE = TRUE;
	TMR5H = 0x63;
	TMR5L = 0xC0;
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
	UART_Send(0x0d);
	UART_Send(0x0a);
	for(i = 0; i < enumSIZE; i++)
	{
		temp16 = g_CycleTimeBuffer.maxCycleTime[i]; 
		temp16 = temp16 >> 1;
		UART_SendString("Zeitwert ");
		UART_SendNumber(i,':');
#ifdef X86
		UART_SendNumber((uns8)(temp16 >> 8),'H');
		UART_SendNumber((uns8)(temp16 & 0xff),'L');
#else
		UART_SendHex_16(temp16);
#endif /* #ifdef X86_SRC */
		UART_SendString(" µS in HEX ");
		UART_Send(0x0d);
		UART_Send(0x0a);
		
		g_CycleTimeBuffer.maxCycleTime[i] = 0;
	}
	
	UART_SendString("WaitValue:");
	UART_SendHex_16(gScriptBuf.waitValue);
	UART_Send(0x0d);
	UART_Send(0x0a);
}
