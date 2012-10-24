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
#include "ScriptCtrl.h"
#include "spi.h"

#ifdef __CC8E__
#include "MATH16.H"
#endif /* #ifdef __CC8E__ */

struct LedBuffer gLedBuf;

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

uns8 Ledstrip_NumOfFades(void)
{
	uns8 counter = 0;
	uns8 i;
	
	for(i = 0; i < NUM_OF_LED * 3; i++)
	{
		if(gLedBuf.delta[i] > 0)
		{
			counter += 1;
		}
	}
	return counter;
}


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
	
	i = sizeof(gLedBuf.step);
	do {
		i--;
		gLedBuf.step[i] = 0;
	} while(0 != i);
	
	gLedBuf.fadeTmms = 0;
	gLedBuf.flags.run_aktiv = 0;
	gLedBuf.flags.run_direction = 0;
}

void Ledstrip_SetColor(struct cmd_set_color *pCmd)
{
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
}

void Ledstrip_SetColorDirect(uns8 *pValues)
{
	uns8 k, temp;
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{
		temp = *pValues;
		gLedBuf.led_array[k] = temp;
		pValues += 1;
		gLedBuf.cyclesLeft[k] = 0;
		gLedBuf.delta[k] = 0;
	}
}

void Ledstrip_DoFade(void)
{
	uns8 k, stepmask;
	uns8* stepaddress = gLedBuf.step;
	stepmask = 0x01;
	uns16 periodeLength;
	
	/* Update cyclesLeft Value for all LED's */
	
	for(k = 0; k < NUM_OF_LED * 3; k++)
	{
		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] > 0))
		{
			gLedBuf.cyclesLeft[k]--;	
		}
	}
	
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
	
}

void Ledstrip_UpdateLed(void)
{
	SPI_SendLedBuffer(gLedBuf.led_array);
}

void Ledstrip_SetFade(struct cmd_set_fade *pCmd)
{
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
}

void Ledstrip_SetRun(struct cmd_set_run *pCmd)
{
	gLedBuf.fadeTmms = ntohs(pCmd->fadeTmms);
	
	if(pCmd->direction > 0)
	{
		gLedBuf.flags.run_direction = 1;
	}
	else
	{
		gLedBuf.flags.run_direction = 0;
	}
	
	gLedBuf.flags.run_aktiv = 1;
}

void Ledstrip_UpdateRun(void)
{
	if(gLedBuf.flags.run_aktiv == FALSE)
	{
		return;
	}
	/* terminate run when wait is stopped */
	if(gScriptBuf.waitValue == 0)
	{
		gLedBuf.flags.run_aktiv = FALSE;
		return;
	}

	if(Ledstrip_NumOfFades() > 0)
	{
		return;
	}
		
	const uns16 fadeTmms = gLedBuf.fadeTmms;
	uns8* stepAddress = gLedBuf.step;
	uns8 stepMask = 0x01;
	uns16 temp16;
	uns8 red, green, blue, delta, k, temp_red, temp_green, temp_blue;
	
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{
		if(gLedBuf.flags.run_direction == 1) //rechts
		{
			if(k < 3)
			{
				temp_blue = gLedBuf.led_array[0];
				temp_green = gLedBuf.led_array[1];
				temp_red = gLedBuf.led_array[2];
				blue = gLedBuf.led_array[k + 3];
				green = gLedBuf.led_array[k + 4];
				red = gLedBuf.led_array[k + 5];
			}
			else if( (k + 3) == (NUM_OF_LED * 3) )
			{
				blue = temp_blue;
				green = temp_green;
				red = temp_red;
			}
			else
			{
				blue = gLedBuf.led_array[k + 3];
				green = gLedBuf.led_array[k + 4];
				red = gLedBuf.led_array[k + 5];
			}
		}
		else
		{
			if(k < 3)
			{
				temp_blue = gLedBuf.led_array[0];
				temp_green = gLedBuf.led_array[1];
				temp_red = gLedBuf.led_array[2];
				blue = gLedBuf.led_array[(NUM_OF_LED * 3) - 3];
				green = gLedBuf.led_array[(NUM_OF_LED * 3) - 2];
				red = gLedBuf.led_array[(NUM_OF_LED * 3) - 1];
			}
			else if( (k + 3) == (NUM_OF_LED * 3) )
			{
				blue = temp_blue;
				green = temp_green;
				red = temp_red;
			}
			else
			{
				blue = temp_blue;
				green = temp_green;
				red = temp_red;
				temp_blue = gLedBuf.led_array[k];
				temp_green = gLedBuf.led_array[k + 1];
				temp_red = gLedBuf.led_array[k + 2];
			}
		}
		CALC_COLOR(blue);
		k++;
		CALC_COLOR(green);
		k++;
		CALC_COLOR(red);
	}
}

