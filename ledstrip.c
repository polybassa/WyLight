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
#define CALC_COLOR(newColor) { \
		delta = gLedBuf.led_array[k]; \
		if(delta > newColor) { \
			delta -= newColor; \
			*(stepAddress) |= (stepMask); \
		} else { \
			delta = newColor - delta; \
		}; \
		INC_BIT_COUNTER(stepAddress, stepMask); \
		gLedBuf.cyclesLeft[k] = 0; \
		if((0 != delta)) {\
			temp16 = (uns16)delta * CYCLE_TMMS; \
			if(fadeTmms >= temp16) { \
				gLedBuf.periodeLength[k] = fadeTmmsPerCycleTmms / delta; \
				gLedBuf.stepSize[k] = 1; \
				gLedBuf.delta[k] = delta; \
			} else { \
				gLedBuf.periodeLength[k] = 1; \
				gLedBuf.stepSize[k] = temp16 / fadeTmms; \
				gLedBuf.delta[k] = fadeTmms / CYCLE_TMMS; \
			} \
		} \
};

void ledstrip_init(void)
{
	memset(gLedBuf.led_array, 0, sizeof(gLedBuf.led_array));
}

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
		// fade active on this led and triggered?
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
	// constant for this fade used in CALC_COLOR
	const uns16 fadeTmms = ntohs(pCmd->fadeTmms);
	const uns16 fadeTmmsPerCycleTmms = fadeTmms / CYCLE_TMMS;

	memset(gLedBuf.delta, 0, sizeof(gLedBuf.delta));
	memset(gLedBuf.step, 0, sizeof(gLedBuf.step));

	uns8 delta;
	uns16 temp16;
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
			// even if nothing to do, we have to increment all of our bitmask pointers and rotate the masks
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

