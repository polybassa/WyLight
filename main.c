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
#pragma optimize 1
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
#include "ScriptCtrl.h"
#include "trace.h"
#ifdef __CC8E__
#include "int18XXX.h"
#endif /* #ifdef __CC8E__ */
//#include "MATH16.h"

#ifdef X86
#include <unistd.h>
jmp_buf g_ResetEnvironment;
#endif /* #ifdef X86 */

//*********************** GLOBAL VARIABLES *******************************************
uns8 g_UpdateLed;
uns8 g_UpdateLedStrip;

//*********************** MACROS *****************************************************
#define do_and_measure(METHOD) {\
	Timer_StartStopwatch(e ## METHOD); \
	METHOD(); \
	Timer_StopStopwatch(e ## METHOD); \
}
	
//*********************** FUNKTIONSPROTOTYPEN ****************************************
void InitAll();
void HighPriorityInterruptFunction(void);
#ifdef X86
void init_x86(void);
#endif /* #ifdef X86 */

#ifndef X86
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 0x8					
//Adresse des High Priority Interrupts	
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

	if(TMR5IF)
	{
	      g_UpdateLed = g_UpdateLed + 1;
	      if(gScriptBuf.waitValue > 0)
	      {
		    gScriptBuf.waitValue = gScriptBuf.waitValue - 1;
	      }
	      Timer5Interrupt();
	} 
		
	if(TMR1IF)
	{
	      g_UpdateLedStrip = g_UpdateLedStrip + 1;
	      Timer1Disable();
	      Timer1Interrupt();
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
			//UART_Send(RCREG1+1);
		}
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			uns8 temp = RCREG1;
		}
	}
	FSR0 = sv_FSR0;
}
#endif /* #ifndef X86 */


//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
	/* softReset() on x86 will jump here! */
	softResetJumpDestination();

	InitAll();

	while(1)
	{
		Timer_StartStopwatch(eMAIN);
#ifdef X86
		// give opengl thread a chance to run
		usleep(10);
#endif /* #ifdef X86 */
		
		
		
		do_and_measure(Platform_CheckInputs);

		do_and_measure(Error_Throw);
	
		do_and_measure(Commandstorage_GetCommands);
				
		do_and_measure(ScriptCtrl_Run);

		if(g_UpdateLed > 0)
		{		  
			do_and_measure(Ledstrip_DoFade);
			
			do_and_measure(Ledstrip_UpdateRun);
			
			Timer5InterruptLock();
			g_UpdateLed = 0;
			Timer5InterruptUnlock();
			
		}
		if(g_UpdateLedStrip > 0)
		{
			do_and_measure(Ledstrip_UpdateLed);
			Timer1Enable();
			g_UpdateLedStrip = 0;
		}
		Timer_StopStopwatch(eMAIN);
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
	Error_Init();
	Commandstorage_Clear();
	Rtc_Init();
	ScriptCtrl_Init();

#ifdef X86
	init_x86();
#endif /* #ifdef X86 */
	
	Platform_AllowInterrupts();
	Platform_DisableBootloaderAutostart();
	
	UART_SendString("Wait");
	
	/* Startup Wait-Time 2s
	 * to protect Wifly-Modul from errors*/
	gScriptBuf.waitValue = 500;
	
	UART_Send('R');
	UART_Send('D');
	UART_Send('Y');
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
#include "ScriptCtrl.c"
#endif /* #ifdef __CC8E__ */
