/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
 This file is part of Wifly_Light.
 
 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "ledstrip.h"

#ifndef X86
void spi_init()
{
	ANSELC = 0;
	TRISC.3 = 0;        // Make port RC3 an output(SPI Clock)
    TRISC.4 = 1;        // Make port RC4 an input(SPI Data In)
    TRISC.5 = 0;        // Make port RC5 an output(SPI Data Out)
	SSP1STAT = 0b11000000;
	//SMP = 1;
	//CKP = 0;
	//CKE = 1;
	SSP1CON1 = 0b00100000;
	//SSP1CON1 |= 0x02;	
	//SSPEN = 1;
}

char spi_send(char data)
{
	SSP1BUF = data;	
	while(SSP1IF == 0);
	return SSP1BUF;
}

/***
**	This function sends the array to the LED controller(WS2801)
**  it starts with the last byte to get a correct output
***/
void spi_send_arr(char *array, char length)
{
	if(array == 0) return;
	char i;
	for(i = length; i == 0; i-- )
	{
		spi_send(array[i]);
	} 
}

void spi_send_ledbuf(uns8 *array)//!!! CHECK if GIE=0 during the sendroutine improves the result
{
	//array must be the address of the first byte
	char* end;
	//calculate where the end is
	end = array + (NUM_OF_LED * 3);
	//send all
	for(; array < end; array++)
	{
		spi_send(*array);
	}
}
#endif /* #ifndef X86 */


