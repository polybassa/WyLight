/**
   Copyright (C) 2012 - 2016 Nils Weiss, Patrick Bruenn.

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

#define Eeprom_Init()

//*********************** EEPROM BYTE SCHREIBEN  **********************************************
void Eeprom_Write(const uns16 adress, const uns8 data)
{
    bit GIE_status = GIE;

    EEADRH = HIGH_BYTE(adress);
    EEADR = LOW_BYTE(adress); // set address register
    EEDATA = data; // copy data into output register

    EECON1 = ~_CFGS & ~_EEPGD & _WREN;
    INTCON &= ~_GIE;

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1 |= _WR; // enable write
    INTCON |= GIE_status; // restore interrupt state
    EECON1 = ~_WREN;
    while (EECON1 & _WR) {}
}

//*********************** EEPROM BYTE LESEN  **********************************************

uns8 Eeprom_Read(const uns16 adress)
{
    EEADRH = HIGH_BYTE(adress);
    EEADR = LOW_BYTE(adress); // set address register
    EECON1 &= ~_CFGS & ~_EEPGD & _RD;
    return EEDATA;
}

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void Eeprom_WriteBlock(const uns8* array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
    if (!array) return;

    for (uns8 i = 0; i < length; i++) {
        uns8* pByte = (uns8*)array;
        Eeprom_Write(adress, *pByte);
        adress++;
        array++;
    }
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void Eeprom_ReadBlock(uns8* array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
    if (!array) return;

    for (uns8 i = 0; i < length; i++) {
        array[i] = Eeprom_Read(adress);
        adress++;
    }
}
