/**
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#ifdef __CC8E__
#define DEBUG
#pragma optimize 1
#pragma sharedAllocation
#endif

//*********************** INCLUDEDATEIEN *********************************************
#include "Version.h"
#include "platform.h"
#include "RingBuf.h"
#include "usart.h"
#include "CommandIO.h"
#include "ledstrip.h"
#include "timer.h"
#include "rtc.h"
#include "ScriptCtrl.h"
#include "trace.h"
#include "error.h"
#include "Flash.h"

#ifdef __CC8E__
#include "int18XXX.h"
#endif /* #ifdef __CC8E__ */
//#include "MATH16.h"

#ifndef __CC8E__
#include <unistd.h>
jmp_buf g_ResetEnvironment;
#endif /* #ifndef CC8E */

//*********************** GLOBAL VARIABLES *******************************************
uns8 g_UpdateLed;
uns8 g_UpdateLedStrip;

//*********************** MACROS *****************************************************
#ifdef DEBUG
#define do_and_measure(METHOD) { \
		Timer_StartStopwatch(e ## METHOD); \
		METHOD(); \
		Timer_StopStopwatch(e ## METHOD); }
#else
#define do_and_measure(METHOD) METHOD();
#endif /*#ifdef DEBUG */


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
#if 0
	uns16 sv_FSR0 = FSR0;
	uns16 sv_FSR1 = FSR1;
	uns16 sv_FSR2 = FSR2;
	uns8 sv_PCLATH = PCLATH;
	uns8 sv_PCLATU = PCLATU;
	uns8 sv_PRODL = PRODL;
	uns8 sv_PRODH = PRODH;
	uns24 sv_TBLPTR = TBLPTR;
	uns8 sv_TABLAT = TABLAT;
#endif

	if(TMR5IF) {
		g_UpdateLed = g_UpdateLed + 1;
		ScriptCtrl_DecrementWaitValue();
		Timer5Interrupt();
	}

	if(TMR1IF) {
		g_UpdateLedStrip = g_UpdateLedStrip + 1;
		ScriptCtrl_CheckAndDecrementWaitValue();
		Timer1Disable();
		Timer1Interrupt();
	}
#if 0
	FSR0 = sv_FSR0;
	FSR1 = sv_FSR1;
	FSR2 = sv_FSR2;
	PCLATH = sv_PCLATH;
	PCLATU = sv_PCLATU;
	PRODL = sv_PRODL;
	PRODH = sv_PRODH;
	TBLPTR = sv_TBLPTR;
	TABLAT = sv_TABLAT;
#endif
	int_restore_registers
}

void HighPriorityInterruptFunction(void)
{
	uns16 sv_FSR0 = FSR0;
	if(RC1IF) {
		if(!RingBuf_HasError(&g_RingBuf)) {
			RingBuf_Put(&g_RingBuf, RCREG1);
		} else {
			//Register lesen um Schnittstellen Fehler zu vermeiden
			uns8 temp = RCREG1;
		}
	}
	FSR0 = sv_FSR0;
}
#endif /* #ifndef X86 */

#pragma cdata[VERSION_STRING_ORIGIN]
#pragma cdata.version = VERSION
#pragma cdata.CDATA_END
#pragma origin CDATA_END

//*********************** HAUPTPROGRAMM **********************************************
#ifdef __CC8E__
void main(void)
#else
int main(void)
#endif
{
	/* softReset() on x86 will jump here! */
	softResetJumpDestination();

	InitAll();

	while(1)
	{
		Timer_StartStopwatch(eMAIN);
#ifndef __CC8E__
		// give opengl thread a chance to run
		usleep(10);
#endif /* #ifndef __CC8E__ */



			do_and_measure(Platform_CheckInputs);

			do_and_measure(Error_Throw);

			do_and_measure(CommandIO_GetCommands);

		if(g_UpdateLedStrip > 0) {
			do_and_measure(Ledstrip_UpdateLed);
			Timer1Enable();
			g_UpdateLedStrip = 0;
		}
		Timer_StopStopwatch(eMAIN);

			do_and_measure(ScriptCtrl_Run);

		if(g_UpdateLed > 0) {
			do_and_measure(Ledstrip_DoFade);

			Timer5InterruptLock();
			g_UpdateLed = 0;
			Timer5InterruptUnlock();

		}
	}
}
//*********************** UNTERPROGRAMME **********************************************

void InitAll()
{
	clearRAM();
	Platform_OsciInit();
	Platform_IOInit();
	RingBuf_Init(&g_RingBuf);
	UART_Init();
	Timer_Init();
	Ledstrip_Init();
	CommandIO_Init();
	Rtc_Init();
	ScriptCtrl_Init();
	Trace_Init();

#ifndef __CC8E__
	init_x86();
#endif /* #ifndef CC8E */

	Platform_AllowInterrupts();
	Platform_DisableBootloaderAutostart();

	Trace_String("Startup");

	/* Startup Wait-Time 2s
	 * to protect Wifly-Modul from errors*/
	gScriptBuf.waitValue = 20;
	CommandIO_CreateResponse(&g_ResponseBuf, FW_STARTED, OK);
	CommandIO_SendResponse(&g_ResponseBuf);
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
#include "CommandIO.c"
#include "platform.c"
#include "rtc.c"
#include "iic.c"
#include "ScriptCtrl.c"
#include "trace.c"
#include "Flash.c"
#include "Version.c"

// MUST be at the last position to prevent for overlapping code
#warning "Version String at 0x3000!!! check for overlapping code"

#endif /* #ifdef __CC8E__ */
