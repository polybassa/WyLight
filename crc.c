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

// 16-bit CCIT CRC

void Crc_AddCrc(unsigned char byte,unsigned char* p_crcH,unsigned char* p_crcL)
{
	unsigned char index;
	unsigned char crcH,crcL;
	crcH = *p_crcH;
	crcL = *p_crcL;

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

	*p_crcH = crcH;
	*p_crcL = crcL;

}

void Crc_BuildCrc(unsigned char *data, unsigned char length, unsigned char* crcH_out, unsigned char* crcL_out)
{
	if(!crcH_out)return;
	if(!crcL_out)return;
	if(!data)return;
	unsigned char crcH,crcL,i,byte;
	crcH=0xff;
	crcL=0xff;

	for(i=0;i<length;i++)
	{
		byte = data[i];
		Crc_AddCrc(byte,&crcH,&crcL);
	}
	
	*crcH_out = crcH;
	*crcL_out = crcL;
}

void Crc_NewCrc(unsigned char* p_crcH, unsigned char* p_crcL)
{
    if(!p_crcH) return;
    if(!p_crcL) return;
    *p_crcH = 0xff;
    *p_crcL = 0xff;
}
