/**
   Copyright (C) 2012 Nils Weiss, Patrick Brünn.

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

#ifndef _EXTERNAL_EEPROM_H_
#define _EXTERNAL_EEPROM_H_

#include "platform.h"

/* globals */
#define EEPROM_ADRESS 0xDE
#define EEPROM_MEMORY_END_ADRESS 0x7F

/* eeprom access functions */
void ExtEeprom_Init(void);
void ExtEeprom_Write(const uns8 adress, const uns8 data);
uns8 ExtEeprom_Read(const uns8 adress);
void ExtEeprom_WriteBlock(const uns8* array, const uns8 adress, const uns8 length);
void ExtEeprom_ReadBlock(uns8* array, const uns8 adress, const uns8 length);
#endif