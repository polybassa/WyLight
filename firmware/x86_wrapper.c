/*
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "x86_wrapper.h"
#include "RingBuf.h"
#include "ScriptCtrl.h"
#include "timer.h"
#include "Version.h"

extern unsigned char do_update_fade;

//const char verStr[] = VER_STRING;

bit g_led_off = 1; //X86 replacement for PORTC.0
pthread_mutex_t g_led_mutex = PTHREAD_MUTEX_INITIALIZER;
uns8 g_led_status[NUM_OF_LED * 3];
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

void *BroadcastLoop(void *unused)
{
	int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == udpSocket)
		return NULL;

	struct sockaddr_in broadcastAddress;
	broadcastAddress.sin_family = AF_INET;
	broadcastAddress.sin_port = htons(BROADCAST_PORT);
	broadcastAddress.sin_addr.s_addr = htonl(INADDR_NONE);
	int val = 1;
	setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));

	for(;; ) {
		sleep(1);
		sendto(udpSocket, capturedBroadcastMessage, sizeof(capturedBroadcastMessage), 0, (struct sockaddr *)&broadcastAddress, sizeof(broadcastAddress));
	}
	return NULL;
}

void *InterruptRoutine(void *unused)
{
	int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == listenSocket) {
			printf("%s:%d %s: create socket failed\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	struct sockaddr_in udp_sock_addr;
	udp_sock_addr.sin_family = AF_INET;
	udp_sock_addr.sin_port = htons(WIFLY_SERVER_PORT);
	udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(0 != bind(listenSocket, (struct sockaddr *)&udp_sock_addr, sizeof(udp_sock_addr))) {
			printf("%s:%d %s: bind() failed\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	if(0 != listen(listenSocket, 0)) {
			printf("%s:%d %s: listen() failed\n", __FILE__, __LINE__, __FUNCTION__);
		return 0;
	}

	g_uartSocket = accept(listenSocket, NULL, NULL);
	for(;; ) {
		int bytesRead;
		do
		{
			uns8 buf[1024];
			bytesRead = recv(g_uartSocket, buf, sizeof(buf), 0);
			printf("%d bytesRead\n", bytesRead);
			int i;
			for(i = 0; i < bytesRead; i++) {
				if(!RingBuf_HasError(&g_RingBuf)) {
					RingBuf_Put(&g_RingBuf, buf[i]);
				}
			}
		}
		while(bytesRead > 0);
		// don't allow immediate reconnection
		sleep(1);
		g_uartSocket = accept(listenSocket, NULL, NULL);
	}
}

void I2C_Init(){}

void *timer1_interrupt(void *unused)
{
	for(;; ) {
		usleep(1000);
		Ledstrip_UpdateLed();
	}
}
void *timer4_interrupt(void *unused)
{
	for(;; ) {
		usleep(1000);
		g_UpdateLed++;
		if(gScriptBuf.waitValue > 0) {
			gScriptBuf.waitValue--;
		}
		;
	}
}

void Rtc_Init() {}
void Rtc_Ctl(enum RTC_request req,struct rtc_time *pRtcTime) {}

void UART_Init() {}
void UART_Send(uns8 ch)
{
	printf("0x%02x(%c)\n", ch, ch);
	send(g_uartSocket, &ch, sizeof(ch), 0);
}
void SPI_Init() {}
char SPI_Send(uns8 data)
{
	int i;
	for(i = 3 * NUM_OF_LED - 1; i > 0; i--) {
		g_led_status[i] = g_led_status[i - 1];
	}
	g_led_status[0] = data;
	return g_led_status[0];
}

void SPI_SendLedBuffer(uns8 *array) //!!! CHECK if GIE=0 during the sendroutine improves the result
{
	//array must be the address of the first byte
	uns8 *end;
	//calculate where the end is
	end = array + (NUM_OF_LED * 3);
	//send all

	pthread_mutex_lock(&g_led_mutex);
	for(; array < end; array++) {
		SPI_Send(*array);
	}
	pthread_mutex_unlock(&g_led_mutex);
}

void init_x86(void)
{
	pthread_t broadcastThread;
	pthread_t isrThread;
	pthread_t glThread;
	pthread_t timer1Thread;
	pthread_t timer4Thread;

	pthread_create(&broadcastThread, 0, BroadcastLoop,    0);
	pthread_create(&isrThread,       0, InterruptRoutine, 0);
	pthread_create(&glThread,        0, gl_start,         0);
	pthread_create(&timer1Thread,    0, timer1_interrupt, 0);
	pthread_create(&timer4Thread,    0, timer4_interrupt, 0);
}
