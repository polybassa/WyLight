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


#pragma codepage 1

#define INC_BIT_COUNTER(PTR, MASK) \
	MASK = MASK << 1; \
	if(0 == MASK) { \
		PTR++; \
		MASK = 0x01; \
	}

#define CLEAR_FLAGS(PTR, MASK) \
	*(PTR) &= ~(MASK); \

#define SET_FLAGS(PTR, MASK) \
	*(PTR) |= (MASK); \

void SET_BIT_AT(uns8* PTR, uns8 POSITION) {
	uns8 bytePos = (POSITION) / 8; \
	uns8 bitPos = (POSITION) - (bytePos * 8); \
	uns8 value = PTR[bytePos]; \
	uns8 mask = (1 << bitPos); \
	value = value | mask; \
	PTR[bytePos] = value; \
	}

void CLEAR_BIT_AT(uns8* PTR, uns8 POSITION) {
	uns8 bytePos = (POSITION) / 8; \
	uns8 bitPos = (POSITION) - (bytePos * 8); \
	uns8 value = PTR[bytePos]; \
	uns8 mask = (1 << bitPos); \
	mask = ~mask; \
	value = value & mask; \
	PTR[bytePos] = value; \
	}

uns8 GET_BIT_AT(uns8* PTR, uns8 POSITION) {
	uns8 bytePos = (POSITION) / 8;
	uns8 bitPos = (POSITION) - (bytePos * 8);
	uns8 value = PTR[bytePos];
	uns8 mask = (1 << bitPos);
	return value & mask;
	}

#define FOR_EACH_MASKED_LED_DO(BLOCK, ELSE) { \
	uns8 *address = pCmd->addr; \
	uns8 k,mask; \
	mask = 0x01; \
	for(k = 0; k < (NUM_OF_LED * 3); k++) {	\
		if(0 != (*address & mask)) { \
			BLOCK \
		} else { \
			ELSE \
		} \
		INC_BIT_COUNTER(address, mask); \
	} \
}

#define CALC_COLOR(newColor) \
		delta = gLedBuf.led_array[k]; \
		if(delta > newColor) { \
			delta -= newColor; \
			SET_FLAGS(stepAddress, stepMask); \
		} else { \
			delta = newColor - delta; \
			CLEAR_FLAGS(stepAddress, stepMask); \
		} \
		INC_BIT_COUNTER(stepAddress, stepMask); \
		gLedBuf.cyclesLeft[k] = 0; \
		if((0 != delta)) {\
			temp16 = delta * CYCLE_TMMS; \
			if(fadeTmms >= temp16) { \
				gLedBuf.periodeLength[k] = fadeTmmsPerCycleTmms / delta; \
				gLedBuf.stepSize[k] = 1; \
				gLedBuf.delta[k] = delta; \
			} else { \
				gLedBuf.periodeLength[k] = 1; \
				temp16 = temp16 / fadeTmms; \
				gLedBuf.stepSize[k] = temp16; \
				gLedBuf.delta[k] = delta / temp16; \
			} \
		} \

void ledstrip_init(void)
{
	uns8 k = (NUM_OF_LED * 3) - 1;
	do {	
		gLedBuf.led_array[k--] = 0;
	} while(k != 0);
}

/***
*** This funktion sets the values of the global LedBuffer
*** only Led's where the address bit is 1 will be set to the new color
***/
void ledstrip_set_color(struct cmd_set_color *pCmd)
{
	char r = pCmd->red;
	char g = pCmd->green;
	char b = pCmd->blue;
	FOR_EACH_MASKED_LED_DO(
		{
			gLedBuf.led_array[k] = b;
			k++;
			gLedBuf.led_array[k] = g;
			k++;
			gLedBuf.led_array[k] = r;
		},
		{
			k++;k++;
		}
	);
	spi_send_ledbuf(gLedBuf.led_array);
}

void ledstrip_do_fade(void)
{
	uns8 k, stepmask;
	uns8* stepaddress = gLedBuf.step;
	stepmask = 0x01;
	unsigned short periodeLength;
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{
		//active and triggered?
		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] == 0))
		{
			uns8 stepSize = gLedBuf.stepSize[k];

			//reset timer
			gLedBuf.delta[k]--;
			periodeLength = gLedBuf.periodeLength[k];
			gLedBuf.cyclesLeft[k] = periodeLength;

			if(0 != ((*stepaddress) & stepmask)) {
				gLedBuf.led_array[k] -= stepSize;
			} else {
				gLedBuf.led_array[k] += stepSize;
			}
		}
		INC_BIT_COUNTER(stepaddress, stepmask);
	}
	//send LED status
	spi_send_ledbuf(gLedBuf.led_array);
}

void ledstrip_set_fade(struct cmd_set_fade *pCmd)
{
	uns8 k;
	uns8 delta;
	uns16 temp16;
	const uns16 fadeTmms = ntohs(pCmd->fadeTmms);
	const uns16 fadeTmmsPerCycleTmms = fadeTmms / CYCLE_TMMS;

	for(k = 0; k < NUM_OF_LED*3; k++) {
		gLedBuf.delta[k] = 0;
	}

	for(k = 0; k < sizeof(gLedBuf.step); k++) {
		gLedBuf.step[k] = 0;
	}

	uns8* stepAddress = gLedBuf.step;
	uns8 stepMask;
	stepMask = 0x01;
	FOR_EACH_MASKED_LED_DO(
		{
			CALC_COLOR(pCmd->blue);
			k++;
			CALC_COLOR(pCmd->green);
			k++;
			CALC_COLOR(pCmd->red);
		},
		{
			k++;k++;
			INC_BIT_COUNTER(stepAddress, stepMask);
			INC_BIT_COUNTER(stepAddress, stepMask);
			INC_BIT_COUNTER(stepAddress, stepMask);
		}
	);
}

void ledstrip_update_fade(void)
{
	uns8 i;
	for(i = 0; i < NUM_OF_LED*3; i++)
	{
		if((gLedBuf.delta[i] > 0) && (gLedBuf.cyclesLeft[i] > 0))
		{
			gLedBuf.cyclesLeft[i]--;		
		}
	}
}

