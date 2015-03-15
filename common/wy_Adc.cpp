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

#include "wy_Adc.h"
#include "adc.h"
#include "hw_memmap.h"
#include "trace.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR |
                                                        ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

uint8_t Adc::adcUseCount = 0;

Adc::Adc(const enum channel& ch) : mChannel(ch)
{
    if (this->mChannel == INVALID_CHANNEL)
        return;

    if (adcUseCount++) {
        ADCDisable(ADC_BASE);
        ADCChannelEnable(ADC_BASE, getHwChannel(this->mChannel));
    } else {
        ADCChannelEnable(ADC_BASE, getHwChannel(this->mChannel));
        ADCTimerConfig(ADC_BASE, 2 ^ 17);
        ADCTimerEnable(ADC_BASE);
    }
    ADCEnable(ADC_BASE);
}

Adc& Adc::operator=(Adc&& rhs)
{
    return *this;
}

Adc::Adc(Adc&& rhs) : mChannel(rhs.mChannel)
{
    rhs.mChannel = INVALID_CHANNEL;
}

Adc::~Adc(void)
{
    if (this->mChannel == INVALID_CHANNEL)
        return;

    ADCChannelDisable(ADC_BASE, getHwChannel(this->mChannel));
    if ((--adcUseCount) == 0) {
        ADCDisable(ADC_BASE);
        ADCTimerDisable(ADC_BASE);
    }
}

uint32_t Adc::getHwChannel(const enum channel& ch)
{
    switch (ch) {
    case CHANNEL0:
        return ADC_CH_0;

    case CHANNEL1:
        return ADC_CH_1;

    case CHANNEL2:
        return ADC_CH_2;

    case CHANNEL3:
        return ADC_CH_3;

    default:
        return 0;
    }
}

std::pair<uint16_t, uint32_t> Adc::get(void) const
{
    if ((this->mChannel == INVALID_CHANNEL)
        || ((ADCFIFOLvlGet(ADC_BASE, getHwChannel(this->mChannel))) == 0))
        return std::make_pair(0, 0);

    uint32_t sample = ADCFIFORead(ADC_BASE, getHwChannel(this->mChannel));
    return std::make_pair((sample & 0x3fff) >> 2, (sample & 0xffffC000) >> 14);
}
