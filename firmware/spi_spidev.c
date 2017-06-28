/*
   Copyright (C) 2016 Nils Weiss, Patrick Bruenn.

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

#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

#include "trace.h"

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static const char* spidev = "/dev/spidev0.0";
static int g_spi_fd;

void SPI_Init(void)
{
    g_spi_fd = open(spidev, O_RDWR);
    if (-1 == g_spi_fd)
        Trace(ZONE_ERROR, "open '%s' faild with errno: %d\n", spidev, errno);
}

static void SPI_SendBuffer(const uint8_t* buf, const size_t buf_len)
{
    struct spi_ioc_transfer xfer[2];

    memset(xfer, 0, sizeof(xfer));
    xfer[0].len = buf_len;
    xfer[0].tx_buf = (__u64)buf;

    const int status = ioctl(g_spi_fd, SPI_IOC_MESSAGE(2), xfer);
    if (status != buf_len)
        Trace(ZONE_INFO, "%s(): status: %d\n", __func__, status);
}

uint8_t SPI_Send(uint8_t data)
{
    SPI_SendBuffer(&data, sizeof(data));
    return 0;
}

void SPI_SendLedBuffer(uint8_t* buf, uns8 length)
{
    SPI_SendBuffer(buf, length);
}
