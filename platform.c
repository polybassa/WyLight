#include "platform.h"

#ifndef X86
void Check_INPUT()
{
//if INPUT is low, then FactoryReset the WLAN Interface
	if(PORTB.1 == 0)
		PORTA.0 = 0;
	else 
		PORTA.0 = 1;
	//Goto Bootloader if PORTB.0 is low
	if(PORTB.5 == 0)
	{
		#asm
		clrf PCLATH
		clrf PCL
		#endasm
	}
		
}

#endif /* X86 */