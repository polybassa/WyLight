/**
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
#include "timer.h"

extern unsigned char do_update_fade;

bit g_led_off = 1; //X86 replacement for PORTC.0
pthread_mutex_t g_led_mutex = PTHREAD_MUTEX_INITIALIZER;
uns8 g_led_status[NUM_OF_LED*3];
extern uns8 g_UpdateLed;

void* InterruptRoutine(void* unused)
{
	int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == udp_sock)
		return 0;

	struct sockaddr_in udp_sock_addr;
  udp_sock_addr.sin_family = AF_INET;
  udp_sock_addr.sin_port = htons(WIFLY_SERVER_PORT);
  udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (-1 == bind(udp_sock, (struct sockaddr*)&udp_sock_addr, sizeof(udp_sock_addr)))
		return 0;

	for(;;)
	{
		uns8 buf[1024];
		int bytesRead = recvfrom(udp_sock, buf, sizeof(buf), 0, 0, 0);
		int i;
		for(i = 0; i < bytesRead; i++)
		{
			if(!RingBuf_HasError)
			{
				RingBuf_Put(buf[i]);
			}
		}
	}
}

void Crc_AddCrc(unsigned char byte,unsigned char* p_crcH,unsigned char* p_crcL) {}
void newCrc_BuildCrc(unsigned char* p_Crc_BuildCrcH, unsigned char* p_Crc_BuildCrcL) {}
void Crc_NewCrc(unsigned char* p_crcH, unsigned char* p_crcL)
{
    if(!p_crcH) return;
    if(!p_crcL) return;
    *p_crcH = 0xff;
    *p_crcL = 0xff;
}

void I2C_Init(){}

void* timer1_interrupt(void* unused)
{
	for(;;)
	{
		usleep(1000);
		Ledstrip_UpdateLed();
	}
}
void* timer4_interrupt(void* unused)
{
	for(;;)
	{
		usleep(1000);
		g_UpdateLed++;
	}
}

void* date_timer_interrupt(void* unused)
{
	for(;;)
	{
		usleep(2000000);
		date_timer_callback();
	}
}

void Rtc_Init() {}

void UART_Init() {}
void UART_Send(unsigned char ch)
{
	printf("%c", ch);
}
void SPI_Init() {}
char SPI_Send(char data)
{
	int i;
	for(i = 3*NUM_OF_LED - 1; i > 0; i--)
	{
		g_led_status[i] = g_led_status[i-1];
	}
	g_led_status[0] = data;
	return g_led_status[0];
}

void SPI_SendLedBuffer(uns8 *array)//!!! CHECK if GIE=0 during the sendroutine improves the result
{
	//array must be the address of the first byte
	uns8* end;
	//calculate where the end is
	end = array + (NUM_OF_LED * 3);
	//send all

	pthread_mutex_lock(&g_led_mutex);
	for(; array < end; array++)
	{
		SPI_Send(*array);
	}
	pthread_mutex_unlock(&g_led_mutex);
}

void init_x86(void)
{
	pthread_t isrThread;
	pthread_t glThread;
	pthread_t timer1Thread;
	pthread_t timer4Thread;
	pthread_t dateTimerThread;
	
	pthread_create(&isrThread, 0, InterruptRoutine, 0);
	pthread_create(&glThread, 0, gl_start, 0);
	pthread_create(&timer1Thread, 0, timer1_interrupt, 0);
	pthread_create(&timer4Thread, 0, timer4_interrupt, 0);
	pthread_create(&dateTimerThread, 0, date_timer_interrupt, 0);
}
