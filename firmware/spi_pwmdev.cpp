/*
   Copyright (C) 2016 Patrick Bruenn.

   This file is part of WyLight.

   WyLight is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   WyLight is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include <cstdlib>
#include <fstream>
#include <string>
#include "platform.h"

#define NUM_PWM 3

static const char* pinmux_cmds[] = {
    "mt7688_pinmux set uart2 pwm",
    "mt7688_pinmux set pwm0 pwm",
    "mt7688_pinmux set pwm1 pwm"
};

struct PwmPin {
    void open(size_t pwm_pin)
    {
        static const std::string SYSFS_PWM_EXPORT {"/sys/class/pwm/pwmchip0/export"};
        static const std::string SYSFS_PWM {"/sys/class/pwm/pwmchip0/pwm"};
        static const std::string PWM_PERIOD {"1024"};
        const char export_pin = '0' + pwm_pin;

        std::ofstream export_stream(SYSFS_PWM_EXPORT);
        export_stream << export_pin << std::flush;

        std::ofstream period_stream;
        do {
            period_stream.open(SYSFS_PWM + export_pin + "/period");
        } while (!period_stream.good());
        period_stream << PWM_PERIOD << std::flush;

        duty_stream.open(SYSFS_PWM + export_pin + "/duty_cycle");
        enable_stream.open(SYSFS_PWM + export_pin + "/enable");
        write(0);
    }

    void write(const size_t value)
    {
        const char enable = '0' + (!!value);
        char duty[16];

        snprintf(duty, sizeof(duty), "%u\0", 4 * value);
        duty_stream << duty << std::flush;
        enable_stream << enable << std::flush;
    }

private:
    std::ofstream enable_stream;
    std::ofstream duty_stream;
};

struct PwmPin g_pwm[NUM_PWM];

void SPI_Init(void)
{
    for (const auto& cmd: pinmux_cmds) {
        std::system(cmd);
    }

    for (size_t i = 0; i < NUM_PWM; ++i) {
        g_pwm[i].open(i);
    }
}

static void SPI_SendBuffer(const uint8_t* buf, const size_t buf_len)
{
    for (size_t i = 0; i < std::min<size_t>(NUM_PWM, buf_len); ++i) {
        g_pwm[i].write(buf[i]);
    }
}

uint8_t SPI_Send(uint8_t data)
{
    SPI_SendBuffer(&data, sizeof(data));
    return 0;
}

void SPI_SendLedBuffer(uint8_t* buf)
{
    SPI_SendBuffer(buf, 3 * NUM_OF_LED);
}
