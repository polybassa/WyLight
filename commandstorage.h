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

#define CMD_POINTER_ADDRESS 0x3fd		// *** Address at EERPOM. Commandpointer indicates the nummer of commands
#define LOOP_POINTER_ADDRESS 0x3fb // *** Address at EEPROM. CommandLoopPointer indicates the next command. Used in Loop-Mode

//*********************** STRUCT DECLARATION *********************************************
struct CommandBuffer{
	uns16 WaitValue;
    uns8 cmd_counter;
    uns8 frame_counter;
    uns8 cmd_buf[FRAMELENGTH];
    uns8 CrcH;
    uns8 CrcL;
	uns8 LoopMode:1;
};
extern struct CommandBuffer g_CmdBuf;

//*********************** METHODS AND MACROS *********************************************
#define CmdWidth sizeof(struct led_cmd)	// *** Number of Bytes for one command
#define Commandstorage_Clear(x)  		\
{								\
	g_CmdBuf.cmd_counter = 0;	\
	g_CmdBuf.frame_counter = 0;	\
}

/**
 * pDest: output buffer
 * return: - 0, if no command available in buffer
 *         - pDest, if available command was written to pDest
**/
struct led_cmd* Commandstorage_Read(struct led_cmd *pDest);

/**
 * pSrc: command which should be written to eeprom
 * length: of the payload data in pSrc
 * return: - TRUE, if command was successfully written to buffer
 *         - FALSE, if not
**/
bit Commandstorage_Write(unsigned char *pSrc, unsigned char length);

/** This function reads one byte from the ringbuffer and check
*** for framestart, framelength, or databyte 
*** if a frame is complete, the function save the frame as a new
*** command in the internal EEPROM and calculate the Pointer for the next Command
**/
void Commandstorage_GetCommands();

void Commandstorage_ExecuteCommands();

/**
*** Initialize commandstorage in eeprom
**/
void Commandstorage_Init();

#define Commandstorage_WaitInterrupt(x) \
		do { \
			if(g_CmdBuf.WaitValue != 0) \
				g_CmdBuf.WaitValue = --g_CmdBuf.WaitValue; \
		} while(0)

#endif /* #ifndef _COMMANDSTORAGE_H_ */

