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

#ifndef __wy__Adc__
#define __wy__Adc__

#include <stdint.h>
#include <utility>

class Adc {
public:
    enum channel {
        CHANNEL0 = 0,
        CHANNEL1,
        CHANNEL2,
        CHANNEL3,
        INVALID_CHANNEL
    };

    Adc(const enum channel&);
    Adc& operator=(const Adc&) = delete;
    Adc& operator=(Adc&&);
    Adc(const Adc&) = delete;
    Adc(Adc&&);
    ~Adc(void);

    std::pair<uint16_t, uint32_t> get(void) const;

private:
    static uint8_t adcUseCount;

    static uint32_t getHwChannel(const enum channel&);

    enum channel mChannel;
};

#endif /* defined(__wy__Adc__) */
