//Nils Weiﬂ 
//08.05.2012
//Compiler CC5x

#include "timer.h"
#include "ledstrip.h"

#ifndef X86
void timer_init()
{
	T2CON = 0b01111101;
	TMR2IE = 1;
	
	T4CON = 0b01111111;
	TMR4IE = 1;
	
	PR4 = 0xff;
}

void timer_set_for_fade(char value)
{
	PR2 = value;
}
#endif

#if X86
void* timer_interrupt(void* unused)
{
	for(;;usleep(1000))
	{
		uns8 i;
		for(i = 0; i < NUM_OF_LED*3; i++)
		{
			if((gLedBuf.delta[i] > 0) && (gLedBuf.cyclesLeft[i] > 0))
			{
				gLedBuf.cyclesLeft[i]--;		
			}
		}
	}
}
#endif
