/*
   Copyright (C) 2016 Patrick Bruenn.

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

//#include <stdlib>
//#include <fstream>
//#include <string>
#include "platform.h"

#define NUM_PWM 1

void SPI_Init(void)
{
    static const uint8_t pins[] = {
        5,
        0,
        0,
    };
    static const uint16_t pwm_freq_1kHz = 1000;
    pwm_init(NUM_PWM, pins);
    pwm_set_freq(pwm_freq_1kHz);
    pwm_set_duty(UINT16_MAX / 2);
    pwm_start();
}

static void SPI_SendBuffer(const uint8_t* buf, size_t buf_len)
{
    if (buf_len > NUM_PWM)
        buf_len = NUM_PWM;
    for (size_t i = 0; i < buf_len; ++i) {
        pwm_set_duty(buf[i] << 8);
    }
}

uint8_t SPI_Send(uint8_t data)
{
    SPI_SendBuffer(&data, sizeof(data));
    return 0;
}

void SPI_SendLedBuffer(uint8_t* buf, uns8 length)
{
    SPI_SendBuffer(buf, length);
}
