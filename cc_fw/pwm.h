/*
   Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef __PWM_H_
#define __PWM_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

extern xTaskHandle g_PwmTaskHandle;
extern xQueueHandle g_PwmMessageQ;

void Pwm_TaskInit(void);
void Pwm_Task(void* pvParameters);

#endif
