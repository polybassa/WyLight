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
		gLedBuf.led_array_r[k] = 0;
		gLedBuf.led_array_r[k] = 0;
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
			gLedBuf.led_array_r[k] = r;
			gLedBuf.led_array_g[k] = g;
			gLedBuf.led_array_b[k] = b;
#ifdef TEST
			USARTsend_num(k,'K');
			USARTsend_num(b,'B');
			USARTsend_num(g,'G');
			USARTsend_num(r,'R');
			USARTsend(0x0A);
#endif
		}
		else
		{	
			gLedBuf.led_array_r[k] = 0;
			gLedBuf.led_array_g[k] = 0;
			gLedBuf.led_array_b[k] = 0;
		}
		RLF(mask,1);
		if(Carry == 1) 
		{
#ifdef TEST
			USARTsend_str("Carry");
#endif
			address++;
			mask= 0b00000001;
		}
	}
	spi_send_ledbuf(&gLedBuf.led_array_r[0],&gLedBuf.led_array_g[0],&gLedBuf.led_array_b[0]);
}