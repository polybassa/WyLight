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

#ifndef X86
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
#endif /* #ifndef X86 */

//*********************** FUNKTIONSPROTOTYPEN ****************************************
void init_all();

//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
	init_all();
	//FactoryRestoreWLAN();
	//Rn171FactoryRestore();
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

