/**
   Copyright (C) 2017 Nils Weiss, Patrick Bruenn.

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

#include "platform.h"
#include "RingBuf.h"
#include "trace.h"

//TODO REMOVE THIS HACK BY MAKING A CLEAN MAKEFILE FOR ESP!!!!
#if !defined(__XTENSA_EL__)
static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;
#endif
static const uint16_t WIFLY_SERVER_PORT = 2000;
static int g_uartSocket = -1;

static Platform_Mutex g_ring_mutex;

int i = 0;

static Platform_ThreadFunc UART_TcpRecv(void* unused)
{
    const int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenSocket) {
        Trace(ZONE_ERROR, "create listen socket failed\n");
        Platform_FatalError();
    }

    struct sockaddr_in udp_sock_addr;
    udp_sock_addr.sin_family = AF_INET;
    udp_sock_addr.sin_port = htons(WIFLY_SERVER_PORT);
    udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (0 != bind(listenSocket, (const struct sockaddr*)&udp_sock_addr, sizeof(udp_sock_addr))) {
        Trace(ZONE_ERROR, "bind() failed\n");
        Platform_FatalError();
    }

    if (0 != listen(listenSocket, 0)) {
        Trace(ZONE_ERROR, "listen() failed\n");
        Platform_FatalError();
    }

    for (g_uartSocket = accept(listenSocket, NULL, NULL); ; ) {
        int bytesRead;
        do {
            uns8 buf[16];
            bytesRead = recv(g_uartSocket, buf, sizeof(buf), 0);
            Trace(ZONE_VERBOSE, "%d bytesRead\n", bytesRead);

            Platform_MutexLock(&g_ring_mutex);
            for (int i = 0; i < bytesRead; i++) {
                if (!RingBuf_HasError(&g_RingBuf))
                    RingBuf_Put(&g_RingBuf, buf[i]);
            }
            Platform_MutexUnlock(&g_ring_mutex);
        } while (bytesRead > 0);

        // don't allow immediate reconnection
        Platform_sleep_ms(1000);
    }
}

static Platform_ThreadFunc UART_UdpRecv(void* param)
{
    const int listenSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == listenSocket) {
        Trace(ZONE_ERROR, "create listen socket failed\n");
        Platform_FatalError();
    }

    struct sockaddr_in udp_sock_addr;
    udp_sock_addr.sin_family = AF_INET;
    udp_sock_addr.sin_port = htons(WIFLY_SERVER_PORT);
    udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (0 != bind(listenSocket, (const struct sockaddr*)&udp_sock_addr, sizeof(udp_sock_addr))) {
        Trace(ZONE_ERROR, "bind() failed\n");
        Platform_FatalError();
    }

    for ( ; ; ) {
        int bytesRead;
        do {
            uns8 buf[128];
            bytesRead = recvfrom(listenSocket, buf, sizeof(buf), 0, NULL, NULL);
            Trace(ZONE_VERBOSE, "%d bytesRead\n", bytesRead);

            Platform_MutexLock(&g_ring_mutex);
            for (int i = 0; i < bytesRead; i++) {
                if (!RingBuf_HasError(&g_RingBuf))
                    RingBuf_Put(&g_RingBuf, buf[i]);
            }
            Platform_MutexUnlock(&g_ring_mutex);
        } while (bytesRead > 0);
        // don't allow immediate reconnection
        Platform_sleep_ms(1000);
    }
}

void UART_Init()
{
    static Platform_Thread tcp;
    static Platform_Thread udp;

    g_ring_mutex = Platform_MutexInit();
    Platform_CreateThread(&UART_TcpRecv, 256, &g_ring_mutex, 2, &tcp);
    Platform_CreateThread(&UART_UdpRecv, 256, &g_ring_mutex, 2, &udp);
}

void UART_Send(uns8 ch)
{
    Trace(ZONE_VERBOSE, "0x%02x(%c)\n", ch, ch);
    send(g_uartSocket, &ch, sizeof(ch), 0);
}
