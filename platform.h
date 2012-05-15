#ifndef _PLATFORM_H_
#define _PLATFORM_H_

//*********************** ENUMERATIONS *********************************************
#define TRUE  1
#define FALSE 0

// *** ERRORBITS
struct ErrorBits {
		char crc_failure:1;
		char eeprom_failure:1;
};
extern struct ErrorBits gERROR;

#ifdef X86
	typedef char bit;
#else
	#include "inline.h"
#endif
#endif /* #ifndef _PLATFORM_H_ */
