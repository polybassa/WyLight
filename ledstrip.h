#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x


#include "spi.h"
#include "eeprom.h"

#define NUM_OF_LED 32

struct LedBuffer{
	char led_array[NUM_OF_LED*3];
	};

extern struct LedBuffer gLedBuf;

void ledstrip_init(void);
#ifndef USE_UNION
void ledstrip_set_color(char *address,char r,char g,char b);
#else
void ledstrip_set_color(struct cmd_set_color *pCmd);
#endif
void sub_func_set_color(char *cmdPointer);
#endif
