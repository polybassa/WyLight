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

#ifdef DEBUG
struct RingBuffer g_TraceBuf;
#endif
uns8 g_UpdateLedStrip;
uns8 g_UpdateLed;

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
