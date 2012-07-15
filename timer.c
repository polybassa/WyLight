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

unsigned short gDateTimer;
bank6 struct date_event gDateEvents[NUM_DATE_EVENTS];

unsigned char date_timer_add_event(uns8 hour, uns8 minute, uns8 second, struct led_cmd* pCmd)
{
	int i;
	for(i = 0; i < NUM_DATE_EVENTS; i++)
	{
		if(0xffff == gDateEvents[i].wakeup)
		{
			hour *= 1800;
			minute *= 30;
			second /= 2;
			gDateEvents[i].wakeup = DATE_TIMER_DAY;
			gDateEvents[i].wakeup -= hour;
			gDateEvents[i].wakeup -= minute;
			gDateEvents[i].wakeup -= second;
			gDateEvents[i].cmd.cmd = SET_COLOR;
			memcpy((char*)&gDateEvents[i].cmd, (char*)pCmd, sizeof(struct led_cmd));
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
			commandstorage_exec_cmd(&gDateEvents[i].cmd);
		}
	}
}

void date_timer_init(void)
{
	gDateTimer = DATE_TIMER_DAY;
	memset(gDateEvents, 0xff, sizeof(gDateEvents));
}

void timer_init()
{
#ifndef X86
	T2CON = 0b01111101;
	TMR2IE = 1;
	
	T4CON = 0b00000101;
	TMR4IE = 1;
	
	PR4 = 0xff;
#endif

	date_timer_init();
}

void timer_set_for_fade(char value)
{
#ifndef X86
	PR2 = value;
#endif
}

