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
#include "wy_LockGuard.h"
#include "i2c.h"
#include "hw_memmap.h"
#include "prcm.h"
#include "trace.h"
#include "FreeRTOS.h"
#include "task.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR |
                                                        ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const uint32_t I2c::TIMEOUT = 0x7D;
const uint32_t I2c::I2C_MRIS_CLKTOUT = 0x2;
const uint32_t I2c::I2C_BASE = I2CA0_BASE;
const uint32_t I2c::SYS_CLK = 80000000;

I2c::I2c(const enum mode& m)
{
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

    this->accessMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(this->accessMutex);
}

I2c& I2c::operator=(I2c&&)
{
    return *this;
}

I2c::~I2c(void)
{
    vSemaphoreDelete(this->accessMutex);
    PRCMPeripheralClkDisable(PRCM_I2CA0, PRCM_RUN_MODE_CLK);
}

bool I2c::write(const uint8_t addr, uint8_t const* const data, const size_t len,
                const bool stop)
{
    const LockGuard lock(this->accessMutex);
    size_t index = 0;

    if ((data == nullptr) || (len == 0))
        return true;

    I2CMasterSlaveAddrSet(I2C_BASE, addr, false);
    I2CMasterDataPut(I2C_BASE, data[index++]);

    if (!this->transact(I2C_MASTER_CMD_BURST_SEND_START))
        return false;

    while (index < len) {
        I2CMasterDataPut(I2C_BASE, data[index++]);
        if (!this->transact(I2C_MASTER_CMD_BURST_SEND_CONT))
            return false;
    }

    if (stop && !this->transact(I2C_MASTER_CMD_BURST_SEND_STOP))
        return false;

    return true;
}

bool I2c::read(const uint8_t addr, uint8_t* const data, const size_t len)
{
    const LockGuard lock(this->accessMutex);

    if ((data == nullptr) || (len == 0))
        return true;

    I2CMasterSlaveAddrSet(I2C_BASE, addr, true);

    if (len == 1) {
        if (!this->transact(I2C_MASTER_CMD_SINGLE_RECEIVE))
            return false;
    } else {
        if (!this->transact(I2C_MASTER_CMD_BURST_RECEIVE_START))
            return false;
    }

    size_t index = 0;
    while (index < len) {
        data[index++] = I2CMasterDataGet(I2C_BASE);

        if (index < len)
            if (!this->transact(I2C_MASTER_CMD_BURST_RECEIVE_CONT))
                return false;
    }
    if (!this->transact(I2C_MASTER_CMD_BURST_RECEIVE_FINISH))
        return false;
    data[index++] = I2CMasterDataGet(I2C_BASE);

    return true;
}

bool I2c::transact(const uint32_t cmd)
{
    I2CMasterIntClearEx(I2C_BASE, I2CMasterIntStatusEx(I2C_BASE, false));

    I2CMasterTimeoutSet(I2C_BASE, TIMEOUT);

    I2CMasterControl(I2C_BASE, cmd);

    while ((I2CMasterIntStatusEx(I2C_BASE, false) &
            (I2C_INT_MASTER | I2C_MRIS_CLKTOUT)) == 0)
    {
        vTaskDelay(1 / portTICK_RATE_MS);
    }

    if (I2CMasterErr(I2C_BASE) != I2C_MASTER_ERR_NONE)
        switch (cmd) {
        case I2C_MASTER_CMD_BURST_SEND_START:
        case I2C_MASTER_CMD_BURST_SEND_CONT:
        case I2C_MASTER_CMD_BURST_SEND_STOP:
            I2CMasterControl(I2C_BASE, I2C_MASTER_CMD_BURST_SEND_ERROR_STOP);
            return false;

        case I2C_MASTER_CMD_BURST_RECEIVE_START:
        case I2C_MASTER_CMD_BURST_RECEIVE_CONT:
        case I2C_MASTER_CMD_BURST_RECEIVE_FINISH:
            I2CMasterControl(I2C_BASE, I2C_MASTER_CMD_BURST_RECEIVE_ERROR_STOP);
            return false;

        default:
            return false;
        }

    return true;
}
