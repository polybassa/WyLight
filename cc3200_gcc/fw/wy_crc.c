/**
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#include "wy_crc.h"
#include "hw_dthe.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_ints.h"
#include "hw_types.h"
#include "rom.h"
#include "rom_map.h"
#include "../../driverlib/crc.h"
#include "prcm.h"

// 16-bit CCIT CRC
void Crc_AddCrc(const uns8 byte, uns8 *p_crcH, uns8 *p_crcL) {
	MAP_CRCDataWrite(DTHE_BASE, byte);
	uint32_t result = MAP_CRCResultRead(DTHE_BASE);
	*p_crcL = (uint8_t)result;
	result = result >> 8;
	*p_crcH = (uint8_t)result;
}

void Crc_NewCrc(uns8 *p_crcH, uns8 *p_crcL) {
	if (!p_crcH) return;
	if (!p_crcL) return;
	// bootloader is using 0x0000 as initial crc not 0xFFFF
	*p_crcH = 0x00;
	*p_crcL = 0x00;
	MAP_CRCSeedSet(DTHE_BASE, 0);
}

void Crc_Init(void) {
	MAP_PRCMPeripheralClkEnable(PRCM_DTHE, PRCM_RUN_MODE_CLK);
	MAP_PRCMPeripheralReset(PRCM_DTHE);

	MAP_CRCConfigSet(DTHE_BASE, (CRC_CFG_INIT_SEED | CRC_CFG_SIZE_8BIT | CRC_CFG_TYPE_P1021));
}
