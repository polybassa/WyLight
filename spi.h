#ifndef _SPI_H_
#define _SPI_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#include "ledstrip.h"

void spi_init();
char spi_send(char data);
void spi_send_arr(char *array, char length);
void spi_send_ledbuf(char *array);

#endif
