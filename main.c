/**
 Copyright (C) 2012 Nils Weiss, Patrick Brünn.
 
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


#ifndef X86
#define NO_CRC
//#define TEST
#pragma optimize 0
//#pragma resetVector 0x400
//#pragma unlockISR
#endif
#pragma sharedAllocation

//*********************** INCLUDEDATEIEN *********************************************
#include "platform.h"
#include "RingBuf.h"		//clean
#include "usart.h"			//clean
#include "eeprom.h"       	//clean 
#include "crc.h"			//clean
#include "commandstorage.h" //under construction
#include "ledstrip.h"		//clean
#include "spi.h"			//clean
#include "timer.h"			//under construction

//*********************** GLOBAL VARIABLES *******************************************
struct CommandBuffer gCmdBuf;
bank1 struct LedBuffer gLedBuf;
struct ErrorBits gERROR;
char gTimecounter;
unsigned short g_timer_signaled;	
//*********************** X86 InterruptRoutine *******************************************

#ifdef X86
bit g_led_off; //X86 replacement for PORTC.0

void* gl_start(void* unused);

#include <sys/socket.h>
#include <netinet/in.h>
void* InterruptRoutine(void* unused)
{
	#define PORT 12345
	int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(-1 == udp_sock)
		return;

	struct sockaddr_in udp_sock_addr;
  udp_sock_addr.sin_family = AF_INET;
  udp_sock_addr.sin_port = htons(PORT);
  udp_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (-1 == bind(udp_sock, (struct sockaddr*)&udp_sock_addr, sizeof(udp_sock_addr)))
		return;

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
#else
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 4					//Adresse des Interrupts	
interrupt InterruptRoutine(void)
{
	if(RCIF)
	{
		if(!RingBufHasError) RingBufPut(RCREG);
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			char temp = RCREG;
		}
	}
	if(TMR2IF)
	{
		Timer2interrupt();
	}
	if(TMR4IF)
	{
		Timer4interrupt();
		commandstorage_wait_interrupt();
	}
}
#endif /* #ifdef X86 */

//*********************** FUNKTIONSPROTOTYPEN ****************************************
void init_all();

//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
	init_all();

#ifdef X86
	#include <pthread.h>
	pthread_t isrThread;
	pthread_t glThread;
	pthread_t timerThread;
	
	pthread_create(&isrThread, 0, InterruptRoutine, 0);
	pthread_create(&glThread, 0, gl_start, 0);
	pthread_create(&timerThread, 0, timer_interrupt, 0);
#endif /* #ifdef X86 */
    
	while(1)
	{
#ifdef X86
		usleep(1000);
#endif
		Check_INPUT();
		throw_errors();
		commandstorage_get_commands();
		commandstorage_execute_commands();

		if(g_timer_signaled > 0)
		{
			ledstripe_update_fade();
			ledstrip_do_fade();
		}
	}
}
//*********************** UNTERPROGRAMME **********************************************

void init_all()
{
	OsciInit();
	InitInputs();
	RingBufInit();
	USARTinit();
	spi_init();
	timer_init();
	ledstrip_init();
	commandstorage_init();
	
	InitFET();
	PowerOnLEDs();
    InitFactoryRestoreWLAN();
	ErrorInit();
	ClearCmdBuf();	
	AllowInterrupts();
	
	// *** send ready after init
	USARTsend('R');
	USARTsend('D');
	USARTsend('Y');
}

// cc5xfree is a bit stupid so we include the other implementation files here
#ifndef X86
//#pragma codepage 1
#include "crc.c"
#include "eeprom.c"
#include "error.c"
#include "ledstrip.c"
#include "RingBuf.c"
#include "spi.c"
#include "timer.c"
#include "usart.c"
#include "commandstorage.c"
#include "platform.c"
#endif /* #ifndef X86 */



