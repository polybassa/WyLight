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

#ifndef __wy__LockGuard__
#define __wy__LockGuard__

#include "FreeRTOS.h"
#include "semphr.h"

class LockGuard {
public:

    explicit LockGuard(const xSemaphoreHandle& lock, portTickType ticksToWait = portMAX_DELAY) : mLock(lock)
    {
        if (this->mLock)
            this->mSemaphoreObtained = (bool)xSemaphoreTake(this->mLock, ticksToWait);
    }

    ~LockGuard()
    {
        if (this->mLock)
            xSemaphoreGive(this->mLock);
    }

    LockGuard(const LockGuard&) = delete;
    LockGuard& operator=(const LockGuard&) = delete;

    operator bool(void) const {
        return mSemaphoreObtained;
    }

private:
    const xSemaphoreHandle& mLock;
    bool mSemaphoreObtained = false;
};

#endif /* defined(__wy__LockGuard__) */
