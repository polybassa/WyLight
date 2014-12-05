/**
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

 This file is part of WyLight.

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

#ifndef _WY_Crc_BuildCrc_H_
#define _WY_Crc_BuildCrc_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "platform.h"

//adds one byte to the given crc checksum
void Crc_AddCrc(const uns8 byte, uns8 *p_crcH, uns8 *p_crcL);

//set the crc-Bytes to inital value
void Crc_NewCrc(uns8 *p_crcH, uns8 *p_crcL);

void Crc_Init(void);
	
#ifdef __cplusplus
}
#endif
		
#endif
