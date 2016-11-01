/*
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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
//*********************** CONFIGURATION ********************************************
#define NUM_OF_LED 32

#ifdef __CC8E__
#ifndef __SDCC_pic16
#include "INLINE.H"
#else
#include <stdint.h>
#include "/home/gpb/workspace/WyLight/tools/sdcc/device/non-free/include/pic16/pic18f26k22.h"
#define HIGH_BYTE(word) ((uint8_t)(word >> 8))
#define LOW_BYTE(word) ((uint8_t)(0xff & word))
typedef int8_t bit;
typedef uint8_t uns8;
typedef uint16_t uns16;
typedef int interrupt;
#define bank1
#define bank2
#define bank3
#define bank5
#define bank6
#define bank7
#define bank10

#define W _W
#define WR _WR
#define CFGS _CFGS
#define EEPGD _EEPGD
#define FSR0 _FSR0
#define GIE _GIE
#define RC1IF _RC1IF
#define RD _RD
#define TMR1IF _TMR1IF
#define TMR1ON _TMR1ON
#define TMR5IE _TMR5IE
#define TMR5IF _TMR5IF
#define WREN _WREN

#endif
#include "INT18XXX.H"

#define true TRUE
#define false FALSE
#define __attribute__(X)

#define softResetJumpDestination(x)

#define Platform_IOInit(x) do { CLRF(PORTB); CLRF(LATB); CLRF(ANSELB); CLRF(PORTA); CLRF(LATA); CLRF(ANSELA); \
                                CLRF(TRISA); PORTA = 0b00000100; } \
    while (0)                                                                         //Eingänge am PORTB initialisieren
#define Platform_OsciInit(x) do { OSCCON = 0b01110010; PLLEN = 1; } \
    while (0)                                                                    //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz

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

#else
#include <stdint.h>
#ifndef cc3200
#include <stdio.h>
#include <arpa/inet.h>
#include <setjmp.h>
#include <string.h>
#endif /* #ifndef cc3200 */

typedef int8_t bit;
typedef uint8_t uns8;
typedef uint16_t uns16;

#ifndef cc3200
//global variables
extern uns8 g_UpdateFade;
extern jmp_buf g_ResetEnvironment;
#endif /* #ifndef cc3200 */

#define bank1
#define bank2
#define bank3
#define bank5
#define bank6
#define bank7
#define bank10
#define Platform_EnableBootloaderAutostart(x)
#ifndef cc3200
#define clearRAM(x)
#define Platform_AllowInterrupts(x)
#define Platform_EnableAllInterrupts()
#define Platform_DisableAllInterrupts()
#define Platform_CheckInputs(x)
#define Platform_DisableBootloaderAutostart(x)
#define InitFactoryRestoreWLAN(x)
#define InitFET(x)
#define Platform_IOInit(x)
#define Platform_OsciInit(x)
#define softReset(x) longjmp(g_ResetEnvironment, 1)
#define softResetJumpDestination(x) setjmp(g_ResetEnvironment)
#else /*ifdef cc3200 */
#define softReset(x)
#endif /* #ifndef cc3200 */

#endif
#endif /* #ifndef _PLATFORM_H_ */
