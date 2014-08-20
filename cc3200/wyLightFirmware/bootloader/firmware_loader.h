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

#define FIRMWARE_ORIGIN 	0x20014000
#define BUFFER_SIZE 		1024

#define FIRMWARE_FILENAME	"/temp/firmware.bin\0"
#define SHA1SUM_FILENAME	"/temp/firmware.sha\0"

int LoadAndExecuteFirmware(void);
void StartFirmware(void);
long SaveSRAMContentAsFirmware(const unsigned char *pSource, const unsigned long length);


#endif /* FIRMWARE_DOWNLOAD_H_ */
