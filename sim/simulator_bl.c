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

#include "simulator.h"
#include "shamd5.h"
#include "openssl/sha.h"
#include "rom_map.h"
#include "prcm.h"

#include <stdio.h>
#include <stdint.h>

extern void SHAMD5IntHandler(void);

#define HASHBLOCK   64

void _SlNonOsMainLoopTask(void) {}

void PRCMCC3200MCUInit(void) {}
void PRCMPeripheralClkEnable(unsigned long ulPeripheral, unsigned long ulClkFlags) {}
void PRCMPeripheralClkDisable(unsigned long ulPeripheral, unsigned long ulClkFlags){}
void PRCMPeripheralReset(unsigned long ulPeripheral) {}

void(*const g_pfnVectors[256]) (void);

long Network_IF_StartSimpleLinkAsAP(void)
{
    printf("Start SimpleLink as AP\r\n");
    return SUCCESS;
}

void Network_IF_InitDriver(void)
{
    printf("Configured SimpleLink in default state \n\r");
}

void Network_IF_DeInitDriver(void)
{
    printf("SL Disconnect...\n\r");
}

uint32_t SHAMD5IntStatus(uint32_t base, bool flag)
{
    return SHAMD5_INT_CONTEXT_READY;
}

void SHAMD5IntDisable(uint32_t base, uint32_t state) {}

void SHAMD5IntEnable(uint32_t base, uint32_t flags)
{
    SHAMD5IntHandler();
}

bool SHAMD5DataProcess(uint32_t ui32Base, uint8_t* pui8DataSrc,
                       uint32_t ui32DataLength, uint8_t* pui8HashResult)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, pui8DataSrc, ui32DataLength);
    SHA256_Final(pui8HashResult, &sha256);
    return true;
}
