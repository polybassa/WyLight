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

#ifndef _Crc_BuildCrc_H_
#define _Crc_BuildCrc_H_

 // Include-Datei zum Erstellen von Crc Pruefsummen
 //
 //
 // Nils Weiﬂ
 // 14.04.2012
 // Compiler CC5x

// 16-bit CCIT crc

//adds one byte to the given crc checksum
void Crc_AddCrc(unsigned char byte,unsigned char* p_crcH,unsigned char* p_crcL);
void Crc_AddCrc16(unsigned char byte,unsigned short* pCrc);

//do a complete crc calulation 
void Crc_BuildCrc(unsigned char *data, unsigned char length, unsigned char* crcH_out, unsigned char* crcL_out);


//set the crc-Bytes to inital value
void Crc_NewCrc(unsigned char* p_crcH, unsigned char* p_crcL);

#endif
