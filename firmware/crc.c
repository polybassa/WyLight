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

#include "crc.h"

// 16-bit CCIT CRC
void Crc_AddCrc(const uns8 byte,uns8 *p_crcH,uns8 *p_crcL)
{
	uns8 index, crcH, crcL;
	crcH = *p_crcH;
	crcL = *p_crcL;
#ifdef __CC8E__
	MOVF(byte,0);

	XORWF(crcH,0);
	MOVWF(index);
	ANDLW(0xf0);
	SWAPF(index,1);
	XORWF(index,1);

	MOVF(index,0);
	ANDLW(0xf0);
	XORWF(crcL,0);
	MOVWF(crcH);

	RLF(index,0);
	RLF(index,0);
	XORWF(crcH,1);
	ANDLW(0xe0);
	XORWF(crcH,1);

	SWAPF(index,1);
	XORWF(index,0);
	MOVWF(crcL);
#else
	uns8 work, temp;

	work = byte;                    //MOVF(byte,0);

	work = work ^ crcH;             //XORWF(crcH,0);
	index = work;                   //MOVWF(index);
	work = 0xf0 & work;             //ANDLW(0xf0);
	index = ((index << 4) & 0xf0) | ((index >> 4) & 0x0f); //SWAPF(index,1);
	index = work ^ index;           //XORWF(index,1);

	work = index;                   //MOVF(index,0);
	work = work & 0xf0;             //ANDLW(0xf0);
	work = work ^ crcL;             //XORWF(crcL,0);
	crcH = work;                    //MOVWF(crcH);

	temp = 0x80 & index;            //RLF(index,0);
	work = index << 1;
	temp = temp >> 7;
	work = work & 0xfe;
	work = work | temp;

	crcH = work ^ crcH;             //XORWF(crcH,1);
	work = work & 0xe0;             //ANDLW(0xe0);
	crcH = work ^ crcH;             //XORWF(crcH,1);

	index = ((index << 4) & 0xf0) | ((index >> 4) & 0x0f);   //SWAPF(index,1);
	work = work ^ index;            //XORWF(index,0);
	crcL = work;                    //MOVWF(crcL);
#endif /* #ifdef __CC8E__ */
	*p_crcH = crcH;
	*p_crcL = crcL;
}

/**
 * Wrapper for the Crc_AddCrc function with seperate parameter for high and low
 * crc value. In a next refactoring step we should replace that other functions
 * with this one.
 */
#ifdef __cplusplus
extern "C" {
#endif
void Crc_AddCrc16(const uns8 byte, uns16 *pCrc)
{
		Crc_AddCrc(byte, ((uns8 *)pCrc) + 1, (uns8 *)pCrc);
}
#ifdef __cplusplus
}
#endif

void Crc_BuildCrc(const uns8 *data, const uns8 length, uns8 *crcH_out, uns8 *crcL_out)
{
	if(!crcH_out) return;
	if(!crcL_out) return;
	if(!data) return;
	uns8 crcH,crcL,i,byte;
	crcH = 0xff;
	crcL = 0xff;

	for(i = 0; i < length; i++) {
		byte = data[i];
		Crc_AddCrc(byte,&crcH,&crcL);
	}

	*crcH_out = crcH;
	*crcL_out = crcL;
}

void Crc_NewCrc(uns8 *p_crcH, uns8 *p_crcL)
{
	if(!p_crcH) return;
	if(!p_crcL) return;
	// bootloader is using 0x0000 as initial crc not 0xFFFF
	*p_crcH = 0x00;
	*p_crcL = 0x00;
}
