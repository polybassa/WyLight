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

#include "platform.h"

#define NUM_PWM 0 //1

void SPI_Init(void)
{
#if NUM_PWM
    static const uint8_t pins[] = {
        5,
        0,
        0,
    };
    static const uint16_t pwm_freq_1kHz = 1000;
    pwm_init(NUM_PWM, pins);
    pwm_set_freq(pwm_freq_1kHz);
    pwm_set_duty(0);
    pwm_start();
#else
    gpio_enable(5, GPIO_OUTPUT);
    gpio_write(5, 1);
#endif
}

static void SPI_SendBuffer(const uint8_t* buf, size_t buf_len)
{
    static const size_t periode = 2;
    static size_t counter = periode;
    static uint16_t toggle = 1;

#if NUM_PWM
    if (buf_len > NUM_PWM)
        buf_len = NUM_PWM;
    for (size_t i = 0; i < buf_len; ++i) {
        pwm_set_duty(buf[i] << 8);
    }
#else
#if 0
    if (!--counter) {
        counter = periode;
        toggle = !toggle;
        gpio_write(5, toggle);
    }
#endif
#endif
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
