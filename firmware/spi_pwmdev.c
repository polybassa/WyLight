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

#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h> // for sleep()

#include "trace.h"

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

#define NUM_PWM 4
static const char* exportdev = "/sys/class/pwm/pwmchip0/export";

static const char* pwm_period[NUM_PWM] = {
    "/sys/class/pwm/pwmchip0/pwm0/period",
    "/sys/class/pwm/pwmchip0/pwm1/period",
    "/sys/class/pwm/pwmchip0/pwm2/period",
    "/sys/class/pwm/pwmchip0/pwm3/period"
};

static const char* pwm_enable[NUM_PWM] = {
    "/sys/class/pwm/pwmchip0/pwm0/enable",
    "/sys/class/pwm/pwmchip0/pwm1/enable",
    "/sys/class/pwm/pwmchip0/pwm2/enable",
    "/sys/class/pwm/pwmchip0/pwm3/enable",
};

static const char* pwm_duty[NUM_PWM] = {
    "/sys/class/pwm/pwmchip0/pwm0/duty_cycle",
    "/sys/class/pwm/pwmchip0/pwm1/duty_cycle",
    "/sys/class/pwm/pwmchip0/pwm2/duty_cycle",
    "/sys/class/pwm/pwmchip0/pwm3/duty_cycle",
};

static const char* pinmux_cmds[] = {
    "mt7688_pinmux set uart2 pwm",
    "mt7688_pinmux set pwm0 pwm",
    "mt7688_pinmux set pwm1 pwm"
};

struct pwm {
    int enable_fd;
    int duty_fd;
};

struct pwm g_pwm[NUM_PWM];

static void pwm_write(const size_t value, size_t pwm)
{
    const char enable = '0' + (!!value);
    char duty[16];

    snprintf(duty, sizeof(duty), "%u\0", 4 * value);
    write(g_pwm[pwm].duty_fd, duty, strlen(duty));
    write(g_pwm[pwm].enable_fd, &enable, sizeof(enable));
}

static void init_pwm(size_t pwm, int export_fd)
{
    const char period[] = "1024";
    const char export = '0' + pwm;

    write(export_fd, &export, sizeof(export));

    sleep(1);
    //TODO wait until file available

    const int period_fd = open(pwm_period[pwm], O_WRONLY);
    if (-1 == period_fd) {
        Trace(ZONE_ERROR, "open '%s' failed with errno: %d\n", pwm_period[pwm], errno);
        return;
    }
    write(period_fd, period, sizeof(period));
    close(period_fd);

    g_pwm[pwm].duty_fd = open(pwm_duty[pwm], O_WRONLY);
    if (-1 == g_pwm[pwm].duty_fd) {
        Trace(ZONE_ERROR, "open '%s' failed with errno: %d\n", pwm_duty[pwm], errno);
        return;
    }

    g_pwm[pwm].enable_fd = open(pwm_enable[pwm], O_WRONLY);
    if (-1 == g_pwm[pwm].enable_fd) {
        Trace(ZONE_ERROR, "open '%s' failed with errno: %d\n", g_pwm[pwm].enable_fd, errno);
        return;
    }
    pwm_write(0, pwm);
}

void SPI_Init(void)
{
    size_t i;

    for (i = 0; i < ARRAY_SIZE(pinmux_cmds); ++i) {
        system(pinmux_cmds[i]);
    }

    int export_fd = open(exportdev, O_WRONLY);
    if (-1 == export_fd) {
        Trace(ZONE_ERROR, "open '%s' failed with errno: %d\n", exportdev, errno);
        return;
    }

    for (i = 0; i < NUM_PWM; ++i) {
        init_pwm(i, export_fd);
    }
    close(export_fd);
}

static void SPI_SendBuffer(const uint8_t* buf, const size_t buf_len)
{
    if (buf_len >= 3) {
        size_t i;
        for (i = 0; i < 3; ++i) {
            pwm_write(buf[i], i);
        }
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