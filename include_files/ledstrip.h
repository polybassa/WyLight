#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#include "include_files\spi.h"
//#include "include_files\eeprom.h"

#define NUM_OF_LED 33
#define BUFFERSIZE 99

struct LedBuffer{
	char led_array[BUFFERSIZE];
	char led_ctrl_array[BUFFERSIZE];
	};
extern struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(char *address,char r,char g,char b);

#include "include_files\ledstrip.c"

#endif