/**
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
 
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


#include "Flash.h"

#ifdef __CC8E__
uns16 Flash_Read(const uns8 upperAdress, const uns16 adress)
{
	uns16 returnWord = 0;
	TBLPTRU = upperAdress;
	TBLPTRH = adress.high8;
	TBLPTRL = adress.low8;        	// Adresse in Adressregister ¸bertragen
	
#asm
	tblrd*+
#endasm
	returnWord.low8 = TABLAT;

#asm
	tblrd*+
#endasm
	returnWord.high8 = TABLAT;
	return returnWord;
}
#endif /* __CC8E__ */

void Flash_Write(const uns8 upperAdress, const uns16 adress, const uns16 data);

void Flash_WriteBlock(const uns8 upperAdress, const uns16 adress, uns16 *data, const uns16 length);

void Flash_Erase(const uns8 upperAdress, const uns16 adress);

void Flash_EraseBlock(const uns8 upperAdress, const uns16 adress, uns16 *data, const uns16 length);
*/