#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#define TRUE  1
#define FALSE 0

#ifdef X86
	typedef char bit;

	#define AllowInterrupts(x)
	#define FactoryRestoreWLAN(x)
	#define InitFET(x)
	#define OsciInit(x)
	#define PowerOnLEDs(x)
#else
	#include "inline.h"

	#define AllowInterrupts(x) RCIE=1;PEIE=1;GIE=1;
	#define FactoryRestoreWLAN(x) TRISA.0 = 0; PORTA.0 = 1;
	#define InitFET(x) TRISC.0 = 0; //Ausgang für FET initalisieren
	#define OsciInit(x) OSCCON = 0b01110010; //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
	#define PowerOnLEDs(x) PORTC.0 = 0; //Spannungsversorgung für LED's einschalten
	#define PowerOffLEDs(x) PORTC.0 = 1; //Spannungsversorgung für LED's ausschaltenP
#endif
#endif /* #ifndef _PLATFORM_H_ */
