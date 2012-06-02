#ifndef _COMMANDSTORAGE_H_
#define _COMMANDSTORAGE_H_

#include "RingBuf.h"		
#include "usart.h"			
#include "eeprom.h"   
#include "error.h"    	
#include "crc.h"			

#define CmdPointerAddr 0xff		// *** Address at EERPOM. Commandpointer indicates the nummer of commands
#define CmdLoopPointerAddr 0xfd // *** Address at EEPROM. CommandLoopPointer indicates the next command. Used in Loop-Mode

//*********************** ENUMERATIONS *********************************************
#define STX 0xFF
#define SET_COLOR 0xFD
#define SET_FADE 0xFC
#define SET_RUN 0xFB
#define WAIT 0xFE
#define SET_ON 0xFA
#define SET_OFF 0xF9
#define DELETE 0xF8

//*********************** STRUCT DECLARATION *********************************************
struct cmd_set_color {
	char addr[4];
	char red;
	char green;
	char blue;
	char reserved0;
	char reserved1;
};

struct cmd_set_fade {
	char addr[4];
	char red;
	char green;
	char blue;
	char timevalue;
	char reserved0;
};

struct cmd_set_run {
	char dummy;
};

struct led_cmd {
	unsigned cmd;
	union {
		struct cmd_set_color set_color;
		struct cmd_set_fade set_fade;
		struct cmd_set_run set_run;
	}data;
};

#define FRAMELENGTH (sizeof(struct led_cmd) + 5)			// *** max length of one commandframe
struct CommandBuffer{
    char cmd_counter;
    char frame_counter;
    char cmd_buf[FRAMELENGTH];
    char crcH;
    char crcL;
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
 * movePtr: if set, the command pointer will be moved to the next command, after command was read
 * return: - 0, if no command available in buffer
 *         - pDest, if available command was written to pDest
**/
struct led_cmd* commandstorage_read(struct led_cmd *pDest, bit movePtr);

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

#endif /* #ifndef _COMMANDSTORAGE_H_ */

