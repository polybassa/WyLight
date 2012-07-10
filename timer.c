//Nils Weiﬂ 
//08.05.2012
//Compiler CC5x

#include "timer.h"
#include "ledstrip.h"

void timer_init()
{
	T2CON = 0b01111101;
	TMR2IE = 1;
	
	T4CON = 0b00000101;
	TMR4IE = 1;
	
	PR4 = 0xff;

	g_timer_signaled = 0;
}

void timer_set_for_fade(char value)
{
	PR2 = value;
}

