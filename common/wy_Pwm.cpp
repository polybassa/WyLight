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

#include "wy_Pwm.h"
#include <algorithm>
#include "timer.h"

Pwm::Pwm(const enum channels& channel)
    : Timer(getBase(channel), getTimer(channel))
{
    this->setupTimerToPwmMode();
    this->enable();
}

Pwm::~Pwm(void)
{
    this->disable();
}

enum Timer::timer Pwm::getTimer(const enum channels& channel)
{
    switch (channel) {
    case CHANNEL0:
    case CHANNEL2:
    case CHANNEL4:
    case CHANNEL6:
        return HW_TIMER_A;

    case CHANNEL1:
    case CHANNEL3:
    case CHANNEL5:
    case CHANNEL7:
        return HW_TIMER_B;
    }

    return HW_TIMER_A;
}

enum Timer::base Pwm::getBase(const enum channels& channel)
{
    switch (channel) {
    case CHANNEL0:
    case CHANNEL1:
        return BASE0;

    case CHANNEL2:
    case CHANNEL3:
        return BASE1;

    case CHANNEL4:
    case CHANNEL5:
        return BASE2;

    case CHANNEL6:
    case CHANNEL7:
        return BASE3;
    }

    return BASE0;
}

void Pwm::setupTimerToPwmMode(void) const
{
    // Set GPT - Configured Timer in PWM mode.
    this->setConfiguration(TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);
    this->setPrescale(0);

    // Inverting the timer output
    this->setControlLevel(1);

    // Load value set to ~0.5 ms time period
    this->setLoad(TIMER_INTERVAL_RELOAD);

    // Match value set so as to output level 0
    this->setMatch(TIMER_INTERVAL_RELOAD);
}

Pwm& Pwm::operator=(const uint16_t& dutyCycle)
{
    this->setDutyCycle(dutyCycle);
    return *this;
}

void Pwm::setDutyCycle(const uint16_t& dutyCycle)
{
    this->mDutyCycle = std::min(DUTYCYCLE_MAX_VALUE, dutyCycle);
    this->setMatch(this->mDutyCycle * DUTYCYCLE_GRANULARITY);
}

uint16_t Pwm::getDutyCycle(void) const
{
    return this->mDutyCycle;
}