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

bit g_led_off = 1; //X86 replacement for PORTC.0
pthread_mutex_t g_led_mutex = PTHREAD_MUTEX_INITIALIZER;
uns8 g_led_status[NUM_OF_LED*3];

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
			if(!RingBufHasError)
			{
				RingBufPut(buf[i]);
			}
		}
	}
}

void addCRC(char byte, unsigned char* p_crcH, unsigned char* p_crcL) {}
void newCRC(unsigned char* p_crcH, unsigned char* p_crcL) {}

void IICinit(){}
void TimerInit(){}
void timer_set_for_fade(char value){}
void* timer_interrupt(void* unused)
{
	for(;;)
	{
		usleep(1000 * CYCLE_TMMS);
		ledstrip_update_fade();
	}
}

void USARTinit() {}
void USARTsend(char ch)
{
	FILE* gUSART = fopen("out_usart.txt", "a+");
	fputc(ch, gUSART);
	fclose(gUSART);
}


static uns8 gEEPROM[0x100];
char EEPROM_RD(uns16 adress)
{
	return gEEPROM[adress];
}


void EEPROM_WR(uns16 adress, char data)
{
	gEEPROM[adress] = data;
}
void spi_init() {}
char spi_send(char data)
{
	int i;
	for(i = 3*NUM_OF_LED - 1; i > 0; i--)
	{
		g_led_status[i] = g_led_status[i-1];
	}
	g_led_status[0] = data;
	return g_led_status[0];
}

void spi_send_ledbuf(uns8 *array)//!!! CHECK if GIE=0 during the sendroutine improves the result
{
	//array must be the address of the first byte
	uns8* end;
	//calculate where the end is
	end = array + (NUM_OF_LED * 3);
	//send all

	pthread_mutex_lock(&g_led_mutex);
	for(; array < end; array++)
	{
		spi_send(*array);
	}
	pthread_mutex_unlock(&g_led_mutex);
}

void init_x86(void)
{
	pthread_t isrThread;
	pthread_t glThread;
	pthread_t timerThread;
	
	pthread_create(&isrThread, 0, InterruptRoutine, 0);
	pthread_create(&glThread, 0, gl_start, 0);
	pthread_create(&timerThread, 0, timer_interrupt, 0);
}
