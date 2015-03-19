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

#ifndef __wy__I2c__
#define __wy__I2c__

#include <stdint.h>
#include <stddef.h>
#include "FreeRTOS.h"
#include "semphr.h"

class I2c {
public:
    enum mode {
        STANDARD, FAST
    };

    I2c(const enum mode&);
    I2c& operator=(const I2c&) = delete;
    I2c& operator=(I2c&&);
    I2c(const I2c&) = delete;
    ~I2c(void);

    bool write(const uint8_t addr, uint8_t const* const data, const size_t len,
               const bool stop);
    bool read(const uint8_t addr, uint8_t* const data, const size_t len);

private:
    static const uint32_t TIMEOUT;
    static const uint32_t I2C_MRIS_CLKTOUT;
    static const uint32_t I2C_BASE;
    static const uint32_t SYS_CLK;

    xSemaphoreHandle accessMutex;

    bool transact(const uint32_t cmd);
};

#endif /* defined(__wy__I2c__) */
