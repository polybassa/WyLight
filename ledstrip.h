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

#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

#include "commandstorage.h"
#include "spi.h"
#include "eeprom.h"

#define NUM_OF_LED 24//32

struct LedBuffer{
	uns8 led_array[NUM_OF_LED*3];			// contains the current colorvalue for each led
	uns8 delta[NUM_OF_LED*3];			// delta until new fade color arrived, decremented each periode
	unsigned short cyclesLeft[NUM_OF_LED*3];	// cycles left in current periode
	unsigned short periodeLength[NUM_OF_LED*3];	// number of cycles in one periode
	uns8 step[NUM_OF_LED / 8* 3]; // if bit is set led_array is decremented each periode else incremented
	uns8 stepSize[NUM_OF_LED * 3]; // if bit is set led_array is decremented each periode else incremented
};

extern bank1 struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(struct cmd_set_color *pCmd);
void ledstrip_set_fade(struct cmd_set_fade *pCmd);
void ledstrip_do_fade(void);
void ledstrip_update_fade(void);
#endif
