/*
 Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef FIRMWARE_LOADER_H_
#define FIRMWARE_LOADER_H_

#include <stdint.h>
#include <stdio.h>

#define FIRMWARE_ORIGIN 	0x20012000

long LoadAndExecuteFirmware(void);
void StartFirmware(void);
long SaveSRAMContentAsFirmware(uint8_t *pSource, const size_t length);


#endif /* FIRMWARE_DOWNLOAD_H_ */
