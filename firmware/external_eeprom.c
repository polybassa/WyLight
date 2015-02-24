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

#include "external_eeprom.h"
#include "iic.h"
#include "trace.h"

static uns8 g_ExtEepromDetected;

void ExtEeprom_Init(void)
{
    I2C_Init();
    if (I2C_DetectSlave(EEPROM_ADRESS)) {
        g_ExtEepromDetected = 1;
        I2C_Write(EEPROM_ADRESS, 0xff, 0x00);
    } else {
        g_ExtEepromDetected = 0;
    }
}
void ExtEeprom_Write(const uns8 adress, const uns8 data)
{
    if (adress <= EEPROM_MEMORY_END_ADRESS) {
        if (g_ExtEepromDetected)
            I2C_Write(EEPROM_ADRESS, adress, data);
    } else {
        Trace_String("invalid ext eeprom adress");
    }
}
uns8 ExtEeprom_Read(const uns8 adress)
{
    if (adress <= EEPROM_MEMORY_END_ADRESS) {
        if (g_ExtEepromDetected)
            return I2C_Read(EEPROM_ADRESS, adress);
    } else {
        Trace_String("invalid ext eeprom adress");
    }
    return 0;
}
void ExtEeprom_WriteBlock(const uns8* array, const uns8 adress, const uns8 length)
{
    if (adress <= EEPROM_MEMORY_END_ADRESS) {
        if (g_ExtEepromDetected)
            I2C_WriteBlock(EEPROM_ADRESS, array, adress, length);
    } else {
        Trace_String("invalid ext eeprom adress");
    }
}
void ExtEeprom_ReadBlock(uns8* array, const uns8 adress, const uns8 length)
{
    if (adress <= EEPROM_MEMORY_END_ADRESS) {
        if (g_ExtEepromDetected)
            I2C_ReadBlock(EEPROM_ADRESS, array, adress, length);
    } else {
        Trace_String("invalid ext eeprom adress");
    }
}