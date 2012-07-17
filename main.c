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
//*************************** GLOBAL VARIABLES ***************************************
char do_update_fade;

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
		commandstorage_wait_interrupt();
	}
	if(TMR4IF)
	{
		Timer4interrupt();
		do_update_fade = 1;
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
#ifndef X86
	clearRAM();
#endif
	init_all();

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
#ifdef with_TIMER
		date_timer_do_events();
#endif
		if(do_update_fade)
		{
			do_update_fade = 0;
			ledstrip_update_fade();
		}
		
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

#ifdef X86
	init_x86();
#endif /* #ifdef X86 */
	
	// *** send ready after init
	USARTsend('R');
	USARTsend('D');
	USARTsend('Y');
	
	AllowInterrupts();
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

