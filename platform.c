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

#ifndef X86
void Platform_CheckInputs()
{
	//Goto Bootloader if PORTB.0 is low
	if(PORTB.5 == 0)
	{
		softReset();
	}
		
}

void Platform_AllowInterrupts()
{
	IPEN = 1;
	TMR1IP = 0;
	TMR2IP = 0;
	TMR3IP = 0;
	TMR4IP = 0;
	RC1IP = 1;
	RC1IE = 1; 
	PEIE = 1; 
	GIEL = 0; // Low Priority Interrupts are disabled
	GIEH = 1;
}

void Platform_DisableBootloaderAutostart()
{
	Eeprom_Write(1023, 1);
}
#endif /* X86 */
