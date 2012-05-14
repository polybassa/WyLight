//Nils Weiß 
//05.09.2011
//Compiler CC5x/

#define NO_CRC
#define MPLAB_IDE

#include "platform.h"
#pragma sharedAllocation

//*********************** ENUMERATIONS *********************************************
#define TRUE  1
#define FALSE 0

//*********************** INCLUDEDATEIEN *********************************************
#include "RingBuf.h"		//clean
#include "usart.h"			//clean
#include "eeprom.h"       	//clean 
#include "crc.h"			//clean
#include "commandstorage.h"
#include "ledstrip.h"		//clean
#include "spi.h"			//clean
#include "timer.h"

//*********************** GLOBAL VARIABLES *******************************************
static struct CommandBuffer gCmdBuf;
static struct LedBuffer gLedBuf;
// *** ERRORBITS
static struct {
		char crc_failure:1;
		char eeprom_failure:1;
}gERROR;

#ifndef X86
//*********************** INTERRUPTSERVICEROUTINE ************************************
#pragma origin 4					//Adresse des Interrupts	
interrupt InterruptRoutine(void)
{
	if (RCIF)
	{
		if(!RingBufHasError) RingBufPut(RCREG);
		else 
		{
			//Register lesen um Schnittstellen Fehler zu vermeiden
			char temp = RCREG;
		}
	}
}
#endif /* #ifndef X86 */

//*********************** FUNKTIONSPROTOTYPEN ****************************************
void init_all();
void throw_errors();
void execute_commands();

//*********************** HAUPTPROGRAMM **********************************************
void main(void)
{
	init_all();
    while(1)
    {	
        throw_errors();
		commandstorage_get_commands();
		commandstorage_execute_commands();
    }
}
//*********************** UNTERPROGRAMME **********************************************

void init_all()
{
#ifndef X86
	//OSZILLATOR initialisieren: 4xPLL deactivated;INTOSC 16MHz
	OSCCON = 0b01110010;
#endif /* #ifndef X86 */
	RingBufInit();
	//initialise UART interface
	USARTinit();
	spi_init();
	ledstrip_init();

/** EEPROM contains FF in every cell after inital start,
*** so I have to delet the pointer address
*** otherwise the PIC thinks he has the EEPROM full with commands
**/
	if (EEPROM_RD(CmdPointerAddr) == 0xff)
	EEPROM_WR(CmdPointerAddr, 0);
	EEPROM_WR(CmdLoopPointerAddr, 0);
	
#ifndef X86
	//Ausgang für FET initalisieren
	TRISC.0 = 0;
	//Spannungsversorgung für LED's einschalten
	PORTC.0 = 0;

	//To Factory Restore WLAN Modul
	//TRISA.0 = 0;
	//PORTA.0 = 1;
#endif /* #ifndef X86 */
    
    // *** load globals variables
    gERROR = 0;
	ClearCmdBuf;
	
#ifndef X86
	// *** allow interrupts
	RCIE=1;
	PEIE=1;
	GIE=1;
#endif /* #ifndef X86 */
	// *** send ready after init
	USARTsend('R');
	USARTsend('D');
	USARTsend('Y');
}

void throw_errors()
{
	if(RingBufHasError) 
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		ClearCmdBuf;
		USARTsend_str(" ERROR: Receivebuffer full");
		// *** Re-init the Ringbuffer to get a consistent commandstring and reset error
		RingBufInit();
	}
	if(gERROR.crc_failure)
	{
		USARTsend_str(" ERROR: CRC-Check failed");
		gERROR.crc_failure = 0;
	}
	if(gERROR.eeprom_failure)
	{
		USARTsend_str(" ERROR: EEPROM is full");
		gERROR.eeprom_failure = 0;
	}
}

// cc5xfree is a bit stupid so we include the other implementation files here
#ifndef X86
#include "crc.c"
#include "eeprom.c"
#include "ledstrip.c"
#include "RingBuf.c"
#include "spi.c"
#include "timer.c"
#include "usart.c"
#include "commandstorage.c"
#endif /* #ifndef X86 */
