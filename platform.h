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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#define TRUE  1
#define FALSE 0

//*********************** CONFIGURATION ********************************************
#define WIFLY_SERVER_PORT 2000 // TCP/UDP Port of Wifly device
#define CYCLE_TMMS 64		//cycle time in milliseconds

#ifdef X86
	#include <stdio.h>
	#include <arpa/inet.h>
	#include <string.h>

	typedef char bit;
	typedef unsigned char uns8;
	typedef unsigned short uns16;

	//global variables
	extern bit g_led_off;
	extern unsigned char g_UpdateFade;	

	#define bank1
	#define Platform_AllowInterrupts(x)
	#define Platform_CheckInputs(x)
	#define Platform_DisableBootloaderAutostart(x)
	#define InitFactoryRestoreWLAN(x)
	#define InitFET(x)
	#define Platform_IOInit(x)	
	#define Platform_OsciInit(x)
	
#else
	#include "inline.h"
	#include "int18XXX.h"

	#define htons(X) (X)
	#define ntohs(X) (X)

	#define Platform_AllowInterrupts(x) RC1IE=1; PEIE=1; GIE=1;
	#define Platform_IOInit(x) CLRF(PORTB); CLRF(LATB); CLRF(ANSELB); //Eingänge am PORTB initialisieren
	#define Platform_OsciInit(x) OSCCON = 0b01110010; PLLEN = 1;//OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
	
	#define memset(PTR, VALUE, NUM_BYTES) { \
		short k; \
		for(k = NUM_BYTES - 1; k >= 0; k--) { \
			PTR[k] = VALUE; \
		} \
	}
	
	void Platform_CheckInputs();
	
	/*** This Function will Disable the Autostart to the Bootloader.
	* At Startup, Bootloader checks the last EEPROM-Cell. If there is 
	* 0x01 in the EEPROM-Cell, the Bootloader will go directly to the
	* Application otherwise the Bootloader stays in Bootloader-Mode.
	*/
	void Platform_DisableBootloaderAutostart();
#endif
#endif /* #ifndef _PLATFORM_H_ */
