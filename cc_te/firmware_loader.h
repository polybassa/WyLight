/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

 This file is part of WyLight.

 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICUL<AR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef FIRMWARE_LOADER_H_
#define FIRMWARE_LOADER_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void StartFirmware(void);
long EmplaceFirmware(void);
long SaveSRAMContent(uint8_t *pSource, const size_t length);

#ifdef __cplusplus
}
#endif

#endif /* FIRMWARE_DOWNLOAD_H_ */
