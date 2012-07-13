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

#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

#include "commandstorage.h"
#include "spi.h"
#include "eeprom.h"

#define NUM_OF_LED 24

/**
 * This structure is used for calculations to manipulate the ledstrip state
 *
 * <led_array> contains the current colorvalue for each led
 * <delta> delta until new fade color arrived, decremented each periode
 * <cyclesLeft> number of cycles until this periode is over, decremented each cycle
 * <periodeLength> number of cycles in each periode, cyclesLeft is reset to this value, each periode
 * <step> bitmask, if bit is set <led_array> is decremented each periode, if cleared incremented
 * <stepSize> <led_array> is decremented/incremented by this value each periode
 */
struct LedBuffer{
	uns8 led_array[NUM_OF_LED*3];
	uns8 delta[NUM_OF_LED*3];
	unsigned short cyclesLeft[NUM_OF_LED*3];
	unsigned short periodeLength[NUM_OF_LED*3];
	uns8 step[NUM_OF_LED / 8* 3];
	uns8 stepSize[NUM_OF_LED * 3];
};
extern bank1 struct LedBuffer gLedBuf;

/**
 * Initialize the ledstrip and all associated variables
 */
void ledstrip_init(void);

/**
 * Callback if a "set_color" command is received.
 * ledstrip is updated according to the provided cmd_set_color
 * only Led's where the address bit is 1 will be set to the new color
 */
void ledstrip_set_color(struct cmd_set_color *pCmd);

/**
 * Callback if a "set_fade" command is received.
 * fading parameters are calculated and stored to be used in
 * ledstrip_do_fade() which is called in the main cycle
 */
void ledstrip_set_fade(struct cmd_set_fade *pCmd);

/**
 * called by the main cycle
 * update the ledstrip accourding to the precalculated parameters in <gLedBuf>
 */
void ledstrip_do_fade(void);

/**
 * callback for the fadecycle timer
 * updates cyclesLeft part of the global <gLedBuf>
**/
void ledstrip_update_fade(void);
#endif
