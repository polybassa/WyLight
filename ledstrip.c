//Nils Weiß 
//20.04.2012
//Compiler CC5x 

#include "ledstrip.h"

#define smallestChange 16
//private struct
struct max_changes_struct
	{
		uns8 red;
		uns8 green;
		uns8 blue;
	};

//private function
uns8 ledstrip_get_change(uns8 destinationvalue, uns8 currentvalue);
void ledstrip_calc_change(uns8 *change_array,struct max_changes_struct *Changestruct);

#define FOR_EACH_MASKED_LED_DO(BLOCK) { \
	char *address = pCmd->addr; 		\
	char r = pCmd->red; 				\
	char g = pCmd->green;	 			\
	char b = pCmd->blue; 				\
	char k,mask; 						\
	mask = 0x01; 						\
	for(k = 0; k < (NUM_OF_LED * 3); k++) {	\
		if(0 != (*address & mask)) { 	\
			BLOCK 						\
		} else { 						\
			k++; k++; 					\
		} 								\
		mask = mask << 1; 				\
		if(0 == mask) { 				\
			address++; 					\
			mask = 0x01; 				\
		} 								\
	} 									\
}

void ledstrip_init(void)
{
	char k;
	for(k = 0;k < (NUM_OF_LED * 3); k++)
	{	
		gLedBuf.led_array[k] = 0;
		gLedBuf.led_destination[k] = 0;
		gLedBuf.led_fade_operation = 0;
		gLedBuf.led_run_operation = 0;
	}
}

/***
*** This funktion sets the values of the global LedBuffer
*** only Led's where the address bit is 1 will be set to the new color
***/
void ledstrip_set_color(struct cmd_set_color *pCmd)
{
	FOR_EACH_MASKED_LED_DO(
			gLedBuf.led_array[k] = b;
			gLedBuf.led_destination[k] = b;
			k++;
			
			gLedBuf.led_array[k] = g;
			gLedBuf.led_destination[k] = g;
			k++;
			
			gLedBuf.led_array[k] = r;
			gLedBuf.led_destination[k] = r;
	);
	spi_send_ledbuf(&gLedBuf.led_array[0]);
	// Disable other functions
	gLedBuf.led_fade_operation = 0;
	gLedBuf.led_run_operation = 0;
}
/***
* This funktion sets the destination color and configurates
* the Timer 2 PR2 Register. If the settings are done, the bit led_fade_operation is 1.
* 
**/
void ledstrip_set_fade(struct cmd_set_fade *pCmd)
{	
	char temp;
	struct max_changes_struct maxChange;
	
	FOR_EACH_MASKED_LED_DO(
		temp = gLedBuf.led_array[k];
		gLedBuf.led_destination[k] = b;
		maxChange.blue = ledstrip_get_change(b,temp);
		k++;
		
		temp = gLedBuf.led_array[k];
		gLedBuf.led_destination[k] = g;
		maxChange.green = ledstrip_get_change(g,temp);
		k++;
			
		temp = gLedBuf.led_array[k];
		gLedBuf.led_destination[k] = r;
		maxChange.red = ledstrip_get_change(r,temp);
		
		ledstrip_calc_change(&gLedBuf.led_changevalue[k], &maxChange);
	);
	timer_set_for_fade(pCmd->timevalue);
	gLedBuf.led_fade_operation = 1;
}


void ledstrip_do_fade()
{
#ifndef X86
	char fade_finish:1 = TRUE;
#else
	char fade_finish = TRUE;
#endif	
	
	char temp_current,temp_destination,temp_value;
	
	char i;
	for(i = 0; i < (NUM_OF_LED*3);i++)
	{
		temp_current = gLedBuf.led_array[i];
		temp_destination = gLedBuf.led_destination[i];
		//check if I have to add or sub
		if(temp_current > temp_destination)
		{
			//if I'm in the near of 0 or 255 I have to check if I can add or sub again( to protect an overflow)
			if(temp_current > gLedBuf.led_changevalue[i])
				temp_value = temp_current - gLedBuf.led_changevalue[i];
			else 
				//I will get an overflow if i sub again, so set current value to the destination value
				temp_value = temp_destination;
			gLedBuf.led_array[i] = temp_value;
			fade_finish = FALSE;
		}
		else if(temp_current < temp_destination)	
		{
			temp_value = (255 - gLedBuf.led_changevalue[i]);
			if(temp_current < temp_value)
				temp_value = temp_current + gLedBuf.led_changevalue[i];
			else 
				temp_value = temp_destination;
			gLedBuf.led_array[i] = temp_value;
			fade_finish = FALSE;
		}		
	}	
	if(fade_finish) 
	{
		gLedBuf.led_fade_operation = FALSE;
		//send Fade Done
		USARTsend('F');
		USARTsend('D');
	}
	else 
		spi_send_ledbuf(&gLedBuf.led_array[0]);
}

//This funktion returns the differenz between the currentvalue of a led and the destinationvalue
uns8 ledstrip_get_change(uns8 destinationvalue, uns8 currentvalue)
{
	uns8 temp;
	if(destinationvalue > currentvalue)
		return temp = destinationvalue - currentvalue;
	else
		return temp = currentvalue - destinationvalue;			
}

/***
* This function calculates a value for the steps by fading operations.
* Reason: When you are fading two leds with different values, on led is normaly faster than
* the other. So I calculate a value for the changes by one fading step. On led fades in bigger
* steps and the other in a smaller.
* Function: 
* Always divide all three values by 2 (rotate right)
* Don't divide a value if it's smaller than 1
* If all values are smaller as the smallestChange-Define and one value must be 1, 
* than write the calculated values in the change array
**/
void ledstrip_calc_change(uns8 *change_array,struct max_changes_struct *Changestruct)
{
	uns8 temp_red = Changestruct->red;
	uns8 temp_green = Changestruct->green;
	uns8 temp_blue = Changestruct->blue;
	struct breakcondition
	{
		char red:1;
		char green:1;
		char blue:1;
	} breakbits;
	
	while(TRUE)	
	{
		if( (temp_red == 0)&&
			(temp_green == 0) &&
			(temp_blue == 0))
			break;
	
		breakbits.red = FALSE;
		breakbits.green = FALSE;
		breakbits.blue = FALSE;
		
		if(temp_red > 1) 
		{
			temp_red = temp_red >> 1;
			breakbits.red = FALSE;
		}
		else 
			breakbits.red = TRUE;
		
		if(temp_green > 1)
		{
			temp_green = temp_green >> 1;
			breakbits.green = FALSE;
		}
		else 
			breakbits.green = TRUE;
	
		if(temp_blue > 1) 
		{
			temp_blue = temp_blue >> 1;
			breakbits.blue = FALSE;
		}
		else
			breakbits.blue = TRUE;
	// do the Final check if the calculation is finished
		if( ( (temp_red < smallestChange) && (temp_green < smallestChange) && (temp_blue < smallestChange) ) &&
			( breakbits.red || breakbits.green || breakbits.blue ) &&
			( (temp_red == 1)||(temp_blue == 1)||(temp_green == 1) )
			) 
			break;
	}
	
	// set the calculated values to the change-array
	*change_array = temp_red;
	change_array--;
	*change_array = temp_green;
	change_array--;
	*change_array = temp_blue;
}
