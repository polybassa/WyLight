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

static uint8_t g_pwm_duty = 0;

void pwm_task(void* unused)
{
    for (uint8_t counter = 0; ; ++counter) {
        gpio_write(5, counter > g_pwm_duty);
        Platform_sleep_ms(1);
    }
}

void SPI_Init(void)
{
    gpio_enable(5, GPIO_OUTPUT);
    gpio_write(5, 1);
    xTaskCreate(pwm_task, "pwm_task", 256, NULL, 2, NULL);
}

static void SPI_SendBuffer(const uint8_t* buf, size_t buf_len)
{
    g_pwm_duty = *buf;
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
