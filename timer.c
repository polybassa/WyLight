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

unsigned short gDateTimer;
bank6 struct date_event gDateEvents[NUM_DATE_EVENTS];

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
			Commandstorage_ExecuteCmd(&gDateEvents[i].cmd);
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
#endif /* #ifdef __CC8E__ */
	date_timer_init();
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
/*
	TMR3ON = 0;
	uns16 measuredValue,tempValue;
	uns8 value[8],temp,i;
	uns8* ptrValue;
	measuredValue.low8 = TMR3L;
	measuredValue.high8 = TMR3H;
	
	UART_SendString("Measured Time:");
	
	tempValue = measuredValue >> 1;			//rotate right, so there are µS in tempValue
	
	ptrValue = &value[0];
	while(!(ptrValue == &value[7]))
	{
		W = tempValue.low8;
		W = decadj(W);
		temp = W;
		*ptrValue = temp & 0x0f;
		ptrValue++;
		*ptrValue = temp & 0xf0;
		//Under Construction
	}
	
	if((tempValue & 0xfF00) > 0)
	{
		value = (uns8)(tempValue >> 12);
		value &= 0x0f;
		W = value;
		W = decadj(W);
		value = W;
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
		UART_SendString(" µS in HEX ");
		UART_Send(0x0d);
		UART_Send(0x0a);
	}
	*/
#endif
}
