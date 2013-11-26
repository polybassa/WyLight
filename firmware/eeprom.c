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

#include "eeprom.h"

#ifdef __CC8E__
//*********************** EEPROM BYTE SCHREIBEN  **********************************************
void Eeprom_Write(const uns16 adress, const uns8 data)
{
	bit GIE_status;
	GIE_status = GIE;

	EEADRH = adress.high8;
	EEADR = adress.low8;            // Adresse in Adressregister übertragen
	EEDATA = data;                          // Daten in Datenregister übertragen

	CFGS = 0;
	EEPGD = 0;                              // Auswahl: Programmspeicher lesen oder EEPROM
	WREN = 1;                               // Schreiben ins EEPROM erlauben
	GIE = 0;                                  // Interrups verbieten

	EECON2 = 0x55;
	EECON2 = 0xAA;
	WR = 1;                                                 // Starten des Schreibens
	GIE = GIE_status;               // Interrups erlauben
	WREN = 0;
	while(WR) ;
}

//*********************** EEPROM BYTE LESEN  **********************************************

uns8 Eeprom_Read(const uns16 adress)
{
	uns8 data;
	EEADRH = adress.high8;        // Adresse in Adressregister übertragen
	EEADR = adress.low8;
	CFGS = 0;
	EEPGD = 0;                                      // Auswahl: Programmspeicher lesen oder EEPROM
	RD = 1;                                         // Starten des Lesesn
	data = EEDATA;                                  // Daten aus Datenregister auslesen
	return data;
}

#else
#include "ScriptCtrl.h"
#include "wifly_cmd.h"
#include <assert.h>
static uns8 g_Eeprom[(1 + SCRIPTCTRL_NUM_CMD_MAX) * sizeof(struct led_cmd)];

uns8 Eeprom_Read(const uns16 adress)
{
	return g_Eeprom[adress];
}

void Eeprom_Write(const uns16 adress, const uns8 data)
{
	assert(adress < sizeof(g_Eeprom));
	g_Eeprom[adress] = data;
}
#endif /* #ifdef X86 */

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void Eeprom_WriteBlock(const uns8 *array, uns16 adress,const uns8 length)                       //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	uns8 i;
	for(i = 0; i < length; i++) {
		uns8 *pByte = (uns8 *)array;
		Eeprom_Write(adress, *pByte);
		adress++;
		array++;
	}
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void Eeprom_ReadBlock(uns8 *array, uns16 adress, const uns8 length)                     //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
	if(!array) return;
	uns8 i, temp;
	for(i = 0; i < length; i++) {
		temp = Eeprom_Read(adress);
		array[i] = temp;
		adress++;
	}
}
