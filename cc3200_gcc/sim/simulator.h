/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
 
 This file is part of WyLight.
 
 WyLight is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 WyLight is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef __SIMULATOR_H_
#define __SIMULATOR_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>

uint8_t memory[0x3FFFF];

#define ERROR -1
#define SUCCESS 0

#define IntVTableBaseSet(x)
#define IntMasterEnable()
#define IntEnable(x)
#define PRCMCC3200MCUInit()
#define PRCMPeripheralClkEnable(x,y)
#define PRCMPeripheralClkDisable(x,y)
#define SHAMD5IntRegister(x,y)
#define PRCMPeripheralReset(x)
#define SHAMD5ConfigSet(x,y)
#define IntDisable(x)
#define IntMasterDisable()

uint32_t SHAMD5IntStatus(uint32_t base, bool flag);

void SHAMD5IntDisable(uint32_t base, uint32_t state);

void MAP_SHAMD5IntDisable(uint32_t base, uint32_t state);

void SHAMD5IntEnable(uint32_t base, uint32_t flags);

void SHAMD5DataLengthSet(uint32_t base, uint32_t length);

void SHAMD5DataWrite(uint32_t base, uint8_t *pSource);

void SHAMD5ResultRead(uint32_t base, uint8_t *pDest);

#endif /* __SIMULATOR_H_ */
