#ifndef _LEDSTRIP_H_
#define _LEDSTRIP_H_

//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x

#include "commandstorage.h"
#include "spi.h"
#include "eeprom.h"

#define NUM_OF_LED 32

struct LedBuffer{
	uns8 led_array[NUM_OF_LED*3];			// contains the current colorvalue for each led
	uns8 delta[NUM_OF_LED*3];			// delta until new fade color arrived, decremented each periode
	unsigned short cyclesLeft[NUM_OF_LED*3];	// cycles left in current periode
	unsigned short periodeLength[NUM_OF_LED*3];	// number of cycles in one periode
	uns8 step[NUM_OF_LED / 8* 3]; // if bit is set led_array is decremented each periode else incremented
};

extern bank1 struct LedBuffer gLedBuf;

void ledstrip_init(void);
void ledstrip_set_color(struct cmd_set_color *pCmd);
void ledstrip_set_fade(struct cmd_set_fade *pCmd);
void ledstrip_do_fade(void);
void ledstripe_update_fade(void);
#endif
