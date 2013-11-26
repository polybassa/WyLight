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


#ifndef ____flash__
#define ____flash__

#include "platform.h"

#define FLASH_BLOCKSIZE_BYTE 64
#define FLASH_BLOCKSIZE_WORD 32

uns16 Flash_Read(const uns8 upperAdress, const uns16 adress);
void Flash_ReadBlock(const uns8 upperAdress, const uns16 adress, uns8 *data, const uns16 length_bytes);
void Flash_WriteBlock(uns16 adress, const uns8 *data, const uns16 length_bytes);
void Flash_EraseBlock64(const uns16 adress);
void Flash_EraseBlocks64(const uns16 adress, uns8 numBlocks);

#endif /* defined(____flash__) */
