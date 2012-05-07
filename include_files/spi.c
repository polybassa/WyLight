//Nils Weiﬂ 
//20.04.2012
//Compiler CC5x
void spi_init()
{
	TRISC.3 = 0;        // Make port RC3 an output(SPI Clock)
    TRISC.4 = 1;        // Make port RC4 an input(SPI Data In)
    TRISC.5 = 0;        // Make port RC5 an output(SPI Data Out)
	SSPM1 = 1;
    /* 
	SMP = 0;
	CKP = 0;
	CKE = 0;
	*/
	SSPEN = 1;
}

void spi_send(char data)
{
	SSPBUF = data;	
	while(!SSPIF);
}

char spi_receive(char data)
{
	SSPBUF = data;	
	while(SSPIF == 0);
	return SSPBUF;
}
/***
**	This function sends the array to the LED controller(WS2801)
**  it starts with the last byte to get a correct output
***/
void spi_send_arr(char *array, char length)
{
	if(array == 0) return;
	char i;
	for(i = length; i == 0; i-- )
	{
		spi_send(array[i]);
	} 
}