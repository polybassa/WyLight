/*
   Copyright (C) 2017 WyLight GbR

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

struct pwm_gpio {
    const uint8_t pin;
    uint8_t duty;
};

static struct pwm_gpio g_pwm[] = {
    {5, 0},
#if 0
    {5, 0},
    {5, 0},
#endif
};

void pwm_task(void* unused)
{
    for (uint8_t counter = 0; ; ++counter) {
        for (uint8_t i = 0; i < ARRAY_SIZE(g_pwm); ++i) {
            gpio_write(g_pwm[i].pin, counter > g_pwm[i].duty);
        }
        Platform_sleep_ms(1);
    }
}

void SPI_Init(void)
{
    for (uint8_t i = 0; i < ARRAY_SIZE(g_pwm); ++i) {
        gpio_enable(g_pwm[i].pin, GPIO_OUTPUT);
        gpio_write(g_pwm[i].pin, 1);
    }
    xTaskCreate(pwm_task, "pwm_task", 256, NULL, 2, NULL);
}

uint8_t SPI_Send(uint8_t data)
{
    SPI_SendLedBuffer(&data, sizeof(data));
    return 0;
}

void SPI_SendLedBuffer(uint8_t* buf, uns8 length)
{
    if (length > ARRAY_SIZE(g_pwm))
        length = ARRAY_SIZE(g_pwm);

    for (uint8_t i = 0; i < length; ++i) {
        g_pwm[i].duty = buf[i];
    }
}
