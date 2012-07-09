#ifndef _COMMANDSTORAGE_H_
#define _COMMANDSTORAGE_H_

#include "platform.h"
#include "RingBuf.h"		
#include "usart.h"			
#include "eeprom.h"   
#include "error.h"    	
#include "crc.h"
#include "timer.h"
#include "wifly_cmd.h"

#define CmdPointerAddr 0xff		// *** Address at EERPOM. Commandpointer indicates the nummer of commands
#define CmdLoopPointerAddr 0xfe // *** Address at EEPROM. CommandLoopPointer indicates the next command. Used in Loop-Mode

//*********************** STRUCT DECLARATION *********************************************
struct CommandBuffer{
    char cmd_counter;
    char frame_counter;
    uns8 cmd_buf[FRAMELENGTH];
    char crcH;
    char crcL;
	char LoopMode:1;
	uns16 WaitValue;
};
extern struct CommandBuffer gCmdBuf;

//*********************** METHODS AND MACROS *********************************************
#define CmdWidth sizeof(struct led_cmd)	// *** Number of Bytes for one command
#define ClearCmdBuf(x)  		\
{								\
	gCmdBuf.cmd_counter = 0;	\
	gCmdBuf.frame_counter = 0;	\
}

/**
 * pDest: output buffer
 * return: - 0, if no command available in buffer
 *         - pDest, if available command was written to pDest
**/
struct led_cmd* commandstorage_read(struct led_cmd *pDest);

/**
 * pSrc: command which should be written to eeprom
 * length: of the payload data in pSrc
 * return: - TRUE, if command was successfully written to buffer
 *         - FALSE, if not
**/
bit commandstorage_write(char *pSrc, char length);

/** This function reads one byte from the ringbuffer and check
*** for framestart, framelength, or databyte 
*** if a frame is complete, the function save the frame as a new
*** command in the internal EEPROM and calculate the Pointer for the next Command
**/
void commandstorage_get_commands();

void commandstorage_execute_commands();

/**
*** Initialize commandstorage in eeprom
**/
void commandstorage_init();

void commandstorage_wait_interrupt();

#endif /* #ifndef _COMMANDSTORAGE_H_ */

