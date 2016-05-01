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

#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

#include "trace.h"

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static const char* ttydev = "/dev/ttyS0";
static int g_tty_fd;

static int set_interface_attribs(int fd, int speed, int parity)
{
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        Trace(ZONE_ERROR, "error %d from tcgetattr", errno);
        return -1;
    }

    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;         // 8-bit chars
    // disable IGNBRK for mismatched speed tests; otherwise receive break
    // as \000 chars
    tty.c_iflag &= ~IGNBRK;             // disable break processing
    tty.c_lflag = 0;                    // no signaling chars, no echo,
                                        // no canonical processing
    tty.c_oflag = 0;                    // no remapping, no delays
    tty.c_cc[VMIN] = 0;                 // read doesn't block
    tty.c_cc[VTIME] = 5;                // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);     // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);    // ignore modem controls,
                                        // enable reading
    tty.c_cflag &= ~(PARENB | PARODD);          // shut off parity
    tty.c_cflag |= parity;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        Trace(ZONE_ERROR, "error %d from tcsetattr", errno);
        return -1;
    }
    return 0;
}

void SPI_Init(void)
{
    g_tty_fd = open(ttydev, O_RDWR, O_NOCTTY);
    if (-1 == g_tty_fd) {
        Trace(ZONE_ERROR, "open '%s' failed with errno: %d\n", ttydev, errno);
        return;
    }

    if (set_interface_attribs(g_tty_fd, B230400, 0)) {
        Trace(ZONE_ERROR, "set_interface_attribs() faild\n");
        close(g_tty_fd);
        g_tty_fd = 0;
    }
}

static void SPI_SendBuffer(const uint8_t* buf, const size_t buf_len)
{
    size_t written = 0;

    do {
        written += write(g_tty_fd, buf + written, buf_len - written);
    } while (written < buf_len);
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
