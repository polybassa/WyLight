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
void SPI_Init()
{
	ANSELC = 0;
	TRISC.3 = 0;        // Make port RC3 an output(SPI Clock)
	TRISC.4 = 1;        // Make port RC4 an input(SPI Data In)
	TRISC.5 = 0;        // Make port RC5 an output(SPI Data Out)
	//SSP1STAT = 0b11000000;
	SMP = 1;
	CKP = 0;
	CKE = 1;
	SSP1CON1 = 0b00100000;
	//SSP1CON1 &= 0x02;	
	SSPEN = 1;
}

char SPI_Send(char data)
{
	SSP1BUF = data;	
	while(SSP1IF == 0);
	
	return SSP1BUF;
}

/***
**	This function sends the array to the LED controller(WS2801)
**  it starts with the last byte to get a correct output
***/
void SPI_SendArray(char *array, char length)
{
	if(array == 0) return;
	char i;
	for(i = length; i == 0; i-- )
	{
		SPI_Send(array[i]);
	} 
}

void SPI_SendLedBuffer(uns8 *array)
{
	//array must be the address of the first byte
	char* end;
	//calculate where the end is
	end = array + (NUM_OF_LED * 3);
	//send all
	
	for(; array < end; array++)
	{
		SPI_Send(*array);
	}
/* If we really have to garantee a sleep after data was written to the LEDs, it should be added here.
 * Other locations would be more attractive to avoid a waiting core, but here it is much clearer and easier
 * to find for later optimization. In my opinion we should spend this 1ms waste here, before we make the main
 * loop more complex. */
}
#endif /* #ifndef X86 */


