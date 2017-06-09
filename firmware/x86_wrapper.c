/*
   Copyright (C) 2012 - 2017 Nils Weiss, Patrick Bruenn.

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

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "platform.h"
#include "ledstrip.h"
#include "RingBuf.h"
#include "ScriptCtrl.h"
#include "timer.h"
#include "Version.h"

extern uns8 g_UpdateLed;

struct RingBuffer g_TraceBuf;
extern struct ScriptBuf gScriptBuf;

int g_uartSocket = -1;
const unsigned short BROADCAST_PORT = 55555;
const unsigned short WIFLY_SERVER_PORT = 2000;
unsigned char capturedBroadcastMessage[110] = {
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

void* BroadcastLoop(void* unused)
{
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == udpSocket)
        return NULL;
    struct sockaddr_in broadcastAddress;
    broadcastAddress.sin_family = AF_INET;
    broadcastAddress.sin_port = htons(BROADCAST_PORT);
    broadcastAddress.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    int val = 1;
    setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));

    for ( ; ; ) {
        sleep(1);
        sendto(udpSocket,
               capturedBroadcastMessage,
               sizeof(capturedBroadcastMessage),
               0,
               (struct sockaddr*)&broadcastAddress,
               sizeof(broadcastAddress));
    }
    return NULL;
}

void I2C_Init(){}
void I2C_Write(const uns8 slaveaddr, const uns8 dataaddr, const uns8 data){}
uns8 I2C_Read(const uns8 slaveaddr, const uns8 readaddr){return 0; }
uns8 I2C_DetectSlave(const uns8 slaveaddr){return 0; }

void Rtc_Init() {}
void Rtc_Ctl(enum RTC_request req, struct rtc_time* pRtcTime) {}

uns16 Timer_PrintCycletime(uns16* pArray, const uns16 arraySize)
{
    return arraySize;
}

void Platform_ExtraInit()
{
    pthread_t broadcastThread;

    pthread_create(&broadcastThread, 0, BroadcastLoop, 0);
}
