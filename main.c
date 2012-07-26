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
//#define TEST_COMMAND
#pragma optimize 0
//#pragma resetVector 0x400
//#pragma unlockISR
#endif
#pragma sharedAllocation

//*********************** INCLUDEDATEIEN *********************************************
#include "platform.h"
#include "spi.h"
#include "RingBuf.h"		//clean
#include "usart.h"			//clean
#include "eeprom.h"       	//clean 
#include "crc.h"			//clean
#include "commandstorage.h" //under construction
#include "ledstrip.h"		//clean
#include "timer.h"			//under construction
#include "rtc.h"

#ifdef X86
#include <unistd.h>
#endif /* #ifdef X86 */

//*********************** GLOBAL VARIABLES *******************************************
uns8 g_UpdateFade,g_DoFade;	
//*********************** X86 InterruptRoutine *******************************************

#ifndef X86
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 8					//Adresse des High Priority Interrupts	
interrupt InterruptRoutine(void)
{
	uns16 sv_FSR0 = FSR0;
	if(RC1IF)
	{
		if(!RingBuf_HasError) RingBuf_Put(RCREG1);
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			unsigned char temp = RCREG1;
		}
	}
	if(TMR1IF)
	{
		Timer1Interrupt();
		Commandstorage_WaitInterrupt();
	}
	if(TMR4IF)
	{
		Timer4Interrupt();
		g_UpdateFade = 1;
	} 
	if(TMR2IF)
	{
		Timer2Interrupt();
		g_DoFade = 1;
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
		Platform_CheckInputs();
		Error_Throw();
		Commandstorage_GetCommands();
		Commandstorage_ExecuteCommands();

		if(g_UpdateFade)
		{
			//Timer_StartStopwatch();
			Ledstrip_UpdateFade();
			Ledstrip_DoFade();
			//Timer_StopStopwatch();
			g_UpdateFade = 0;
		}
		

	}
}
//*********************** UNTERPROGRAMME **********************************************

void InitAll()
{
	Platform_OsciInit();
	Platform_IOInit();
	RingBuf_Init();
	UART_Init();
	SPI_Init();
	I2C_Init();
	Timer_Init();
	Ledstrip_Init();
	Commandstorage_Init();
	Error_Init();
	Commandstorage_Clear();

#ifdef X86
	init_x86();
#endif /* #ifdef X86 */
	
	// *** send ready after init
	UART_Send('R');
	UART_Send('D');
	UART_Send('Y');
	
	Platform_AllowInterrupts();
	Platform_DisableBootloaderAutostart();
}

// cc5xfree is a bit stupid so we include the other implementation files here
#ifndef X86
//#pragma codepage 1
#include "crc.c"
#include "eeprom.c"
#include "error.c"
#include "ledstrip.c"
#include "spi.c"
#include "timer.c"
#include "RingBuf.c"
#include "usart.c"
#include "commandstorage.c"
#include "platform.c"
#include "iic.c"
#endif /* #ifndef X86 */

