/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
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
#include "spi.h"

#ifdef __CC8E__
#include "MATH16.H"
#endif /* #ifdef __CC8E__ */

bank1 struct LedBuffer gLedBuf;

/**
 * Since we often work with a rotating bitmask which is greater
 * than 1 byte we use this macro to keep the mask and the bitfield
 * in sync.
 */
#define INC_BIT_COUNTER(PTR, MASK) { \
	MASK <<= 1; \
	if(0 == MASK) { \
		PTR++; \
		MASK = 0x01; \
	} \
}

/**
 * This macro is used to iterate over each led and each color.
 * <BLOCK> is executed if the led color was selected in <pCmd->addr>
 * <ELSE> is executed if not
 */
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

/**
 * This is a sub-macro of <FOR_EACH_MASKED_LED_DO> used in fade precalculations
 * to calculate the fading parameters(<periodeLength>, <stepSize> and <delta>) for <newColor>
**/
#define CALC_COLOR(newColor)  \
		{\
		delta = gLedBuf.led_array[k]; \
		if(delta > newColor)  \
		{ \
			delta = delta - newColor;  \
			*(stepAddress) |= (stepMask);  \
		} \
		else  \
		{  \
			delta = newColor - delta;  \
			*(stepAddress) &= ~(stepMask); \
		}  \
		INC_BIT_COUNTER(stepAddress, stepMask); \
		temp16 = 0; \
		if((0 != delta))  \
		{ \
			temp16 = fadeTmms / delta;  \
			gLedBuf.periodeLength[k] = temp16;  \
		} \
		if((temp16 == 0))  \
		{ \
			gLedBuf.led_array[k] = newColor;  \
			gLedBuf.delta[k] = 0; \
		}  \
		else  \
		{  \
			gLedBuf.delta[k] = delta; \
		}  \
		gLedBuf.cyclesLeft[k] = temp16;  \
};

void Ledstrip_Init(void)
{
	// initialize interface to ledstrip
	SPI_Init();
	
	// initialize variables
	uns8 i = sizeof(gLedBuf.led_array);
	do {
		i--;
		gLedBuf.led_array[i] = 0;
	} while(0 != i);
	i = sizeof(gLedBuf.delta);
	do {
		i--;
		gLedBuf.delta[i] = 0;
	} while(0 != i);
	i = sizeof(gLedBuf.cyclesLeft);
	do {
		i--;
		gLedBuf.cyclesLeft[i] = 0;
	} while(0 != i);
	i = sizeof(gLedBuf.periodeLength);
	do {
		i--;
		gLedBuf.periodeLength[i] = 0;
	} while(0 != i);
}

void Ledstrip_SetColor(struct cmd_set_color *pCmd)
{
	gLedBuf.processing_of_data = TRUE;
	uns8 r = pCmd->red;
	uns8 g = pCmd->green;
	uns8 b = pCmd->blue;
	
	FOR_EACH_MASKED_LED_DO(
		{
			gLedBuf.led_array[k] = b;
			gLedBuf.cyclesLeft[k] = 0;
			gLedBuf.delta[k] = 0;
			k++;
			gLedBuf.led_array[k] = g;
			gLedBuf.cyclesLeft[k] = 0;
			gLedBuf.delta[k] = 0;
			k++;
			gLedBuf.led_array[k] = r;
			gLedBuf.cyclesLeft[k] = 0;
			gLedBuf.delta[k] = 0;
		},
		{
			k++;k++;
		}
	);
	gLedBuf.processing_of_data = FALSE;
}

void Ledstrip_DoFade(void)
{
	if(gLedBuf.processing_of_data)
	{
		return;
	}
	gLedBuf.processing_of_data = TRUE;
	
	uns8 k, stepmask;
	uns8* stepaddress = &gLedBuf.step[0];
	stepmask = 0x01;
	uns16 periodeLength;
	
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{
		// fade active on this led and current periode is over?
		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] == 0))
		{
			// reset cycle counters
			gLedBuf.delta[k]--;
			periodeLength = gLedBuf.periodeLength[k];
			gLedBuf.cyclesLeft[k] = periodeLength;

			// update rgb value by one step
			if(0 != ((*stepaddress) & stepmask)) {
				gLedBuf.led_array[k] -= 1;
			} else {
				gLedBuf.led_array[k] += 1;
			}
		}
		INC_BIT_COUNTER(stepaddress, stepmask);
	}

	// write changes to ledstrip
	SPI_SendLedBuffer(gLedBuf.led_array);
	gLedBuf.processing_of_data = FALSE;
}

void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
{
	gLedBuf.processing_of_data = TRUE;
	
	// constant for this fade used in CALC_COLOR
	const uns16 fadeTmms = ntohs(pCmd->fadeTmms);

	uns8* stepAddress = gLedBuf.step;
	uns8 stepMask = 0x01;
	uns16 temp16;
	uns8 red,green,blue,delta;
	
	red = pCmd->red;
	green = pCmd->green;
	blue = pCmd->blue;
	// calc fade parameters for each led
	FOR_EACH_MASKED_LED_DO(
		{
			CALC_COLOR(blue);
			k++;
			CALC_COLOR(green);
			k++;
			CALC_COLOR(red);
		},
		{
			// if led is not fade, we have to increment our pointers and rotate the mask
			k++;k++;
			INC_BIT_COUNTER(stepAddress, stepMask);
			INC_BIT_COUNTER(stepAddress, stepMask);
			INC_BIT_COUNTER(stepAddress, stepMask);
		}
	);
	gLedBuf.processing_of_data = FALSE;
}

void Ledstrip_UpdateFade(void)
{
	if(gLedBuf.processing_of_data)
	{
		return;
	}
	gLedBuf.processing_of_data = TRUE;
	uns8 i;
	for(i = 0; i < NUM_OF_LED * 3; i++)
	{
		if((gLedBuf.delta[i] > 0) && (gLedBuf.cyclesLeft[i] > 0))
		{
			gLedBuf.cyclesLeft[i]--;		
		}
	}
	gLedBuf.processing_of_data = FALSE;
}

