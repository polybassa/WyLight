#ifndef _SPI_H_
#define _SPI_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

void spi_init();
void spi_send(char data);
char spi_receive(char data);
void spi_send_arr(char *array, char length);

#include "include_files\spi.c"

#endif