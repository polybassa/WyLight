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

#ifndef __wy__Pwm__
#define __wy__Pwm__

#include <stdint.h>
#include <wy_Timer.h>

class Pwm : protected Timer {
public:
    enum channels {
        CHANNEL0 = 0,
        CHANNEL1,
        CHANNEL2,
        CHANNEL3,
        CHANNEL4,
        CHANNEL5,
        CHANNEL6,
        CHANNEL7
    };

    Pwm(const enum channels& channel);
    Pwm& operator=(const Pwm&) = delete;
    Pwm(const Pwm&) = delete;
    Pwm(Pwm&&) = delete;
    ~Pwm(void);

    void setDutyCycle(const uint16_t& dutyCycle);
    uint16_t getDutyCycle(void) const;

    Pwm& operator=(const uint16_t& dutyCycle);

private:
    static const uint32_t CPU_FREQUENCY_HZ;
    static const uint32_t PWM_FREQUENCY_HZ;
    static const uint16_t DUTYCYCLE_MAX_VALUE;
    static const uint32_t DUTYCYCLE_GRANULARITY;
    static const uint32_t TIMER_INTERVAL_RELOAD;

    static enum Timer::timer getTimer(const enum Pwm::channels& channel);
    static enum Timer::base getBase(const enum Pwm::channels& channel);

    uint16_t mDutyCycle = 0;

    void setupTimerToPwmMode(void) const;
};

#endif /* defined(__wy__Pwm__) */
