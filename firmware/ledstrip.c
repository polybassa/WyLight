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

struct LedBuffer gLedBuf;
struct cmd_set_fade mFade;


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
		for(k = 0; k < sizeof(gLedBuf.led_array); k++) {        \
			if(0 != (*address & mask)) { \
				BLOCK \
			} \
			else { \
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
	{ \
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
		stepSize = 0x01; \
		temp16 = 0; \
		if((0 != delta))  \
		{ \
			do { \
				temp8 = delta / stepSize; \
				temp16 = fadeTmms / temp8;  \
				if(temp16 < 1) { \
					stepSize += 1; } \
			} \
			while(temp16 < 1); \
		} \
		gLedBuf.stepSize[k] = stepSize; \
		gLedBuf.delta[k] = delta; \
		gLedBuf.periodeLength[k] = temp16;  \
		gLedBuf.cyclesLeft[k] = temp16;  \
	};

void Ledstrip_FadeOffLeds(void)
{
	//check current status of led
	mFade.addr[0] = 0xff;
	mFade.addr[1] = 0xff;
	mFade.addr[2] = 0xff;
	mFade.addr[3] = 0xff;
	mFade.fadeTmms = htons(200);

	mFade.red = 0x00;
	mFade.green = 0x00;
	mFade.blue = 0x00;

	Ledstrip_SetFade(&mFade);
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
	}
	while(0 != i);
	/*-------------------------------------*/
	i = sizeof(gLedBuf.delta);
	do {
		i--;
		gLedBuf.delta[i] = 0;
	}
	while(0 != i);
	/*-------------------------------------*/
	i = sizeof(gLedBuf.cyclesLeft);
	do {
		i--;
		gLedBuf.cyclesLeft[i] = 0;
	}
	while(0 != i);
	/*-------------------------------------*/
	i = sizeof(gLedBuf.periodeLength);
	do {
		i--;
		gLedBuf.periodeLength[i] = 0;
	}
	while(0 != i);
	/*-------------------------------------*/
	i = sizeof(gLedBuf.step);
	do {
		i--;
		gLedBuf.step[i] = 0;
	}
	while(0 != i);
	/*-------------------------------------*/
	i = sizeof(gLedBuf.stepSize);
	do {
		i--;
		gLedBuf.stepSize[i] = 0;
	}
	while(0 != i);

	gLedBuf.fadeTmms = 0;
}

void Ledstrip_SetColorDirect(uns8 *pValues)
{
	uns8 k, red, green, blue;
	for(k = 0; k < sizeof(gLedBuf.led_array); ) {
		red = *pValues;
		++pValues;
		green = *pValues;
		++pValues;
		blue = *pValues;
		++pValues;
		gLedBuf.led_array[k] = blue;
		gLedBuf.cyclesLeft[k] = 0;
		gLedBuf.delta[k] = 0;
		++k;
		gLedBuf.led_array[k] = green;
		gLedBuf.cyclesLeft[k] = 0;
		gLedBuf.delta[k] = 0;
		++k;
		gLedBuf.led_array[k] = red;
		gLedBuf.cyclesLeft[k] = 0;
		gLedBuf.delta[k] = 0;
		++k;
	}
}

void Ledstrip_DoFade(void)
{
	uns8 k, stepmask, stepSize;
	uns8 *stepaddress = gLedBuf.step;
	stepmask = 0x01;
	uns16 periodeLength;

	/* Update cyclesLeft Value for all LED's */

	for(k = 0; k < sizeof(gLedBuf.delta); k++) {
		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] > 0)) {
			gLedBuf.cyclesLeft[k]--;
		}
	}

	for(k = 0; k < sizeof(gLedBuf.delta); k++) {
		// fade active on this led and current periode is over?
		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] == 0)) {
			stepSize = gLedBuf.stepSize[k];
			// reset cycle counters
			if(gLedBuf.delta[k] < stepSize) {
				stepSize = gLedBuf.delta[k];
				gLedBuf.delta[k] = 0;
			} else {
				gLedBuf.delta[k] -= stepSize;
			}
			periodeLength = gLedBuf.periodeLength[k];
			gLedBuf.cyclesLeft[k] = periodeLength;

			// update rgb value by one step
			if(0 != ((*stepaddress) & stepmask)) {
				gLedBuf.led_array[k] -= stepSize;
			} else {
				gLedBuf.led_array[k] += stepSize;
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

	uns8 *stepAddress = gLedBuf.step;
	uns8 stepMask = 0x01;
	uns16 temp16;
	uns8 red,green,blue,delta,stepSize,temp8;

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
			k++; k++;
			INC_BIT_COUNTER(stepAddress, stepMask);
			INC_BIT_COUNTER(stepAddress, stepMask);
			INC_BIT_COUNTER(stepAddress, stepMask);
		}
		);
}

#define CALC_DELTA(target,source_1,source_2) { \
		target = source_1; \
		if(target > source_2) \
			target = target - source_2; \
		else \
			target = source_2 - target; \
		target = target / numOfLeds; }

// To add or sub the diff from color by each loop run to get the right color for
// every led. If compare is greater then color, this macro add's diff, otherwise it sub's diff
#define ADJUST_COLOR(color,compare,diff) { \
		if(color > compare) \
			color -= diff; \
		else \
			color += diff; } \

void Ledstrip_SetGradient(struct cmd_set_gradient *pCmd)
{
	const uns16 fadeTmms = ntohs(pCmd->fadeTmms);

	uns8 offset = pCmd->parallelAndOffset & 0x7f;
	uns8 numOfLeds = pCmd->numberOfLeds - 1;
	uns8 deltaRed, deltaGreen, deltaBlue;

	if(numOfLeds == 255 || numOfLeds == 0)
		numOfLeds = 1;

	CALC_DELTA(deltaRed,   pCmd->red_1,   pCmd->red_2);
	CALC_DELTA(deltaGreen, pCmd->green_1, pCmd->green_2);
	CALC_DELTA(deltaBlue,  pCmd->blue_1,  pCmd->blue_2);

	uns8 red = pCmd->red_1;
	uns8 green = pCmd->green_1;
	uns8 blue = pCmd->blue_1;

	//define variables for CALC_COLOR macro
	uns16 temp16;
	uns8 k,delta,stepSize,temp8;
	uns8 *stepAddress = gLedBuf.step;
	uns8 stepMask = 0x01;

	offset = offset * 3;
	numOfLeds = numOfLeds * 3;

	const uns8 endPosition = offset + numOfLeds;

	for(k = 0; k < NUM_OF_LED * 3; k++) {
		if(k >= endPosition) {
			red = pCmd->red_2;
			green = pCmd->green_2;
			blue = pCmd->blue_2;

			CALC_COLOR(blue);
			k++;
			CALC_COLOR(green);
			k++;
			CALC_COLOR(red);
			break;
		}

		if(k >= offset) {
			CALC_COLOR(blue);
			k++;
			CALC_COLOR(green);
			k++;
			CALC_COLOR(red);
			ADJUST_COLOR(red,   pCmd->red_2,   deltaRed);
			ADJUST_COLOR(green, pCmd->green_2, deltaGreen);
			ADJUST_COLOR(blue,  pCmd->blue_2,  deltaBlue);
		} else
			INC_BIT_COUNTER(stepAddress, stepMask);
	}
}

#ifdef DEBUG
#ifndef __CC8E__
void Ledstrip_Test(unsigned char address)
{
	unsigned int cur = 0x1;
	uns8 color;
	uns8 i;
	for(i = 0; i < NUM_OF_LED; i++) {
		if(address & cur) {
			color = 0xff;
		} else {
			color = 0;
		}
		gLedBuf.led_array[i] = color;
		gLedBuf.cyclesLeft[i] = 0;
		gLedBuf.delta[i] = 0;
		i++;
		gLedBuf.led_array[i] = color;
		gLedBuf.cyclesLeft[i] = 0;
		gLedBuf.delta[i] = 0;
		i++;
		gLedBuf.led_array[i] = color;
		gLedBuf.cyclesLeft[i] = 0;
		gLedBuf.delta[i] = 0;
		cur = cur << 1;
	}
	Ledstrip_UpdateLed();
}
#endif
#endif

