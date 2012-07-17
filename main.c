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
#include "int18XXX.h"
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
char g_update_fade:1;	
//*********************** X86 InterruptRoutine *******************************************


#ifndef X86
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 8					//Adresse des High Priority Interrupts	
interrupt InterruptRoutine(void)
{
	uns16 sv_FSR0 = FSR0;
	if(RC1IF)
	{
		if(!RingBufHasError) RingBufPut(RCREG1);
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			char temp = RCREG1;
		}
	}
	if(TMR2IF)
	{
		Timer2Interrupt();
		gTimecounter = ++gTimecounter;
		commandstorage_wait_interrupt();
	}
	if(TMR4IF)
	{
		Timer4Interrupt();
		g_update_fade = 1;
		
	} 
	FSR0 = sv_FSR0;
	#pragma fastMode
}
#endif /* #ifndef X86 */

//*********************** FUNKTIONSPROTOTYPEN ****************************************
void InitAll();

#ifdef X86
void init_x86(void);
#endif /* #ifdef X86 */

//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
#ifndef X86
	clearRAM();
#endif
	InitAll();

	while(1)
	{
#ifdef X86
		// give opengl thread a chance to run
		usleep(10);
#endif
		CheckInputs();
		ThrowErrors();
		commandstorage_get_commands();
		commandstorage_execute_commands();
		ledstrip_do_fade();
		
		if(g_update_fade)
		{
			ledstrip_update_fade();
			g_update_fade = 0;
		}
	}
}
//*********************** UNTERPROGRAMME **********************************************

void InitAll()
{
	OsciInit();
	InitInputs();
	RingBufInit();
	USARTinit();
	spi_init();
	TimerInit();
	ledstrip_init();
	commandstorage_init();
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
#include "timer.c"
#include "usart.c"
#include "commandstorage.c"
#include "platform.c"
#endif /* #ifndef X86 */

