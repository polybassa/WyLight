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

void timer_init();
//To Do: Funktion bekommt einen Wert, anhand diese wertes wird die geschwindigkeit des
//Timers festgelegt. Wert kommt aus der berechnung einer anderen Funktion (fade oder run)
void timer_set_for_fade(char value);

#define Timer2interrupt(x)  		\
{									\
	TMR2IF = 0;						\
	gTimecounter = ++gTimecounter;	\
}

#define Timer4interrupt(x)  		\
{									\
	TMR4IF = 0;						\
	g_timer_signaled++;				\
}

#endif
