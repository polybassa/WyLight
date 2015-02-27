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


#ifndef __cc_fw__Pwm__
#define __cc_fw__Pwm__

#include <stdint.h>

class Timer {
    
    public enum base {
        BASE0 = 0,
        BASE1,
        BASE2,
        BASE3
    };
    
    public enum timer {
        HW_TIMER_A = 0,
        HW_TIMER_B,
        HW_TIMER_BOTH
    };
    
    static const uint8_t NUMBER_OF_TIMERS = 4;
    static const uint8_t MAX_USE_COUNT = 2;
    static uint8_t baseUseCount[NUMBER_OF_TIMERS];
    
    static uint32_t getHwBase(const enum base& b);
    static uint32_t getHwTimer(const enum timer& t);
    
    const enum base mBase;
    const enum timer mTimer;
    
    void enablePeripheralClk(const enum base& b) const;
    void disablePeripheralClk(const enum base& b) const;
    
public:
    
    Timer(const enum base&, const enum timer&);
    Timer& operator=(const Timer&) = delete;
    Timer(const Timer&) = delete;
    Timer(Timer&&) = delete;
    ~Timer(void);
    
    void setConfiguration(const uint32_t config) const;
    void setPrescale(const uint32_t prescale) const;
    void setControlLevel(const uint32_t level) const;
    void setLoad(const uint32_t load) const;
    void setMatch(const uint32_t match);
    
};

class Pwm : Timer {
    
    static const uint32_t CPU_FREQUENCY_HZ = 80000000;
    static const uint32_t PWM_FREQUENCY_HZ = 2000;
    static const uint16_t DUTYCYCLE_MAX_VALUE = 1000;
    static const uint32_t DUTYCYCLE_GRANULARITY = (uint32_t)(CPU_FREQUENCY_HZ / PWM_FREQUENCY_HZ / DUTYCYCLE_MAX_VALUE);
    static const uint32_t TIMER_INTERVAL_RELOAD = (uint32_t)(PWM_MAX_VALUE * DUTYCYCLE_GRANULARITY);
    
    static enum Timer::timer getTimer(const enum channels& channel);
    static enum Timer::base getBase(const enum channels& channel);
    
    uint16_t mDutyCycle = 0;
    
    void setupTimerToPwmMode(void) const;
    
public:
    enum channels
    {
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
};

#endif /* defined(__cc_fw__Pwm__) */
