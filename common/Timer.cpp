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

#include "Pwm.h"
#include <algorithm>
#include "hw_memmap.h"
#include "rom_map.h"
#include "timer.h"
#include "prcm.h"
    
Timer::Timer(const enum base& b, const enum timer& t) : mBase(b), mTimer(t) {
    baseUseCount[b]++;
    if (baseUseCount[b]) {
        this->enablePeripheralClk(b);
    }
}

Timer::~Timer(void) {
    baseUseCount[mBase]--;
    if (baseUseCount[mBase] == 0) {
        this->disablePeripheralClk(mBase);
    }
}

uint32_t Timer::getHwBase(const enum base& b){
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
}

uint32_t Timer::getHwTimer(const enum timer& t) {
    switch (t) {
        case HW_TIMER_A:
            return TIMER_A;
        case HW_TIMER_B:
            return TIMER_B;
        case HW_TIMER_BOTH:
            return TIMER_BOTH;
    }
}
    
void Timer::setConfiguration(const uint32_t config) const {
    MAP_TimerConfigure(getHwBase(this->mBase), config);
}

void Timer::setPrescale(const uint32_t prescale) const {
    MAP_TimerPrescaleSet(getHwBase(this->mBase), getHwTimer(this->mTimer), prescale);
}

void Timer::setControlLevel(const uint32_t level) const {
     MAP_TimerControlLevel(getHwBase(this->mBase), getHwTimer(this->mTimer), level);
}

void Timer::setLoad(const uint32_t load) const {
    MAP_TimerLoadSet(getHwBase(this->mBase), getHwTimer(this->mTimer), load);
}

void Timer::setMatch(const uint32_t match) const {
    MAP_TimerMatchSet(getHwBase(this->mBase), getHwTimer(this->mTimer), match);
}

void Timer::enablePeripheralClk(const enum base& b) const {
    switch(b){
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
    };
}

void Timer::disablePeripheralClk(const enum base& b) const {
    switch(b){
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
    };
}



Pwm::Pwm(const enum channels& channel)
    : Timer(getBase(channel), getTimer(channel)) {
    this->setupTimerToPwmMode();
    MAP_TimerEnable(this->mBase, this->mTimer);
}

Pwm::~Pwm(void) {
    MAP_TimerDisable(this->mBase, this->mTimer);
}

enum Timer::timer Pwm::getTimer(const enum channels& channel) {
    switch(channel){
        case CHANNEL0:
        case CHANNEL2:
        case CHANNEL4:
        case CHANNEL6:
            return TIMER_A;
        case CHANNEL1:
        case CHANNEL3:
        case CHANNEL5:
        case CHANNEL7:
            return TIMER_B;
    };

}

enum Timer::base Pwm::getBase(const enum channels& channel) {
    switch(channel){
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
    };
}

void Pwm::setupTimerToPwmMode(void) const {
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

Pwm& Pwm::operator=(const uint16_t& dutyCycle) {
    this->mDutyCycle.setDutyCycle(dutyCycle);
    return *this;
}

void setDutyCycle(const uint16_t& dutyCycle) {
    this->mDutyCycle = std::min(DUTYCYCLE_MAX_VALUE, dutyCycle);
    this->setMatch(this->mDutyCycle * DUTYCYCLE_GRANULARITY);
}

uint16_t getDutyCycle(void) const {
    return this->mDutyCycle;
}

