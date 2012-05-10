//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x 

#include "ledstrip.h"

struct LedBuffer gLedBuf;

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

void ledstrip_set_color(char *address, char r, char g, char b)
{	
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

/** This function extracts the parameters for the set_color command
*** from the EEPROM in relation to the CmdWidth and give the values 
*** to the next function with controls the led's

*** BYTES according to Pointer: x=Commandpointer w=Commandwith
							   x-w     x-w+1   x-w+2   x-w+3  x-w+4     x-w+5         x-w+6        x-w+7                                              x
*** Example: EEPROM DATA: <SET_COLOR> <ADDR0> <ADDR1> <ADDR2> <ADDR3> <RED_VALUE> <GREEN_VALUE> <BLUE_VALUE> <not important> <not important> <SET_COLOR(nextCommand)>
*/ 
void sub_func_set_color(char *cmdPointer)
{
	char r,g,b,i, temp,temp1,address[4];
	
	r = EEPROM_RD(*cmdPointer - CmdWidth + 5);
	g = EEPROM_RD(*cmdPointer - CmdWidth + 6);
	b = EEPROM_RD(*cmdPointer - CmdWidth + 7);
	for(i=0;i<4;i++)
	{
		temp1 = *cmdPointer;
		temp1 = temp1 - CmdWidth + 1 + i;
		temp = EEPROM_RD(temp1);
		address[i] = temp;
	}
	
#ifdef TEST
	USARTsend_str("Command:");
	USARTsend_num(address[0],'#');
	USARTsend_num(address[1],'#');
	USARTsend_num(address[2],'#');
	USARTsend_num(address[3],'#');
	USARTsend_num(r,'R');
	USARTsend_num(g,'G');
	USARTsend_num(b,'B');
#endif	
	ledstrip_set_color(&address[0],r,g,b);
}
