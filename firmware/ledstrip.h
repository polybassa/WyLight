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

#include "platform.h"
#include "wifly_cmd.h"

#ifdef __cplusplus
extern "C" {
#endif

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
struct LedBuffer {
    uns8 led_array[NUM_OF_LED * 3];
    uns8 delta[NUM_OF_LED * 3];
    uns16 cyclesLeft[NUM_OF_LED * 3];
    uns16 periodeLength[NUM_OF_LED * 3];
    uns8 step[NUM_OF_LED / 8 * 3];
    uns8 stepSize[NUM_OF_LED * 3];
    uns16 fadeTmms;
    uns8 offset;
};

extern struct LedBuffer gLedBuf;

/**
 * Initialize the ledstrip and all associated variables
 */
void Ledstrip_Init(void);

/**
 * Callback if a "set_color_direct" command is received.
 * ledstrip is updated according to the provided values.
 * *pValues indicates the start of the Value-Array.
 * Length of the Array is always NUM_OF_LED * 3
 */
void Ledstrip_SetColorDirect(uns8* pValues);

/**
 * Callback if a "set_fade" command is received.
 * fading parameters are calculated and stored to be used in
 * Ledstrip_DoFade() which is called in the main cycle
 */
void Ledstrip_SetFade(struct cmd_set_fade* pCmd);

/**
 * Callback if a "set_gradient" command is received.
 * fading parameters are calculated and stored to be used in
 * Ledstrip_DoFade() which is called in the main cycle
 */
void Ledstrip_SetGradient(struct cmd_set_gradient* pCmd);

/**
 * Callback if a "set_move" command is received.
 */
void Ledstrip_SetMove(struct cmd_set_move* pCmd);

/**
 * called by the main cycle
 * update the ledstrip accourding to the precalculated parameters in <gLedBuf>
 */
/**
 * callback for the fadecycle timer
 * updates cyclesLeft part of the global <gLedBuf>
 **/
void Ledstrip_DoFade(void);

void Ledstrip_UpdateLed(void);

void Ledstrip_FadeOffLeds(void);

#ifdef DEBUG
#ifndef __CC8E__
void Ledstrip_Test(unsigned char address);
#endif
#endif

#ifdef __cplusplus
}
#endif
#endif
