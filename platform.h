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
	#include <arpa/inet.h>
	#include <string.h>

	typedef char bit;
	typedef unsigned char uns8;
	typedef unsigned short uns16;

	//global variables
	extern bit g_led_off;

	#define bank1
	#define AllowInterrupts(x)
	#define InitFactoryRestoreWLAN(x)
	#define InitFET(x)
	#define InitInputs(x)	
	#define OsciInit(x)
	#define PowerOnLEDs(x) g_led_off = 0;
	#define PowerOffLEDs(x) g_led_off = 1;
	#define Check_INPUT(x)	
	
#else

	#include "inline.h"

	#define htons(X) (X)
	#define ntohs(X) (X)

	#define memset(PTR, VALUE, NUM_BYTES) { \
		short k; \
		for(k = NUM_BYTES - 1; k >= 0; k--) { \
			PTR[k] = VALUE; \
		} \
	}

	#define AllowInterrupts(x) RCIE=1;PEIE=1;GIE=1;
	#define InitFactoryRestoreWLAN(x) TRISA.0 = 0; 
	#define InitInputs(x) CLRF(PORTB); CLRF(LATB); CLRF(ANSELB); //Eingänge am PORTB initialisieren
	#define OsciInit(x) OSCCON = 0b01110010; //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
	
	void CheckInputs();
#endif
#endif /* #ifndef _PLATFORM_H_ */
