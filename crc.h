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

#ifndef _CRC_H_
#define _CRC_H_

 // Include-Datei zum Erstellen von CRC Pr¸fsummen
 //
 //
 // Nils Weiﬂ
 // 14.04.2012
 // Compiler CC5x

// 16-bit CCIT CRC

//adds one byte to the given crc checksum
void addCRC(char byte, unsigned char* p_crcH, unsigned char* p_crcL);

//do a complete crc calulation 
void CRC(char *data, char length, char* crcH_out, char* crcL_out);

//set the CRC-Bytes to inital value
void newCRC(unsigned char* p_crcH, unsigned char* p_crcL);

#endif
