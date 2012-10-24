/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
 This file is part of Wifly_Light.
 
 Wifly_Light is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Wifly_Light is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Wifly_Light.  If not, see <http://www.gnu.org/licenses/>. */

#include "platform.h"
#include "eeprom.h"

#ifdef X86
static uns8 g_Eeprom[0x400];
unsigned char Eeprom_Read(uns16 adress)
{
	return g_Eeprom[adress];
}

void Eeprom_Write(uns16 adress, unsigned char data)
{
	g_Eeprom[adress] = data;
}
#else
//*********************** EEPROM BYTE SCHREIBEN  **********************************************
void Eeprom_Write(uns16 adress, uns8 data)
{
#ifdef TEST_EEPROM
	UART_SendString("Writing in EEPROM");
	UART_Send(0x0a);
	UART_Send(0x0d);
	
	UART_SendString("Addresse:");
	UART_SendNumber(adress.high8,'H');
	UART_SendNumber(adress.low8,'L');
	UART_SendString("Data:");
	UART_SendNumber(data,'D');
#endif
	bit GIE_status; 
	GIE_status = GIE;	
	
	EEADRH = adress.high8;
	EEADR = adress.low8;        	// Adresse in Adressregister übertragen
	EEDATA = data;          		// Daten in Datenregister übertragen
	
	CFGS = 0;
	EEPGD = 0;                		// Auswahl: Programmspeicher lesen oder EEPROM
	WREN = 1;                 		// Schreiben ins EEPROM erlauben
	GIE=0;                  		// Interrups verbieten
	
	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR = 1; 						// Starten des Schreibens
	GIE = GIE_status;               // Interrups erlauben
	WREN = 0;
	while(WR);
}

//*********************** EEPROM BYTE LESEN  **********************************************

uns8 Eeprom_Read(uns16 adress)
{

#ifdef TEST_EEEPROM
	UART_SendString("Reading in EEPROM");
	UART_Send(0x0a);
	UART_Send(0x0d);
	
	UART_SendString("Addresse:");
	UART_SendNumber(adress.high8,'H');
	UART_SendNumber(adress.low8,'L');
#endif

	uns8 data;
	EEADRH = adress.high8;        // Adresse in Adressregister übertragen
	EEADR = adress.low8;
	CFGS = 0;
	EEPGD = 0;                			// Auswahl: Programmspeicher lesen oder EEPROM
	RD = 1;                   			// Starten des Lesesn
	data = EEDATA;       				// Daten aus Datenregister auslesen
#ifdef TEST_EEPROM
	UART_SendString("Data:");
	UART_SendNumber(data,'D');
#endif	
    return data;
}
#endif /* #ifdef X86 */

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void Eeprom_WriteBlock(uns8 *array, uns16 adress, uns8 length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	uns8 i;
	for(i=0;i<length;i++)
	{
		Eeprom_Write(adress,*array);
		adress++;
		array++;
	}
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void Eeprom_ReadBlock(uns8 *array, uns16 adress, uns8 length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	uns8 i, temp;
	for(i=0;i<length;i++)
	{
		temp = Eeprom_Read(adress);
		array[i] = temp;
		adress++;
	}
}
