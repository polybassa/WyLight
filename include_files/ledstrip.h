#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#include "include_files\spi.h"

#define NUM_OF_LED 32
#define BUFFERSIZE (NUM_OF_LED*3)

struct LedBuffer{
	char led_array[BUFFERSIZE];
	char led_ctrl_array[BUFFERSIZE];
	};
extern struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(char red, char green, char blue);

#include "include_files\ledstrip.c"

#endif