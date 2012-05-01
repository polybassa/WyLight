//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x 

static struct LedBuffer gLedBuf;

void ledstrip_init(void)
{
	char k;
	for(k = 0;k < BUFFERSIZE; k++)
	{
		gLedBuf.led_array[k] = 0;
		gLedBuf.led_ctrl_array[k] = 0;
	}
	spi_send_arr(&gLedBuf.led_array[0], BUFFERSIZE);
}

void ledstrip_set_color(char *address, char r, char g, char b)
{				
	char k,selector;
	selector = 0;
	for(k = 0; k < BUFFERSIZE; k++)
	{	
		switch (selector)
		{
			case 0: 
				{
					gLedBuf.led_array[k] = r;
					selector = 1;
				} break;
			case 1:
				{	
					gLedBuf.led_array[k] = g;
					selector = 2;
				}break;
			case 2:
				{
					gLedBuf.led_array[k] = b;
					selector = 0;
				}break;
		}
	}
	spi_send_arr(&gLedBuf.led_array[0], BUFFERSIZE);
}