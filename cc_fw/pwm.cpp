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

//Application Includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "wy_Pwm.h"

static xTaskHandle g_PwmTaskHandle;
static xQueueHandle g_PwmMessageQ;

OsiTaskHandle* PwmTaskHandle = &g_PwmTaskHandle;
OsiMsgQ_t* PwmMessageQ = &g_PwmMessageQ;

void Pwm_TaskInit(void)
{
    osi_MsgQCreate(PwmMessageQ, (char*)"PwmMsgQ", 3, 6);
}

void Pwm_Task(void* pvParameters)
{
    uint8_t buffer[3];
    Pwm p5(Pwm::CHANNEL5), p6(Pwm::CHANNEL6), p7(Pwm::CHANNEL7);

    for ( ; ; ) {
        osi_MsgQRead(PwmMessageQ, buffer, OSI_WAIT_FOREVER);
        p5.setDutyCycle(buffer[1]);
        p6.setDutyCycle(buffer[2]);
        p7.setDutyCycle(buffer[0]);
    }
}
