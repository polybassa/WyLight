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

#ifndef _EEPROM_H_
#define _EEPROM_H_

//Funktionen f¸r EEPROM-Zugriffe

//Nils Weiﬂ 
//05.09.2011
//Compiler CC5x
void EEPROM_WR(uns16 adress, char data);
char EEPROM_RD(uns16 adress);
void EEPROM_WR_BLK(char *array, uns16 adress, char length);
void EEPROM_RD_BLK(char *array, uns16 adress, char length);
#endif
