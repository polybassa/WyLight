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

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;
static const uint16_t WIFLY_SERVER_PORT = 2000;
static const uint16_t BROADCAST_PORT = 55555;
static int g_uartSocket = -1;

static const uint8_t capturedBroadcastMessage[110] = {
    0x00, 0x0f, 0xb5, 0xb2, 0x57, 0xfa, //MAC
    0x07, //channel
    0x3f, //rssi
    0x07, 0xd0, //port
    0x00, 0x00, 0x24, 0xb1, //rtc
    0x0b, 0xff, //battery
    0x0d, 0x11, //gpio
    0x54, 0x69, 0x6d, 0x65, 0x20, 0x4e, 0x4f, 0x54, 0x20, 0x53, 0x45, 0x54, 0x00, 0x00, //time
    0x57, 0x69, 0x46, 0x6c, 0x79, 0x20, 0x56, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x33, 0x36,
    0x2c, 0x20, 0x30, 0x38, 0x2d, 0x32, 0x32, 0x2d, 0x32, 0x30, 0x31, 0x32, 0x00, 0x00, //version
    'W', 'i', 'F', 'l', 'y', '-', 'E', 'Z', 'X',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', //deviceid
    0x4e, 0x00, //boottime
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
};

static Platform_ThreadFunc UART_TcpRecv(void* param)
{
    Platform_Mutex* mutex = (Platform_Mutex*)(param);
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

            Platform_MutexLock(mutex);
            for (int i = 0; i < bytesRead; i++) {
                if (!RingBuf_HasError(&g_RingBuf))
                    RingBuf_Put(&g_RingBuf, buf[i]);
            }
            Platform_MutexUnlock(mutex);
        } while (bytesRead > 0);

        // don't allow immediate reconnection
        Platform_sleep_ms(1000);
    }
}

static Platform_ThreadFunc UART_UdpRecv(void* param)
{
    Platform_Mutex* mutex = (Platform_Mutex*)(param);
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

            Platform_MutexLock(mutex);
            for (int i = 0; i < bytesRead; i++) {
                if (!RingBuf_HasError(&g_RingBuf))
                    RingBuf_Put(&g_RingBuf, buf[i]);
            }
            Platform_MutexUnlock(mutex);
        } while (bytesRead > 0);
        // don't allow immediate reconnection
        Platform_sleep_ms(1000);
    }
}

Platform_ThreadFunc UART_Broadcast(void* unused)
{
    int udpSocket;
    do {
        udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    } while (-1 == udpSocket);

    struct sockaddr_in broadcastAddress;
    broadcastAddress.sin_family = AF_INET;
    broadcastAddress.sin_port = htons(BROADCAST_PORT);
    broadcastAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    int val = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));

    for ( ; ; ) {
        sendto(udpSocket,
               capturedBroadcastMessage,
               sizeof(capturedBroadcastMessage),
               0,
               (struct sockaddr*)&broadcastAddress,
               sizeof(broadcastAddress));
        Platform_sleep_ms(1000);
    }
}

void UART_Init()
{
    static Platform_Mutex mutex;
    static Platform_Thread tcp;
    static Platform_Thread udp;
    static Platform_Thread broadcast;

    mutex = Platform_MutexInit();
    Platform_CreateThread(&UART_TcpRecv, 256, &mutex, 2, &tcp);
    Platform_CreateThread(&UART_UdpRecv, 256, &mutex, 2, &udp);
    Platform_CreateThread(&UART_Broadcast, 256, NULL, 2, &broadcast);
}

void UART_Send(uns8 ch)
{
    Trace(ZONE_VERBOSE, "0x%02x(%c)\n", ch, ch);
    send(g_uartSocket, &ch, sizeof(ch), 0);
}
