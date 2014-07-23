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

//Befehle:
//I2C_Init() zum initialisieren
//IICwrite(Slave-Adresse,Register-Adresse,Datenbyte) zum Schreiben eines Byte in einen Slave
//IICread(Slave-Adresse,Register-Adresse) ließt ein Byte von der angegebenen Adresse

#include "iic.h"

void I2C_Init()
{
	ANSELB = 0;
	SSP2ADD = 39;                             //Takt des IIC-Bus auf 400kHz einstellen
	SSP2STAT = 0x00;
	SSP2CON1 = 0x08;                  //Betriebsart des MSSP-Modul festlegen
	SSPEN2 = 1;                               //MSSP-Modul einschalten
}

void I2C_Write(const uns8 slaveaddr, const uns8 dataaddr, const uns8 data)
{
	//Writebit in Slaveadresse setzen

	slaveaddr = slaveaddr & 0b11111110;

	//Bus übernehmen
	SSP2IF = 0;
	SEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Slave ansprechen
	SSP2BUF = slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Datenregisteradresse übertragen
	SSP2BUF = dataaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Daten schreiben
	SSP2BUF = data;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
}

void I2C_WriteBlock(const uns8 slaveaddr, const uns8* data, const uns8 dataaddr,const uns8 length)
{
	//Writebit in Slaveadresse setzen
	
	slaveaddr = slaveaddr & 0b11111110;
	uns8 _length = length;
	//Bus übernehmen
	SSP2IF = 0;
	SEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Slave ansprechen
	SSP2BUF = slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Datenregisteradresse übertragen
	SSP2BUF = dataaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	while (_length) {
		_length--;
		//Daten schreiben
		SSP2BUF = *data;
		data++;
		while(!SSP2IF) ;
		SSP2IF = 0;
	}
	
	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
}

void I2C_ReadBlock(const uns8 slaveaddr, uns8* buffer, const uns8 readaddr, const uns8 length)
{
	//Writebit in Slaveadresse setzen
	uns8 _slaveaddr;
	_slaveaddr = slaveaddr & 0b11111110;
	uns8 _length = length;
	
	//Bus übernehmen
	SSP2IF = 0;
	SEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Slave ansprechen
	SSP2BUF = _slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Datenregisteradresse übertragen
	SSP2BUF = readaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Bus übernehmen
	SSP2IF = 0;
	SEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Readbit in Slaveadresse setzen
	_slaveaddr = slaveaddr | 0b00000001;
	//Slave ansprechen
	SSP2BUF = _slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	while (_length) {
		//Pic auf Lesen umschalten
		SSP2IF = 0;
		RCEN2 = 1;
		while(!SSP2IF) ;
		
		*buffer = SSP2BUF;
		buffer++;
		_length--;
		
		//Send ACK if we have some bytes to read
		if (_length) {
			SSP2IF = 0;
			ACKEN2 = 1;
			ACKDT2 = 0;
			while(!SSP2IF) ;
		}
	}
	
	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
}

uns8 I2C_Read(const uns8 slaveaddr, const uns8 readaddr)
{
	//Writebit in Slaveadresse setzen
	uns8 _slaveaddr;
	uns8 _data;
	_slaveaddr = slaveaddr & 0b11111110;

	//Bus übernehmen
	SSP2IF = 0;
	SEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Slave ansprechen
	SSP2BUF = _slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Datenregisteradresse übertragen
	SSP2BUF = readaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Bus übernehmen
	SSP2IF = 0;
	RSEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Readbit in Slaveadresse setzen
	_slaveaddr |= 0b00000001;
	//Slave ansprechen
	SSP2BUF = _slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;

	//Pic auf Lesen umschalten
	RCEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	ACKEN2 = 0;
	_data = SSP2BUF;


	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	return _data;
}

uns8 I2C_DetectSlave(const uns8 slaveaddr) {
	//Bus übernehmen
	SSP2IF = 0;
	SEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//Slave ansprechen
	SSP2BUF = slaveaddr;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	//check ACKSTAT
	uns8 returnValue = SSP2CON2 & 0b01000000;
	
	//Bus freigeben
	PEN2 = 1;
	while(!SSP2IF) ;
	SSP2IF = 0;
	
	if (returnValue == 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

