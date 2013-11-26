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

#include "platform.h"
//#include "ScriptCtrl.h"
#include "ledstrip.h"

#ifdef __CC8E__
void Platform_CheckInputs()
{
	static uns8 lastSwitchState;
	if(PORTB .5 == 0) {
		lastSwitchState = 1;
		return;
	}
	if(PORTB .5 == 1) {
		if(lastSwitchState == 1) {
			if(gScriptBuf.isRunning) {
				Ledstrip_FadeOffLeds();
				gScriptBuf.isRunning = FALSE;
			} else
				gScriptBuf.isRunning = TRUE;

		}
		lastSwitchState = 0;
	}
	/*
	//Goto Bootloader if PORTB.0 is low
	if(PORTB.5 == 0)
	{
	        softReset();
	}*/
}

void Platform_AllowInterrupts()
{
	IPEN = 1;
	TMR1IP = 0;
	TMR2IP = 0;
	TMR3IP = 0;
	TMR4IP = 0;
	TMR5IP = 0;
	RC1IP = 1;
	RC1IE = 1;
	PEIE = 1;
	GIEL = 1;
	GIEH = 1;
}

void Platform_EnableAllInterrupts()
{
	GIEL = 1;
	GIEH = 1;
}
/*
void Platform_DisableAllInterrupts()
{
        GIEL = 0;
        GIEH = 0;
}*/

void Platform_DisableBootloaderAutostart()
{
	Eeprom_Write(0x3ff, 1);
}

void Platform_EnableBootloaderAutostart()
{
	Eeprom_Write(0x3ff, 0xff);
}

uns16 htons(uns16 hostShort)
{
	uns16 retval;
	retval.low8 = hostShort.high8;
	retval.high8 = hostShort.low8;
	return retval;
}

uns16 ntohs(uns16 networkShort)
{
	uns16 retval;
	retval.low8 = networkShort.high8;
	retval.high8 = networkShort.low8;
	return retval;
}

#endif /* __CC8E__ */
