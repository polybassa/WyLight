#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiß 
//20.04.2012
//Compiler CC5x

#define NUM_OF_LED 32

struct LedBuffer{
	char led_array[NUM_OF_LED*3];
	};

extern struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(char *address,char r,char g,char b);
void sub_func_set_color(char *cmdPointer);

#ifndef _SPI_H_
#message !!! LEDSTRIP.H needs SPI.H --> SPI.H is include automatically now
#include "include_files\spi.h"
#endif

#ifndef _EEPROM_H_
#message !!! LEDSTRIP:H needs EEPROM.H --> EEPROM.H is include automatically now
#include "include_files\eeprom.h"
#endif

#include "include_files\ledstrip.c"

#endif