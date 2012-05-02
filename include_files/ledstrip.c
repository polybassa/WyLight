//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x 

struct LedBuffer gLedBuf;

void ledstrip_init(void)
{
	char k;
	for(k = 0;k < NUM_OF_LED; k++)
	{
		gLedBuf.led_array_r[k] = 0;
		spi_send(0);
		gLedBuf.led_array_r[k] = 0;
		spi_send(0);
		gLedBuf.led_array_r[k] = 0;
		spi_send(0);
	}
}

void ledstrip_set_color(char *address, char r, char g, char b)
{	
	char k,mask;
	mask = 0b00000001;
	for(k = 0; k < NUM_OF_LED; k++)
	{	
		if(*address && mask)
		{
			spi_send(b);
			spi_send(g);
			spi_send(r);
		}
		else
		{	
			spi_send(0);
			spi_send(0);
			spi_send(0);
		}
		RLF(mask,1);
		if(Carry == 1) 
		{
			address++;
			mask= 0b00000001;
		}
	}
}