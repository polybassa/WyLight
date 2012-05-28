#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#include "commandstorage.h"
#include "spi.h"
#include "eeprom.h"

#define NUM_OF_LED 32

struct LedBuffer{
	uns8 led_array[NUM_OF_LED*3];
	int8 led_vector[NUM_OF_LED*3];
	uns8 led_destination[NUM_OF_LED*3];
	char led_operation_running:1;
	};

extern struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(struct cmd_set_color *pCmd);
void ledstrip_set_fade(struct cmd_set_fade *pCmd);
#endif
