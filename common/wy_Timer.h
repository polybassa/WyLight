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

#ifndef __wy__Timer__
#define __wy__Timer__

#include <stdint.h>
#include <array>

class Timer {
public:
    enum base {
        BASE0 = 0,
        BASE1,
        BASE2,
        BASE3
    };

    enum timer {
        HW_TIMER_A = 0,
        HW_TIMER_B,
        HW_TIMER_BOTH
    };

    Timer(const enum base&, const enum timer&);
    Timer& operator=(const Timer&) = delete;
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    ~Timer(void);

    void setConfiguration(const uint32_t config) const;
    void setPrescale(const uint32_t prescale) const;
    void setControlLevel(const uint32_t level) const;
    void setLoad(const uint32_t load) const;
    void setMatch(const uint32_t match) const;
    void enable(void) const;
    void disable(void) const;

private:
    static uint32_t getHwBase(const enum base& b);
    static uint32_t getHwTimer(const enum timer& t);

    static const uint8_t NUMBER_OF_TIMERS = 4;
    static const uint8_t MAX_USE_COUNT = 2;
    static std::array<uint8_t, NUMBER_OF_TIMERS> baseUseCount;

    void enablePeripheralClk(const enum base& b) const;
    void disablePeripheralClk(const enum base& b) const;

    inline uint32_t hwBase(void) const;
    inline uint32_t hwTimer(void) const;

    const enum base mBase;
    const enum timer mTimer;
};

#endif /* defined(__wy__Timer__) */
