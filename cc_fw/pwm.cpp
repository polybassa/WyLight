/*
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

#include <stdint.h>

//Common interface includes
#include "pwm.h"
#include "wy_Pwm.h"
#include <array>

xTaskHandle g_PwmTaskHandle;
xQueueHandle g_PwmMessageQ;

static const size_t NUMBER_OF_PWMS = 3;
static const std::array<const Pwm, NUMBER_OF_PWMS> Pwms = { Pwm(Pwm::CHANNEL5),
                                                            Pwm(Pwm::CHANNEL6), Pwm(Pwm::CHANNEL7) };

void Pwm_TaskInit(void)
{
    for (const auto& pwm : Pwms) {
        pwm.enable();
    }
    for (const auto& pwm : Pwms) {
        pwm = 0;
    }

    g_PwmMessageQ = xQueueCreate(6, NUMBER_OF_PWMS);
}

void Pwm_Task(void* pvParameters)
{
    uint8_t buffer[NUMBER_OF_PWMS];

    for ( ; ; ) {
        xQueueReceive(g_PwmMessageQ, buffer, portMAX_DELAY);
        unsigned int i = 0;
        for (const auto& pwm : Pwms) {
            pwm = (buffer[i++] * (Pwm::DUTYCYCLE_MAX_VALUE - 1)) / UINT8_MAX;
        }
    }
}
