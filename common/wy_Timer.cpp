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

#include "wy_Timer.h"
#include <algorithm>
#include "hw_memmap.h"
#include "rom_map.h"
#include "timer.h"
#include "prcm.h"

Timer::Timer(const enum base& b, const enum timer& t) : mBase(b), mTimer(t)
{
    baseUseCount[b]++;
    if (baseUseCount[b])
        this->enablePeripheralClk(b);
}

Timer::~Timer(void)
{
    baseUseCount[mBase]--;
    if (baseUseCount[mBase] == 0)
        this->disablePeripheralClk(mBase);
}

uint32_t Timer::getHwBase(const enum base& b)
{
    switch (b) {
    case BASE0:
        return TIMERA0_BASE;

    case BASE1:
        return TIMERA1_BASE;

    case BASE2:
        return TIMERA2_BASE;

    case BASE3:
        return TIMERA3_BASE;
    }

    return 0;
}

uint32_t Timer::getHwTimer(const enum timer& t)
{
    switch (t) {
    case HW_TIMER_A:
        return TIMER_A;

    case HW_TIMER_B:
        return TIMER_B;

    case HW_TIMER_BOTH:
        return TIMER_BOTH;
    }

    return 0;
}

void Timer::setConfiguration(const uint32_t config) const
{
    MAP_TimerConfigure(getHwBase(this->mBase), config);
}

void Timer::setPrescale(const uint32_t prescale) const
{
    MAP_TimerPrescaleSet(getHwBase(this->mBase), getHwTimer(this->mTimer), prescale);
}

void Timer::setControlLevel(const uint32_t level) const
{
    MAP_TimerControlLevel(getHwBase(this->mBase), getHwTimer(this->mTimer), level);
}

void Timer::setLoad(const uint32_t load) const
{
    MAP_TimerLoadSet(getHwBase(this->mBase), getHwTimer(this->mTimer), load);
}

void Timer::setMatch(const uint32_t match) const
{
    MAP_TimerMatchSet(getHwBase(this->mBase), getHwTimer(this->mTimer), match);
}

void Timer::enable(void) const
{
    MAP_TimerEnable(getHwBase(this->mBase), getHwTimer(this->mTimer));
}

void Timer::disable(void) const
{
    MAP_TimerDisable(getHwBase(this->mBase), getHwTimer(this->mTimer));
}

void Timer::enablePeripheralClk(const enum base& b) const
{
    switch (b) {
    case BASE0:
        MAP_PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
        break;

    case BASE1:
        MAP_PRCMPeripheralClkEnable(PRCM_TIMERA1, PRCM_RUN_MODE_CLK);
        break;

    case BASE2:
        MAP_PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
        break;

    case BASE3:
        MAP_PRCMPeripheralClkEnable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);
        break;
    }
}

void Timer::disablePeripheralClk(const enum base& b) const
{
    switch (b) {
    case BASE0:
        MAP_PRCMPeripheralClkDisable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
        break;

    case BASE1:
        MAP_PRCMPeripheralClkDisable(PRCM_TIMERA1, PRCM_RUN_MODE_CLK);
        break;

    case BASE2:
        MAP_PRCMPeripheralClkDisable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
        break;

    case BASE3:
        MAP_PRCMPeripheralClkDisable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);
        break;
    }
}