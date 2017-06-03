/*
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

#ifndef _PLATFORM_PIC_H_
#define _PLATFORM_PIC_H_

/*
 * Don't include this file directly. It is included
 * by the generic platform.h. Include that one instead.
 */

#include "inline.h"
#include "int18XXX.h"

#define true TRUE
#define false FALSE
#define __attribute__(X)

#define Platform_Main(x) \
    void main(x)
#define Platform_MainLoopSleep(x)
#define Platform_ExtraInit(x)
#define Platform_IOInit(x) do { CLRF(PORTB); CLRF(LATB); CLRF(ANSELB); CLRF(PORTA); CLRF(LATA); CLRF(ANSELA); \
                                CLRF(TRISA); PORTA = 0b00000100; } \
    while (0)                                                                         //Eingänge am PORTB initialisieren
#define Platform_OsciInit(x) do { OSCCON = 0b01110010; PLLEN = 1; } \
    while (0)                                                                    //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz

#define softResetJumpDestination(x)
void Platform_AllowInterrupts();
void Platform_EnableAllInterrupts();
//void Platform_DisableAllInterrupts();

void Platform_CheckInputs();

uns16 htons(uns16 hostShort);
uns16 ntohs(uns16 networkShort);

/*** This Function will Disable the Autostart to the Bootloader.
 * At Startup, Bootloader checks the last EEPROM-Cell. If there is
 * 0x01 in the EEPROM-Cell, the Bootloader will go directly to the
 * Application otherwise the Bootloader stays in Bootloader-Mode.
 */
void Platform_DisableBootloaderAutostart();
void Platform_EnableBootloaderAutostart();

#endif /* #ifndef _PLATFORM_PIC_H_ */
