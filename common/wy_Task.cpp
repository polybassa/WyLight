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

#include "wy_Task.h"
#include "trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR |
                                                        ZONE_WARNING | ZONE_VERBOSE | ZONE_INFO;

Task::Task(const char* name, unsigned short stackSize, unsigned long priority,
           std::function<void(const bool&)> function) :
    mTaskFunction(function)
{
    vSemaphoreCreateBinary(this->mStartSemaphore);
    xSemaphoreTake(this->mStartSemaphore, 0);
    vSemaphoreCreateBinary(this->mStopSemaphore);
    xSemaphoreTake(this->mStopSemaphore, 0);

    this->mStopFlag = false;
    xTaskCreate(Task::task, (signed portCHAR*)name,
                Task::STACKSIZE_IN_BYTE(stackSize), this, priority, &this->mHandle);
}

void Task::run(void)
{
    xSemaphoreGive(this->mStartSemaphore);
    Trace(ZONE_INFO, "%s started \r\n", pcTaskGetTaskName(this->mHandle));
}

void Task::stop(void)
{
    this->mStopFlag = true;
    xSemaphoreTake(this->mStopSemaphore, portMAX_DELAY);
    Trace(ZONE_INFO, "%s stopped \r\n", pcTaskGetTaskName(this->mHandle));
}

void Task::task(void* pvParameters)
{
    static_cast<Task*>(pvParameters)->taskFunction();
}

void Task::taskFunction(void)
{
    vTaskDelay(200 / portTICK_RATE_MS);
    for ( ; ; ) {
        xSemaphoreTake(this->mStartSemaphore, portMAX_DELAY);
        this->mStopFlag = false;
        this->mTaskFunction(this->mStopFlag);
        xSemaphoreGive(this->mStopSemaphore);
    }
}
