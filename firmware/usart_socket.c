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
#include "trace.h"

static const uint16_t WIFLY_SERVER_PORT = 2000;
static int g_uartSocket = -1;
static int g_blinky = 0;

int i = 0;
static void UART_InterruptRoutine(void* unused)
{
    while (!wifi_alive) {
        Platform_sleep_ms(1000);
    }
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenSocket) {
        printf("%s:%d %s: create socket failed\n", __FILE__, __LINE__, __FUNCTION__);
        Trace_Blink(100);
    }

    struct sockaddr_in udp_sock_addr;
    udp_sock_addr.sin_family = AF_INET;
    udp_sock_addr.sin_port = htons(WIFLY_SERVER_PORT);
    udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (0 != bind(listenSocket, (struct sockaddr*)&udp_sock_addr, sizeof(udp_sock_addr))) {
        printf("%s:%d %s: bind() failed\n", __FILE__, __LINE__, __FUNCTION__);
        Trace_Blink(100);
        return;
    }

    if (0 != listen(listenSocket, 0)) {
        printf("%s:%d %s: listen() failed\n", __FILE__, __LINE__, __FUNCTION__);
        Trace_Blink(100);
        return;
    }

    g_uartSocket = accept(listenSocket, NULL, NULL);
    for ( ; ; ) {
        int bytesRead;
        do {
            uns8 buf[16];
            bytesRead = recv(g_uartSocket, buf, sizeof(buf), 0);
            printf("%d bytesRead\n", bytesRead);
//            pthread_mutex_lock(&g_ring_mutex);
            int i;
            for (i = 0; i < bytesRead; i++) {
                if (!RingBuf_HasError(&g_RingBuf))
                    RingBuf_Put(&g_RingBuf, buf[i]);
            }
//            pthread_mutex_unlock(&g_ring_mutex);
        } while (bytesRead > 0);
        // don't allow immediate reconnection
        Platform_sleep_ms(1000);
        g_uartSocket = accept(listenSocket, NULL, NULL);
    }
}

void UdpRoutine(void* unused)
{
    int listenSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == listenSocket) {
        printf("%s:%d %s: create socket failed\n", __FILE__, __LINE__, __FUNCTION__);
        Trace_Blink(100);
        return;
    }

    struct sockaddr_in udp_sock_addr;
    udp_sock_addr.sin_family = AF_INET;
    udp_sock_addr.sin_port = htons(WIFLY_SERVER_PORT);
    udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (0 != bind(listenSocket, (struct sockaddr*)&udp_sock_addr, sizeof(udp_sock_addr))) {
        printf("%s:%d %s: bind() failed\n", __FILE__, __LINE__, __FUNCTION__);
        Trace_Blink(100);
        return;
    }

    for ( ; ; ) {
        int bytesRead;
        do {
            uns8 buf[16];
            bytesRead = recvfrom(listenSocket, buf, sizeof(buf), 0, NULL, NULL);
            printf("%d bytesRead\n", bytesRead);
//            pthread_mutex_lock(&g_ring_mutex);
            int i;
            for (i = 0; i < bytesRead; i++) {
                if (!RingBuf_HasError(&g_RingBuf))
                    RingBuf_Put(&g_RingBuf, buf[i]);
            }
//    pthread_mutex_unlock(&g_ring_mutex);
        } while (bytesRead > 0);
        // don't allow immediate reconnection
        Platform_sleep_ms(1000);
    }
}

void UART_Init()
{
    xTaskCreate(&UdpRoutine, "uart_udp task", 256, NULL, 2, NULL);
    xTaskCreate(&UART_InterruptRoutine, "uart_socket task", 256, NULL, 2, NULL);
}

void UART_Send(uns8 ch)
{
    send(g_uartSocket, &ch, sizeof(ch), 0);
}
