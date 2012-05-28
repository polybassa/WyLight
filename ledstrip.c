//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x 

#include "ledstrip.h"
#include "MATH16.H"
//private function
int8 ledstrip_get_vect(char destinationvalue, char currentvalue);

void ledstrip_init(void)
{
	char k;
	for(k = 0;k < (NUM_OF_LED * 3); k++)
	{	
		gLedBuf.led_array[k] = 0;
	}
}

/***
*** This funktion sets the values of the global LedBuffer
*** only Led's where the address bit is 1 will be set to the new color
***/
void ledstrip_set_color(struct cmd_set_color *pCmd)
{	
	char *address = pCmd->addr;
	char r = pCmd->red;
	char g = pCmd->green;
	char b = pCmd->blue;
	
	char k,mask;
	mask = 0b00000001;
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{	
		if(0 != (*address & mask))
		{
			gLedBuf.led_array[k] = b;
			k++;
			gLedBuf.led_array[k] = g;
			k++;
			gLedBuf.led_array[k] = r;
		}
		else 
		{
			k++;
			k++;
		}
#ifdef X86
		mask = mask << 1;
		if(0 == mask)
#else
		RLF(mask,1);
		if(Carry == 1) 
#endif
		{
			address++;
			mask= 0b00000001;
		}
	}
	spi_send_ledbuf(&gLedBuf.led_array[0]);
}
/***
* This funktion sets the destination color, the vector and configurates
* the timer 1. If the settings are done, the bit led_fade_operation is 1.
**/
void ledstrip_set_fade(struct cmd_set_fade *pCmd)
{
	char *address = pCmd->addr;
	char r = pCmd->red;
	char g = pCmd->green;
	char b = pCmd->blue;
	timer_config(pCmd->timevalue);
	
	char k,mask,temp;
	mask = 0b00000001;
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{	
		if(0 != (*address & mask))
		{
			temp = gLedBuf.led_array[k];
			temp = ledstrip_get_vect(b,temp);
			gLedBuf.led_vector[k] = temp;
			gLedBuf.led_destination[k] = b;
			k++;
			temp = gLedBuf.led_array[k];
			temp = ledstrip_get_vect(g,temp);
			gLedBuf.led_vector[k] = temp;
			gLedBuf.led_destination[k] = g;
			k++;
			temp = gLedBuf.led_array[k];
			temp = ledstrip_get_vect(r,temp);
			gLedBuf.led_vector[k] = temp;
			gLedBuf.led_destination[k] = r;
		}
		else 
		{ 
			temp = gLedBuf.led_array[k];
			gLedBuf.led_vector[k] = 0;
			gLedBuf.led_destination[k] = temp;
			k++;
			temp = gLedBuf.led_array[k];
			gLedBuf.led_vector[k] = 0;
			gLedBuf.led_destination[k] = temp;
			k++;
			temp = gLedBuf.led_array[k];
			gLedBuf.led_vector[k] = 0;
			gLedBuf.led_destination[k] = temp;
		}
#ifdef X86
		mask = mask << 1;
		if(0 == mask)
#else
		RLF(mask,1);
		if(Carry == 1) 
#endif
		{
			address++;
			mask= 0b00000001;
		}
	}
	gLedBuf.led_fade_operation = 1;
}

//This funktion returns a value between 1 - 15 that indicates how fast the color
//from one led has to been changed during the fade operation
//In bearbeitung, muss noch ausgefeilt werden.
int8 ledstrip_get_vect(char destinationvalue, char currentvalue)
{
	char temp;
	if(destinationvalue > currentvalue)
	return temp = destinationvalue / currentvalue;
	else
	{
		temp = currentvalue / destinationvalue;
		return (-1 * temp);
	}

}

void ledstrip_do_fade()
{

}