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

// a day has 86400 seconds, we are updating our counter in 2 sec steps
#define DATE_TIMER_DAY (unsigned short)43200
#define NUM_DATE_EVENTS 1

struct date_event {
	unsigned short wakeup;
	struct led_cmd cmd;
};

extern char gTimecounter;

void timer_init();
//TODO: Funktion bekommt einen Wert, anhand diese wertes wird die geschwindigkeit des
//Timers festgelegt. Wert kommt aus der berechnung einer anderen Funktion (fade oder run)
void timer_set_for_fade(char value);

unsigned char date_timer_add_event(struct cmd_add_color* pCmd);
void date_timer_callback(void);
void date_timer_do_events(void);

#define Timer2interrupt(x)  		\
{									\
	TMR2IF = 0;						\
}

#define Timer4interrupt(x)  		\
{									\
	TMR4IF = 0;						\
}

#endif
