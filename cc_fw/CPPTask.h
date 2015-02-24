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

#ifndef __cc_firmware__CPPTask__
#define __cc_firmware__CPPTask__

#include <stdint.h>
#include <stdbool.h>
#include "osi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include <functional>

class Task {
    std::function<void(const bool&)> mTaskFunction;
    void taskFunction(void);
    xTaskHandle mHandle;
    xSemaphoreHandle mStopSemaphore;
    xSemaphoreHandle mStartSemaphore;
    bool mStopFlag;

    static void task(void* pvParameters);

public:
    Task(const char* name, unsigned short stackSize, unsigned long priority, std::function<void(const bool&)> function);
    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&&) = delete;

    virtual void run(void);
    virtual void stop(void);
};

#endif /* defined(__cc_firmware__Task__) */