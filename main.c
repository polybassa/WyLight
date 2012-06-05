//Nils Weiﬂ 
//05.09.2011
//Compiler CC5x/

#define NO_CRC
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
struct LedBuffer gLedBuf;
struct ErrorBits gERROR;
char gTimecounter;

#ifdef X86
void* gl_start(void* unused);
void* InterruptRoutine(void* unused)
{

	char dummyData[] = {
		STX, (uns8)sizeof(struct cmd_set_color) + 2,
		SET_COLOR,
		0xff, 0xff, 0xff, 0xff,
		0, 128, 0,
		0, 0,
		0xDE, 0xAD};

	uns8 i = 0;
	for(;;)
	{
		if(!RingBufHasError)
		{
			RingBufPut(dummyData[i]);
		}
	
		i++;
		if(i == sizeof(dummyData))
		{
			i = 0;
			char temp = dummyData[7];
			dummyData[7] = dummyData[8];
			dummyData[8] = dummyData[9];
			dummyData[9] = temp;
			sleep(1);//sleep a second
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
		Timerinterrupt();
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
	
	pthread_create(&isrThread, 0, InterruptRoutine, 0);
	pthread_create(&glThread, 0, gl_start, 0);
#endif /* #ifdef X86 */
    
	while(1)
	{
		throw_errors();
		commandstorage_get_commands();
		commandstorage_execute_commands();
		if(gTimecounter == 0)
		{
			if(gLedBuf.led_fade_operation)
				ledstrip_do_fade();
		}	
	}
}
//*********************** UNTERPROGRAMME **********************************************

void init_all()
{
	OsciInit();
	RingBufInit();
	USARTinit();
	spi_init();
	ledstrip_init();
	commandstorage_init();
	timer_init();
	InitFET();
	PowerOnLEDs();
    
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
#include "crc.c"
#include "eeprom.c"
#include "error.c"
#include "ledstrip.c"
#include "RingBuf.c"
#include "spi.c"
#include "timer.c"
#include "usart.c"
#include "commandstorage.c"
#endif /* #ifndef X86 */

