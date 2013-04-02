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
#include "trace.h"

#ifdef __CC8E__
void Flash_ReadBlock(const uns8 upperAdress, const uns16 adress, uns8 *data, const uns16 length_bytes)
{
	TBLPTRU = upperAdress;
	TBLPTRH = adress.high8;
	TBLPTRL = adress.low8;        	// Adresse in Adressregister uebertragen
	
	//length is in bytecount, so we have to divide by 2 to get the wordcount of length
	uns16 length_words = length_bytes >> 1;
	while(length_words)
	{
		length_words--;
#asm
		tblrd*+
#endasm
		*data++ = TABLAT;
		Trace_Hex(TABLAT);
#asm
		tblrd*+
#endasm
		*data++ = TABLAT;
		Trace_Hex(TABLAT);
	}
}

uns16 Flash_Read(const uns8 upperAdress, const uns16 adress)
{
	uns16 buffer;
	Flash_ReadBlock(upperAdress, adress, &buffer, sizeof(buffer));
	return buffer;
}

void Flash_WriteBlock(const uns8 upperAdress, const uns16 adress, uns8 *data, const uns16 length_bytes)
{
	uns16 length_words = length_bytes >> 1;
	while(length_words)
	{
		uns16 mLength;
		if(length_words > FLASH_BLOCKSIZE_WORD)
		{
			length_words -= FLASH_BLOCKSIZE_WORD;
			mLength = FLASH_BLOCKSIZE_BYTE;
		}
		else
		{
			mLength = length_words << 1;
			length_words = 0;
		}
		
		uns8 buffer[FLASH_BLOCKSIZE_BYTE];
		Flash_ReadBlock(upperAdress, adress, buffer, sizeof(buffer));
		
		uns8 *pBuffer;
		pBuffer = buffer;
		
		while(mLength)
		{
			mLength--;
			uns8 temp = *data;
			*pBuffer = temp;
			pBuffer++;
			data++;
		}
		
		Flash_EraseBlock64(upperAdress, adress);
		
		TBLPTRU = upperAdress;
		TBLPTRH = adress.high8;
		TBLPTRL = adress.low8;
		
		mLength = FLASH_BLOCKSIZE_BYTE;
		pBuffer = buffer;
		while(mLength)
		{
			mLength--;
			TABLAT = *pBuffer++;
#asm
			tblwt+*
#endasm
		}
#asm
		bsf	EECON1, EEPGD
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
		adress += FLASH_BLOCKSIZE_BYTE;
		if(adress == 0)
			upperAdress++;
	
	}
	
}

void Flash_EraseBlock64(const uns8 upperAdress, const uns16 adress)
{
	TBLPTRU = upperAdress;
	TBLPTRH = adress.high8;
	TBLPTRL = adress.low8;        	// Adresse in Adressregister uebertragen
	
#asm
	bsf	EECON1, EEPGD
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
#endasm
	
}

void Flash_EraseBlocks64(uns8 upperAdress, uns16 adress, uns8 numBlocks)
{
	while(numBlocks)
	{
		numBlocks--;
		Flash_EraseBlock64(upperAdress, adress);
		
		adress += FLASH_BLOCKSIZE_BYTE;
		if(adress == 0)
			upperAdress++;
	}
}
#endif /* __CC8E__ */