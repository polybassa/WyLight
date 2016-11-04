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
#define SSPM0 SSP1CON1 .0
#define TRISC3 TRISC .3
#define TRISC4 TRISC .4
#define TRISC5 TRISC .5
#define TRISC6 TRISC .6
#define BootSwitchState(X) PORTB .5
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
#define bank14

#define CLRF(X) X = 0
#define BootSwitchState(X) PORTBbits.RB5

#ifdef __SDCC_pic16
#define clearRAM(x)
#define softReset(x)

#define ADDEN1 RC1STAbits.ADDEN1
#define BRGH1 TX1STAbits.BRGH1
#define BRG16 BAUD1CONbits.BRG16
#define CFGS EECON1bits.CFGS
#define CKE SSP1STATbits.CKE
#define CKP SSP1CON1bits.CKP
#define CREN1 RC1STAbits.CREN1
#define EEPGD EECON1bits.EEPGD

#define GIE INTCONbits.GIE
#define GIEL INTCONbits.PEIE_GIEL
#define GIEH INTCONbits.GIE_GIEH
#define IPEN RCONbits.IPEN

#define PEIE INTCONbits.PEIE

#define RC1IE PIE1bits.RC1IE
#define RC1IF PIR1bits.RC1IF
#define RC1IP IPR1bits.RC1IP
#define RD EECON1bits.RD
#define RX9_1 RC1STAbits.RX91

#define SMP SSP1STATbits.SMP
#define SPEN1 RC1STAbits.SPEN1
#define SSP1IF PIR1bits.SSP1IF
#define SSPEN SSP1CON1bits.SSPEN
#define SSPM0 SSP1CON1bits.SSPM0
#define SYNC1 TX1STAbits.SYNC1

#define TMR1IE PIE1bits.TMR1IE
#define TMR2IE PIE1bits.TMR2IE
#define TMR4IE PIE5bits.TMR4IE
#define TMR5IE PIE5bits.TMR5IE

#define TMR1IP IPR1bits.TMR1IP
#define TMR2IP IPR1bits.TMR2IP
#define TMR3IP IPR2bits.TMR3IP
#define TMR4IP IPR5bits.TMR4IP
#define TMR5IP IPR5bits.TMR5IP

#define TMR1ON T1CONbits.TMR1ON
#define TMR2ON T2CONbits.TMR2ON
#define TMR3ON T3CONbits.TMR3ON

#define TRISC3 DDRCbits.TRISC3
#define TRISC4 DDRCbits.TRISC4
#define TRISC5 DDRCbits.TRISC5
#define TRISC6 DDRCbits.TRISC6

#define TX1IF PIR1bits.TX1IF
#define TX9_1 TX1STAbits.TX91
#define TXEN1 TX1STAbits.TXEN1

#define WR EECON1bits.WR
#define WREN EECON1bits.WREN
#endif

#endif
#include "INT18XXX.H"

#define true TRUE
#define false FALSE
#define __attribute__(X)

#define softResetJumpDestination(x)

#define Platform_IOInit(x) do { CLRF(PORTB); CLRF(LATB); CLRF(ANSELB); CLRF(PORTA); CLRF(LATA); CLRF(ANSELA); \
                                CLRF(TRISA); PORTA = 0x4; } \
    while (0)                                                                         //Eingänge am PORTB initialisieren
#define Platform_OsciInit(x) do { \
        OSCCON = 0x72; \
        OSCTUNEbits.PLLEN = 1; \
} while (0)                                                                    //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz

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
