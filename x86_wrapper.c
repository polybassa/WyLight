#include <stdio.h>

void addCRC(char byte, char* p_crcH, char* p_crcL)
{
}

void USARTinit() {}
void USARTsend(char ch)
{
	FILE* gUSART = fopen("usart_out.txt", "a+");
	fputc(ch, gUSART);
	fclose(gUSART);
}


static char gEEPROM[255];
char EEPROM_RD(char adress)
{
	return gEEPROM[adress];
}


void EEPROM_WR(char adress, char data)
{
	gEEPROM[adress] = data;
}

void spi_init() {}
char spi_send(char data)
{
	FILE* gSPI = fopen("spi_out.txt", "a+");
	fprintf(gSPI, "%c", data);
	fclose(gSPI);
}
