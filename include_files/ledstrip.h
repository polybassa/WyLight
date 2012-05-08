#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#include "include_files\eeprom.h"

#define NUM_OF_LED 32

struct LedBuffer{
	char led_array_r[NUM_OF_LED];
	char led_array_g[NUM_OF_LED];
	char led_array_b[NUM_OF_LED];
	//char led_ctrl_array[BUFFERSIZE];
	};
extern struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(char *address,char r,char g,char b);
void sub_func_set_color(char *cmdPointer);

#include "include_files\spi.h"
#include "include_files\ledstrip.c"

#endif