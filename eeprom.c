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

#ifndef X86
//*********************** EEPROM BYTE SCHREIBEN  **********************************************

void EEPROM_WR(uns16 adress, uns8 data)
{
#ifdef TEST_EEPROM
	USARTsend_str("Writing in EEPROM");
	USARTsend(0x0a);
	USARTsend(0x0d);
	
	USARTsend_str("Addresse:");
	USARTsend_num(adress.high8,'H');
	USARTsend_num(adress.low8,'L');
	USARTsend_str("Data:");
	USARTsend_num(data,'D');
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

char EEPROM_RD(uns16 adress)
{

#ifdef TEST_EEEPROM
	USARTsend_str("Reading in EEPROM");
	USARTsend(0x0a);
	USARTsend(0x0d);
	
	USARTsend_str("Addresse:");
	USARTsend_num(adress.high8,'H');
	USARTsend_num(adress.low8,'L');
#endif

    char data;
    EEADRH = adress.high8;        // Adresse in Adressregister übertragen
    EEADR = adress.low8;
	CFGS = 0;
	EEPGD = 0;                			// Auswahl: Programmspeicher lesen oder EEPROM
    RD = 1;                   			// Starten des Lesesn
    data = EEDATA;       				// Daten aus Datenregister auslesen
#ifdef TEST_EEPROM
	USARTsend_str("Data:");
	USARTsend_num(data,'D');
#endif	
    return data;
}
#else
void EEPROM_WR(char adress, char data);
char EEPROM_RD(char adress);
#endif /* #ifndef X86 */

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void EEPROM_WR_BLK(unsigned char *array, uns16 adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	char i;
	for(i=0;i<length;i++)
	{
		EEPROM_WR(adress,*array);
		adress++;
		array++;
	}
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void EEPROM_RD_BLK(unsigned char *array, uns16 adress, char length)			//Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	uns8 i, temp;
	for(i=0;i<length;i++)
	{
		temp = EEPROM_RD(adress);
		array[i] = temp;
		adress++;
	}
}
