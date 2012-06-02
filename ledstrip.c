//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x 

#include "ledstrip.h"
#include "MATH16.H"
//private function
void ledstrip_get_distance(char destinationvalue, char currentvalue,char* max_distance);

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
* This funktion sets the destination color configurates
* the timer 1. If the settings are done, the bit led_fade_operation is 1.
* To configurate the Timer the funktion calculates the maximal distance between
* two values to calculate the time to fade by the maximal changens of one LED
**/
void ledstrip_set_fade(struct cmd_set_fade *pCmd)
{
	char *address = pCmd->addr;
	char r = pCmd->red;
	char g = pCmd->green;
	char b = pCmd->blue;
	
	char k,mask,temp,max_distance;
	//max_distance = 0;
	mask = 0b00000001;
	
	for(k = 0; k < (NUM_OF_LED * 3); k++)
	{	
		if(0 != (*address & mask))
		{
#ifdef DEBUG
			USARTsend('1');
#endif
			temp = gLedBuf.led_array[k];
			gLedBuf.led_destination[k] = b;
			//ledstrip_get_distance(b,temp,&max_distance);
			k++;
			temp = gLedBuf.led_array[k];
			gLedBuf.led_destination[k] = g;
			//ledstrip_get_distance(g,temp,&max_distance);
			k++;
			temp = gLedBuf.led_array[k];
			gLedBuf.led_destination[k] = r;
			//ledstrip_get_distance(g,temp,&max_distance);
		}
		else 
		{ 
	#ifdef DEBUG
	USARTsend('0');
	#endif
	// To Do finde lˆsung wie zwei fadeoperationen parallel laufen kˆnnen
			temp = gLedBuf.led_array[k];
			gLedBuf.led_destination[k] = temp;
			k++;
			temp = gLedBuf.led_array[k];
			gLedBuf.led_destination[k] = temp;
			k++;
			temp = gLedBuf.led_array[k];
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
#ifdef DEBUG
	USARTsend_str("timevalue:");
	USARTsend_num(pCmd->timevalue,'#');
#endif
	timer_set_for_fade(pCmd->timevalue);
	gLedBuf.led_fade_operation = 1;
}

//This funktion calculates the distance between the current value of a Led and the 
//destination value of a Led. Distance meens the steps to change. After calculation, 
//check if there is a new maximal_value
void ledstrip_get_distance(char destinationvalue, char currentvalue,char* max_distance)
{
	char temp;
	if(destinationvalue > currentvalue)
		temp = destinationvalue - currentvalue;
	else
		temp = currentvalue - destinationvalue;
		
	if(temp > *max_distance) *max_distance = temp;
}

void ledstrip_do_fade()
{
	char fade_finish:1;
	fade_finish = 1;
	char temp_current,temp_destination;
	
	char i;
	for(i = 0; i < (NUM_OF_LED*3);i++)
	{
		temp_current = gLedBuf.led_array[i];
		temp_destination = gLedBuf.led_destination[i];
		if(temp_current > temp_destination)
		{
			gLedBuf.led_array[i] = --temp_current;
			fade_finish = 0;
		}
		else if(temp_current < temp_destination)	
		{
			gLedBuf.led_array[i] = ++temp_current;
			fade_finish = 0;
		}		
	}	
	if(fade_finish) 
	{
		gLedBuf.led_fade_operation = 0;
		#ifdef DEBUG
		USARTsend_str(" fade finish");
		#endif
	}
	else 
		spi_send_ledbuf(&gLedBuf.led_array[0]);
}