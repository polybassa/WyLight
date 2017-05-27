/**
   Copyright (C) 2012 - 2017 Nils Weiss, Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

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

#ifdef __CC8E__
void HighPriorityInterruptFunction(void);
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

    if (TMR5IF) {
        g_UpdateLed = g_UpdateLed + 1;
        ScriptCtrl_DecrementWaitValue();
        Timer5Interrupt();
    }

    if (TMR1IF) {
        g_UpdateLedStrip = g_UpdateLedStrip + 1;
        ScriptCtrl_CheckAndDecrementWaitValue();
        Timer1Disable();
        Timer1Interrupt();
    }
    int_restore_registers
}

void HighPriorityInterruptFunction(void)
{
    uns16 sv_FSR0 = FSR0;
    if (RC1IF) {
        //Replace RingBuf_Put to avoid failures when main-cycle call's RingBuf_Put
        if (!g_RingBuf.error_full) {
            uns8 writeNext = RingBufInc(g_RingBuf.write);
            if (writeNext != g_RingBuf.read) {
                uns8 write = g_RingBuf.write;
                g_RingBuf.data[write] = RCREG1;
                g_RingBuf.write = writeNext;
            } else {g_RingBuf.error_full = 1; }
        } else {
            //Register lesen um Schnittstellen Fehler zu vermeiden
            uns8 temp = RCREG1;
        }
    }
    FSR0 = sv_FSR0;
}

#pragma cdata[VERSION_STRING_ORIGIN]
#pragma cdata.version = VERSION
#pragma cdata.CDATA_END
#pragma origin CDATA_END
#endif /* #ifdef __CC8E__ */

void InitAll()
{
    clearRAM();
    Trace_Init();
    Platform_OsciInit();
    Platform_IOInit();
    RingBuf_Init(&g_RingBuf);
    UART_Init();
    Timer_Init();
    Ledstrip_Init();
    CommandIO_Init();
    Rtc_Init();
    ScriptCtrl_Init();
    Platform_ExtraInit();

    Platform_AllowInterrupts();

    /* Startup Wait-Time 2s
     * to protect Wifly-Modul from errors*/
    gScriptBuf.waitValue = 20;
    CommandIO_CreateResponse(&g_ResponseBuf, FW_STARTED, OK);
    CommandIO_SendResponse(&g_ResponseBuf);
    Trace_String(" Init Done ");
    Platform_DisableBootloaderAutostart();
}

//*********************** HAUPTPROGRAMM **********************************************
Platform_Main(void)
{
    /* softReset() on x86 will jump here! */
    softResetJumpDestination();

    InitAll();

    while (1) {
        Timer_StartStopwatch(eMAIN);
        // give opengl thread a chance to run
        Platform_MainLoopSleep();

        do_and_measure(Platform_CheckInputs);

        do_and_measure(Error_Throw);

        do_and_measure(CommandIO_GetCommands);

        if (g_UpdateLedStrip > 0) {
            do_and_measure(Ledstrip_UpdateLed);
            Timer1Enable();
            g_UpdateLedStrip = 0;
        }
        Timer_StopStopwatch(eMAIN);

        do_and_measure(ScriptCtrl_Run);

        if (g_UpdateLed > 0) {
            do_and_measure(Ledstrip_DoFade);

            Timer5InterruptLock();
            g_UpdateLed = 0;
            Timer5InterruptUnlock();
        }
    }
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
