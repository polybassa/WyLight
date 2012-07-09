//Nils Weiß, Patrick Brünn
//20.04.2012
//Compiler CC5x 

#include "ledstrip.h"
#include "timer.h"


#define INC_BIT_COUNTER(PTR, MASK) \
	MASK = MASK << 1; \
	if(0 == MASK) { \
		PTR++; \
		MASK = 0x01; \
	}

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

#define FOR_EACH_MASKED_LED_DO(BLOCK) { \
	uns8 *address = pCmd->addr; \
	char k,mask; \
	mask = 0x01; \
	for(k = 0; k < (NUM_OF_LED * 3); k++) {	\
		if(0 != (*address & mask)) { \
			BLOCK \
		} else { \
			k++; k++; \
		} \
		INC_BIT_COUNTER(address, mask); \
	} \
}

#define CALC_COLOR \
		oldColor = gLedBuf.led_array[k]; \
		if(oldColor > newColor) { \
			delta = oldColor - newColor; \
			SET_BIT_AT(gLedBuf.step, k); \
		} else { \
			delta = newColor - oldColor; \
			CLEAR_BIT_AT(gLedBuf.step, k); \
		} \
		gLedBuf.cyclesLeft[k] = 0; \
		gLedBuf.periodeLength[k] = 0; \
		gLedBuf.delta[k] = delta; \
		if((0 != delta)) {\
			timevalue = 1000 * pCmd->timevalue; \
			gLedBuf.periodeLength[k] = timevalue / delta; \
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
			gLedBuf.led_array[k] = b;
			k++;
			gLedBuf.led_array[k] = g;
			k++;
			gLedBuf.led_array[k] = r;
	);
	spi_send_ledbuf(gLedBuf.led_array);
}

void ledstrip_do_fade(void)
{
	char step;
	uns8 k, stepmask;
	uns8* stepaddress = gLedBuf.step;
	stepmask = 0x01;
	unsigned short periodeLength;
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{
		//active and triggered?
		if((gLedBuf.delta[k] > 0) && (gLedBuf.cyclesLeft[k] == 0))
		{
			//reset timer
			gLedBuf.delta[k]--;
			periodeLength = gLedBuf.periodeLength[k];
			gLedBuf.cyclesLeft[k] = periodeLength;

			if(GET_BIT_AT(gLedBuf.step, k)) {
				gLedBuf.led_array[k]--;
			} else {
				gLedBuf.led_array[k]++;
			}
		}
		INC_BIT_COUNTER(stepaddress, stepmask); \
	}
	//send LED status
	spi_send_ledbuf(gLedBuf.led_array);
}

void ledstrip_set_fade(struct cmd_set_fade *pCmd)
{
	uns8 k, stepmask;
	uns8 delta, timevalue;
	uns8 oldColor, newColor;
	uns8* stepaddress = gLedBuf.step;
	for(k = 0; k < NUM_OF_LED*3; k++) {
		gLedBuf.delta[k] = 0;
		gLedBuf.cyclesLeft[k] = 0;
	}
	for(k = 0; k < sizeof(gLedBuf.step); k++) {
		gLedBuf.step[k] = 0;
	}
	stepmask = 0x01;
	FOR_EACH_MASKED_LED_DO(
		newColor = pCmd->blue;
		CALC_COLOR;
		k++;
		newColor = pCmd->green;
		CALC_COLOR;
		k++;
		newColor = pCmd->red;
		CALC_COLOR;
	);
}

void ledstripe_update_fade(void)
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

