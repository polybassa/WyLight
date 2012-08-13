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
#include "commandstorage.h" //under construction
#include "ledstrip.h"		//clean
#include "timer.h"			//under construction
#include "rtc.h"
#ifdef __CC8E__
#include "int18XXX.h"
#endif /* #ifdef __CC8E__ */
//#include "MATH16.h"

#ifdef X86
#include <unistd.h>
#endif /* #ifdef X86 */

//*********************** GLOBAL VARIABLES *******************************************
uns8 g_TmmsCounter;

//*********************** FUNKTIONSPROTOTYPEN ****************************************
void InitAll();
void HighPriorityInterruptFunction(void);
#ifdef X86
void init_x86(void);
#endif /* #ifdef X86 */

#ifndef X86
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 0x8					//Adresse des High Priority Interrupts	
interrupt HighPriorityInterrupt(void)
{
	HighPriorityInterruptFunction();
	#pragma fastMode
}

#pragma origin 0x18
interrupt LowPriorityInterrupt(void)
{
	int_save_registers
	uns16 sv_FSR0 = FSR0;
	uns16 sv_FSR1 = FSR1;
	uns16 sv_FSR2 = FSR2;
	uns8 sv_PCLATH = PCLATH;
	uns8 sv_PCLATU = PCLATU;
	uns8 sv_PRODL = PRODL;
	uns8 sv_PRODH = PRODH;
	uns24 sv_TBLPTR = TBLPTR;
	uns8 sv_TABLAT = TABLAT;

	if(TMR1IF)
	{
		Timer1Interrupt();
		Commandstorage_WaitInterrupt();
	}
	if(TMR4IF)
	{
		Timer4Interrupt();
		g_TmmsCounter++;
	} 
	if(TMR2IF)
	{
		Timer2Interrupt();
	}
	
	FSR0 = sv_FSR0;
	FSR1 = sv_FSR1;
	FSR2 = sv_FSR2;
	PCLATH = sv_PCLATH;
	PCLATU = sv_PCLATU;
	PRODL = sv_PRODL;
	PRODH = sv_PRODH;
	TBLPTR = sv_TBLPTR;
	TABLAT = sv_TABLAT;

	int_restore_registers
}

void HighPriorityInterruptFunction(void)
{
	uns16 sv_FSR0 = FSR0;
	if(RC1IF)
	{
		if(!RingBuf_HasError) 
		{
			RingBuf_Put(RCREG1);
		}
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			unsigned char temp = RCREG1;
		}
	}
	FSR0 = sv_FSR0;
}
#endif /* #ifndef X86 */


//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
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
		if(g_TmmsCounter >= CYCLE_TMMS)
		{
			Timer4InterruptLock();
			g_TmmsCounter -= CYCLE_TMMS;
			Timer4InterruptUnlock();

			Ledstrip_UpdateFade();
			Ledstrip_DoFade();
		}
	}
}
//*********************** UNTERPROGRAMME **********************************************

void InitAll()
{
	clearRAM();
	Platform_OsciInit();
	Platform_IOInit();
	RingBuf_Init();
	UART_Init();
	Timer_Init();
	Ledstrip_Init();
	Commandstorage_Init();
	Error_Init();
	Commandstorage_Clear();
	Rtc_Init();

	g_TmmsCounter = 0;

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

#ifdef __CC8E__
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
#include "rtc.c"
#include "iic.c"
#endif /* #ifdef __CC8E__ */

