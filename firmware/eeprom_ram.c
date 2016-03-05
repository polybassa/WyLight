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

#include <assert.h>
#include "eeprom.h"
#include "ScriptCtrl.h"
#include "wifly_cmd.h"

static uns8 g_Eeprom[(1 + SCRIPTCTRL_NUM_CMD_MAX) * sizeof(struct led_cmd)];

void Eeprom_Init(void)
{}

uns8 Eeprom_Read(const uns16 adress)
{
    return g_Eeprom[adress];
}

void Eeprom_Write(const uns16 adress, const uns8 data)
{
    assert(adress < sizeof(g_Eeprom));
    g_Eeprom[adress] = data;
}

//*********************** EEPROM BYTEARRAY SCHREIBEN  **************************************

void Eeprom_WriteBlock(const uns8* array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
    if (!array) return;

    memcpy(&g_Eeprom[adress], array, length);
}

//*********************** EEPROM BYTEARRAY LESEN  **************************************

void Eeprom_ReadBlock(uns8* array, uns16 adress, const uns8 length) //Zum Ausführen eines beliebigen Befehls durch den Programmcode
{
    if (!array) return;

    memcpy(array, &g_Eeprom[adress], length);
}
