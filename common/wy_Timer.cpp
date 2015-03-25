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
#include "timer.h"
#include "prcm.h"
#include "trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR |
                                                        ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

std::array<uint8_t, Timer::NUMBER_OF_TIMERS> Timer::baseUseCount = { 0 };

Timer::Timer(const enum base& b, const enum timer& t) : mBase(b), mTimer(t)
{
    if ((this->mBase == INVALID_BASE) || (this->mTimer == INVALID_HW_TIMER))
        return;

    baseUseCount[b]++;
    if (Timer::baseUseCount[b])
        this->enablePeripheralClk(b);

    if (Timer::baseUseCount[b] > Timer::MAX_USE_COUNT)
        Trace(ZONE_ERROR, "Can not create more than two instances!!");

    Trace(ZONE_INFO, "Base: %d UseCount: %d", b, baseUseCount[b]);
}

Timer& Timer::operator=(Timer&& rhs)
{
    return *this;
}

Timer::Timer(Timer&& rhs) : mBase(rhs.mBase), mTimer(rhs.mTimer)
{
    rhs.mBase = INVALID_BASE;
    rhs.mTimer = INVALID_HW_TIMER;
}

Timer::~Timer(void)
{
    if ((this->mBase == INVALID_BASE) || (this->mTimer == INVALID_HW_TIMER))
        return;

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

    default:
        return 0;
    }
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

    default:
        return 0;
    }
}

uint32_t Timer::hwBase(void) const
{
    return getHwBase(this->mBase);
}

uint32_t Timer::hwTimer(void) const
{
    return getHwTimer(this->mTimer);
}

void Timer::setConfiguration(const uint32_t config) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerConfigure(this->hwBase(), config);
}

void Timer::setPrescale(const uint32_t prescale) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerPrescaleSet(this->hwBase(), this->hwTimer(), prescale);
}

void Timer::setControlLevel(const uint32_t level) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerControlLevel(this->hwBase(), this->hwTimer(), level);
}

void Timer::setLoad(const uint32_t load) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerLoadSet(this->hwBase(), this->hwTimer(), load);
}

void Timer::setMatch(const uint32_t match) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerMatchSet(this->hwBase(), this->hwTimer(), match);
}

uint32_t Timer::getMatch(void) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        return TimerMatchGet(this->hwBase(), this->hwTimer());
    else
        return 0;
}

void Timer::enable(void) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerEnable(this->hwBase(), this->hwTimer());
}

void Timer::disable(void) const
{
    if ((this->mBase != INVALID_BASE) && (this->mTimer != INVALID_HW_TIMER))
        TimerDisable(this->hwBase(), this->hwTimer());
}

void Timer::enablePeripheralClk(const enum base& b) const
{
    switch (b) {
    case BASE0:
        PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
        break;

    case BASE1:
        PRCMPeripheralClkEnable(PRCM_TIMERA1, PRCM_RUN_MODE_CLK);
        break;

    case BASE2:
        PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
        break;

    case BASE3:
        PRCMPeripheralClkEnable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);
        break;

    case INVALID_BASE:
        break;
    }
}

void Timer::disablePeripheralClk(const enum base& b) const
{
    switch (b) {
    case BASE0:
        PRCMPeripheralClkDisable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
        break;

    case BASE1:
        PRCMPeripheralClkDisable(PRCM_TIMERA1, PRCM_RUN_MODE_CLK);
        break;

    case BASE2:
        PRCMPeripheralClkDisable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
        break;

    case BASE3:
        PRCMPeripheralClkDisable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);
        break;

    case INVALID_BASE:
        break;
    }
}
