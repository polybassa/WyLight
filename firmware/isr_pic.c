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

void HighPriorityInterruptFunction(void);

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
