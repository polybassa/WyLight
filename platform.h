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

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#define TRUE  1
#define FALSE 0

#define CYCLE_TMMS 64		//cycle time in milliseconds

#ifdef X86
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

	#define AllowInterrupts(x) RCIE=1;PEIE=1;GIE=1;
	#define InitFactoryRestoreWLAN(x) TRISA.0 = 0; 
	#define InitFET(x) TRISC.0 = 0; //Ausgang für FET initalisieren
	#define InitInputs(x) CLRF(PORTB); CLRF(LATB); CLRF(ANSELB); //Eingänge am PORTB initialisieren
	#define OsciInit(x) OSCCON = 0b01110010; //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
	#define PowerOnLEDs(x) BCF(PORTC.0); //Spannungsversorgung für LED's einschalten
	#define PowerOffLEDs(x) BSF(PORTC.0); //Spannungsversorgung für LED's ausschalten
	
	void Check_INPUT();
#endif
#endif /* #ifndef _PLATFORM_H_ */
