#ifndef _SPI_H_
#define _SPI_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#ifndef NUM_OF_LED
#message !!! SPI_H needs the number of LEDs at void spi_send_ledbuf(char *array)
#message !!! NUM_OF_LED is set to 32 as default
#define NUM_OF_LED 32
#endif

void spi_init();
char spi_send(char data);
void spi_send_arr(char *array, char length);
void spi_send_ledbuf(char *array);

#include "include_files\spi.c"

#endif