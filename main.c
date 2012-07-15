/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
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


#ifndef X86
#define NO_CRC
//#define TEST
#pragma optimize 0
//#pragma resetVector 0x400
//#pragma unlockISR
#endif
#pragma sharedAllocation

//*********************** INCLUDEDATEIEN *********************************************
#include "platform.h"
#include "RingBuf.h"		//clean
#include "usart.h"			//clean
#include "eeprom.h"       	//clean 
#include "crc.h"			//clean
#include "commandstorage.h" //under construction
#include "ledstrip.h"		//clean
#include "timer.h"			//under construction
#ifdef X86
#include <unistd.h>
#endif /* #ifdef X86 */

//*********************** GLOBAL VARIABLES *******************************************
//TODO wird dieser noch verwendet???? 
char gTimecounter;

#ifndef X86
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 4					//Adresse des Interrupts	
interrupt InterruptRoutine(void)
{
	if(RCIF)
	{
		if(!RingBufHasError) RingBufPut(RCREG);
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			char temp = RCREG;
		}
	}
	if(TMR2IF)
	{
		Timer2interrupt();
	}
	if(TMR4IF)
	{
		Timer4interrupt();
		commandstorage_wait_interrupt();
	}
}
#endif /* #ifndef X86 */

//*********************** FUNKTIONSPROTOTYPEN ****************************************
void init_all();
#ifdef X86
void init_x86(void);
#endif /* #ifdef X86 */

//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
	init_all();
#ifdef TEST
{
	struct cmd_add_color dateCmd;
/**TODO	dateCmd.data.add_color.addr[0] = 0xff;
	dateCmd.data.add_color.addr[1] = 0xff;
	dateCmd.data.add_color.addr[2] = 0xff;
	dateCmd.data.add_color.addr[3] = 0xff;
*/
	dateCmd.red = 0xff;
	dateCmd.green = 0;
	dateCmd.blue = 0;
//TODO	dateCmd.data.add_color.hour = 0xff;
	dateCmd.minute = 0;
	dateCmd.second = 3;
	date_timer_add_event(&dateCmd);
}
#endif /* #ifdef TEST */
    
	while(1)
	{
#ifdef X86
		// give opengl thread a chance to run
		usleep(10);
#endif
		Check_INPUT();
		throw_errors();
		commandstorage_get_commands();
		commandstorage_execute_commands();
		ledstrip_do_fade();
		date_timer_do_events();
	}
}
//*********************** UNTERPROGRAMME **********************************************

void init_all()
{
	OsciInit();
	InitInputs();
	RingBufInit();
	USARTinit();
	timer_init();
	ledstrip_init();
	commandstorage_init();
	
	InitFET();
	PowerOnLEDs();
	InitFactoryRestoreWLAN();
	ErrorInit();
	ClearCmdBuf();	
	AllowInterrupts();

#ifdef X86
	init_x86();
#endif /* #ifdef X86 */
	
	// *** send ready after init
	USARTsend('R');
	USARTsend('D');
	USARTsend('Y');
}

// cc5xfree is a bit stupid so we include the other implementation files here
#ifndef X86
//#pragma codepage 1
#include "crc.c"
#include "eeprom.c"
#include "error.c"
#include "ledstrip.c"
#include "RingBuf.c"
#include "spi.c"
#include "usart.c"
#include "commandstorage.c"
#include "platform.c"
#pragma codepage 1
#include "timer.c"
#endif /* #ifndef X86 */

