/*
 Copyright (C) 2012 - 2015 Nils Weiss, Patrick Bruenn.

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

#include "wy_I2c.h"
#include "osi.h"
#include "i2c.h"
#include "hw_memmap.h"
#include "prcm.h"
#include "trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR |
ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const uint32_t I2c::TIMEOUT = 0x7D;
const uint32_t I2c::I2C_MRIS_CLKTOUT = 0x2;
const uint32_t I2c::I2C_BASE = I2CA0_BASE;
const uint32_t I2c::SYS_CLK = 80000000;

I2c::I2c(const enum mode& m) {

	PRCMPeripheralClkEnable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
	PRCMPeripheralReset(PRCM_I2CA0);

	switch (m) {
	case STANDARD:
		I2CMasterInitExpClk(I2C_BASE, SYS_CLK, false);
		break;

	case FAST:
		I2CMasterInitExpClk(I2C_BASE, SYS_CLK, true);
		break;
	}
	osi_LockObjCreate(&this->accessMutex);
}

I2c& I2c::operator=(I2c&&) {
	return *this;
}

I2c::~I2c(void) {
	osi_LockObjDelete(&this->accessMutex);
	PRCMPeripheralClkDisable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
}

int I2c::write(const uint8_t addr, uint8_t const* const data, const size_t len,
		const bool stop) {
	osi_LockObjLock(&this->accessMutex, OSI_WAIT_FOREVER);

	osi_LockObjUnlock(&this->accessMutex);
	return 0;

}

int I2c::read(const uint8_t addr, uint8_t* const data, const size_t len) {
	osi_LockObjLock(&this->accessMutex, OSI_WAIT_FOREVER);

	osi_LockObjUnlock(&this->accessMutex);
	return 0;
}

int I2c::transact(const uint32_t cmd)
{

	I2CMasterIntClearEx(I2C_BASE, I2CMasterIntStatusEx(I2C_BASE,false));

	I2CMasterTimeoutSet(I2C_BASE, TIMEOUT);

	I2CMasterControl(I2C_BASE, cmd);

	while((I2CMasterIntStatusEx(I2C_BASE, false) & (I2C_INT_MASTER | I2C_MRIS_CLKTOUT)) == 0)
	{
		osi_Sleep(1);
	}

	if(I2CMasterErr(I2C_BASE) != I2C_MASTER_ERR_NONE)
	{
		switch(cmd)
		{
			case I2C_MASTER_CMD_BURST_SEND_START:
			case I2C_MASTER_CMD_BURST_SEND_CONT:
			case I2C_MASTER_CMD_BURST_SEND_STOP:
			I2CMasterControl(I2C_BASE, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
			break;

			case I2C_MASTER_CMD_BURST_RECEIVE_START:
			case I2C_MASTER_CMD_BURST_RECEIVE_CONT:
			case I2C_MASTER_CMD_BURST_RECEIVE_FINISH:
			I2CMasterControl(I2C_BASE,	I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
			break;

			default:
			break;
		}
		return -1;
	}

	return 0;
}
