#include "platform.h"

#ifndef X86
void Check_INPUT()
{
//if INPUT is low, then FactoryReset the WLAN Interface
	if(PORTB.0 == 0)
		PORTA.0 = 0;
	else 
		PORTA.0 = 1;
}

#endif /* X86 */