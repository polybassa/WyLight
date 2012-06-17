#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#define TRUE  1
#define FALSE 0

#ifdef X86
	typedef char bit;
	typedef unsigned char uns8;

	//global variables
	bit g_led_off;

	#define AllowInterrupts(x)
	#define FactoryRestoreWLAN(x)
	#define InitFET(x)
	#define OsciInit(x)
	#define PowerOnLEDs(x) g_led_off = 0;
	#define PowerOffLEDs(x) g_led_off = 1;

#else
	#include "inline.h"

	#define AllowInterrupts(x) RCIE=1;PEIE=1;GIE=1;
	#define FactoryRestoreWLAN(x) TRISA.0 = 0; PORTA.0 = 1;
	#define InitFET(x) TRISC.0 = 0; //Ausgang für FET initalisieren
	#define OsciInit(x) OSCCON = 0b01110010; //OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
	#define PowerOnLEDs(x) BCF(PORTC.0); //Spannungsversorgung für LED's einschalten
	#define PowerOffLEDs(x) BSF(PORTC.0); //Spannungsversorgung für LED's ausschalten
#endif
#endif /* #ifndef _PLATFORM_H_ */
