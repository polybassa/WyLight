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

#include "platform.h"

//adds one byte to the given crc checksum
void Crc_AddCrc(const uns8 byte, uns8 *p_crcH, uns8 *p_crcL);

#ifdef __cplusplus
extern "C" {
#endif
void Crc_AddCrc16(const uns8 byte, uns16 *pCrc);
#ifdef __cplusplus
}
#endif

//do a complete crc calulation
void Crc_BuildCrc(const uns8 *data, const uns8 length, uns8 *crcH_out, uns8 *crcL_out);


//set the crc-Bytes to inital value
void Crc_NewCrc(uns8 *p_crcH, uns8 *p_crcL);

#endif
