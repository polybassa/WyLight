/*
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

uns8 AdressValid(const uns16 adress)
{
	if(adress < 0xf000 && adress >= 0x5000)
		return TRUE;
	else {
		return FALSE;
	}
}

void Flash_ReadBlock(const uns8 upperAdress, const uns16 adress, uns8 *data, const uns16 length_bytes)
{
	TBLPTRU = upperAdress;
	TBLPTRH = adress.high8;
	TBLPTRL = adress.low8;          // Adresse in Adressregister uebertragen

	//length is in bytecount, so we have to divide by 2 to get the wordcount of length
	uns16 i;
	for(i = 0; i < length_bytes; i++) {
#asm
		tblrd *+
#endasm
		*data++ = TABLAT;
	}
}

uns16 Flash_Read(const uns8 upperAdress, const uns16 adress)
{
	uns16 buffer;
		Flash_ReadBlock(upperAdress, adress, &buffer, sizeof(buffer));
	return buffer;
}

void Flash_WriteBlock(uns16 adress, const uns8 *data, const uns16 length_bytes)
{
	uns16 pageAdress, offset, endAdress, i;

	/* check the memory range, abort if we want to write in the app oder bl memory */
	endAdress = adress + length_bytes;
	if(!AdressValid(endAdress)) return;
	if(!AdressValid(adress)) return;

	while(endAdress > adress)
	{
		/* calculate the page adress for erase and write operation */
		pageAdress = adress & 0xffc0;
		/* calculate offset. Used as empty space befor the data block is written on a page */
		offset = adress - pageAdress;

		bank14 uns8 flashBuff[FLASH_BLOCKSIZE_BYTE];
		/* get old data from current page */
		Flash_ReadBlock(0, pageAdress, flashBuff, sizeof(flashBuff));
		/* erase this page */
		Flash_EraseBlock64(pageAdress);

		for(i = 0; i < sizeof(flashBuff); i++) {
			/* if we are in the offset region, we don't modify the old data */
			if(i < offset) continue;

			/* calculate a temp adress that we know, when we reach the endadress */
			uns16 tempAdress = pageAdress + i;
			if(tempAdress < endAdress) {
				/* insert new data to page in RAM */
				uns8 temp = *data;
				data++;
				flashBuff[i] = temp;
			}
		}
		/* transfer page from RAM to holding registers */
		TBLPTRU = 0;
		TBLPTRH = pageAdress.high8;
		TBLPTRL = pageAdress.low8;
		for(i = 0; i < sizeof(flashBuff); i++) {
			TABLAT = flashBuff[i];
#asm
			tblwt *+
#endasm
		}
		/* write page to program memory */
		TBLPTRU = 0;
		TBLPTRH = pageAdress.high8;
		TBLPTRL = pageAdress.low8;
#asm
		bsf EECON1, EEPGD
		bcf EECON1, CFGS
		bsf EECON1, WREN
		bcf INTCON, GIE
		    movlw 0x55
		movwf EECON2
		    movlw 0xaa
		movwf EECON2
		bsf EECON1, WR
		bsf INTCON, GIE
		bcf EECON1, WREN
#endasm
		/* increment adress with the size of a page for the next run */
		adress = pageAdress + FLASH_BLOCKSIZE_BYTE;
	}
}

void Flash_EraseBlock64(const uns16 adress)
{
	uns16 pageAdress = adress & 0xffc0;
	if(!AdressValid(pageAdress)) return;
	TBLPTRU = 0;
	TBLPTRH = pageAdress.high8;
	TBLPTRL = pageAdress.low8;              // Adresse in Adressregister uebertragen

#asm
	bsf EECON1, EEPGD
	bcf EECON1, CFGS
	bsf EECON1, WREN
	bsf EECON1, FREE
	bcf INTCON, GIE
	    movlw 0x55
	movwf EECON2
	    movlw 0xaa
	movwf EECON2
	bsf EECON1, WR
	bsf INTCON, GIE
	    tblrd *-
#endasm

}

void Flash_EraseBlocks64(const uns16 adress, uns8 numBlocks)
{
	uns16 pageAdress = adress & 0xffc0;
	while(numBlocks)
	{
		numBlocks--;
		Flash_EraseBlock64(pageAdress);

		pageAdress = pageAdress + FLASH_BLOCKSIZE_BYTE;
	}
}
#endif /* __CC8E__ */