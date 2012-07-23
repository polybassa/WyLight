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

#include "platform.h"
#include "commandstorage.h"
#include "ledstrip.h"

//*********************** PRIVATE FUNCTIONS *********************************************
uns16 GetEepromPointer(uns16 PointerAddr)
{
	uns16 temp;
	temp.low8 = EEPROM_RD(PointerAddr);
	temp.high8 = EEPROM_RD(PointerAddr+1);

	return temp;
}

void SetEepromPointer(uns16 PointerAddr, uns16 Value)
{
	EEPROM_WR(PointerAddr, Value.low8);
	EEPROM_WR(PointerAddr+1, Value.high8);
}
//*********************** PUBLIC FUNCTIONS *********************************************

struct CommandBuffer gCmdBuf;

struct led_cmd* commandstorage_read(struct led_cmd *pDest)
{
	// if the commandstorage is waiting
	if(gCmdBuf.WaitValue == 0) 
	{
		//check parameter
		if(0 == pDest) return 0;

		//commands available in eeprom?
		uns16 nextCmd = GetEepromPointer(CmdPointerAddr);
		if(0 == nextCmd) return 0;
	
		if(gCmdBuf.LoopMode)
		{
			nextCmd = GetEepromPointer(CmdLoopPointerAddr);
			if(0 == nextCmd)
				nextCmd = EEPROM_RD(CmdPointerAddr);
			SetEepromPointer(CmdLoopPointerAddr,nextCmd);
		}

		//read command from eeprom
		EEPROM_RD_BLK((unsigned char*)pDest,nextCmd - CmdWidth, CmdWidth);

		//update the CmdPointer?
		if(gCmdBuf.LoopMode)
			SetEepromPointer(CmdLoopPointerAddr,nextCmd - CmdWidth);		
		else
			SetEepromPointer(CmdPointerAddr,nextCmd - CmdWidth);
			
#ifdef TEST
		USARTsend_str("Read_Done");
#endif 
		return pDest;
	}
	else return 0;
}

bit commandstorage_write(unsigned char *pSrc, unsigned char length)
{
	//check parameter
	if(0 == pSrc) return FALSE;
	
	//enought free space in eeprom?
	uns16 nextCmd = GetEepromPointer(CmdPointerAddr);
	if(nextCmd >= (CmdPointerAddr - CmdWidth + 1)) 
		return FALSE;

	//increase the command pointer in eeprom
	SetEepromPointer(CmdPointerAddr,nextCmd + (uns16)CmdWidth);
		
	//write data to eeprom
	EEPROM_WR_BLK(pSrc, nextCmd, length);
		
	return TRUE;
}

void commandstorage_get_commands()
{	
	if(RingBufHasError)
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		ClearCmdBuf();
	}

	if(RingBufIsNotEmpty)
	{
		// *** preload variables and 
		// *** get new_byte from ringbuffer
		char new_byte, temp, j;
		temp = 0;
		j = 0;
		// *** get new byte
		new_byte = RingBufGet();
#ifdef TEST_COMMAND
		USARTsend_str("BYTE:");
		USARTsend_num(new_byte,'b');
#endif

#ifdef TEST_COMMAND
			USARTsend_str("FRAMECOUNTER");
			USARTsend_num(gCmdBuf.frame_counter,' ');
#endif
		// *** do I wait for databytes?
		if(gCmdBuf.frame_counter == 0)
		{
			// *** I don't wait for databytes
			// *** Do I receive a Start_of_Text sign
			if(new_byte == STX)
			{
#ifdef TEST_COMMAND
				USARTsend_str("STX_detected  ");
#endif
				// *** increse the cmd_counter
				gCmdBuf.cmd_counter = 1;
				// *** Write the startsign at the begin of the buffer
				gCmdBuf.cmd_buf[0] = new_byte;
                // *** Reset crc Variables
                newCRC(&gCmdBuf.crcH, &gCmdBuf.crcL);
                // *** add new_byte to crc checksum
                addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
			}
			else
			{	
				// *** to avoid arrayoverflow
				temp = FRAMELENGTH - 2;
				// *** check if I get the framelength byte
				if((new_byte < temp) && (gCmdBuf.cmd_counter == 1))
				{
#ifdef TEST_COMMAND
					USARTsend_str("FRAME_LEN_detected  ");
#endif
					gCmdBuf.frame_counter = new_byte;
					gCmdBuf.cmd_buf[1] = new_byte;
					gCmdBuf.cmd_counter = 2;
                    // *** add new_byte to crc checksum
                    addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
				}
			}
		}
		else
		{
			// *** I wait for Databytes, so I save all bytes 
			// *** that I get until my framecounter is > 0
			gCmdBuf.cmd_buf[gCmdBuf.cmd_counter] = new_byte;
			gCmdBuf.cmd_counter++;
			
            // *** add new_byte to crc checksum
			if(gCmdBuf.frame_counter > 2)
            addCRC(new_byte, &gCmdBuf.crcH, &gCmdBuf.crcL);
			gCmdBuf.frame_counter--;
			// *** now I have to check if my framecounter is null.
			// *** If it's null my string is complete 
			// *** and I can give the string to the crc check function.
			if(gCmdBuf.frame_counter == 0)
			{
#ifndef NO_CRC
                // *** verify crc checksum
                if( (gCmdBuf.crcL == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 1]) &&
                    (gCmdBuf.crcH == gCmdBuf.cmd_buf[gCmdBuf.cmd_counter - 2]) )
#endif
                {
					// *** Execute the simple Commands
					switch(gCmdBuf.cmd_buf[2])
					{
						case DELETE: 
							{
								SetEepromPointer(CmdPointerAddr,0);
								SetEepromPointer(CmdLoopPointerAddr,0);
								USARTsend('D');
								return;
							}
						case SET_ON: 
							{
								//Not Implement on PIC18 Board
								return;
							}
						case SET_OFF: 
							{
								//Not Implement on PIC18 Board
								return;
							}
						case LOOP_ON:
							{	
								gCmdBuf.LoopMode = 1;
								USARTsend('L');
								USARTsend('1');
								return;
							}
						case LOOP_OFF:
							{	
								gCmdBuf.LoopMode = 0;
								gCmdBuf.WaitValue = 0;
								USARTsend('L');
								USARTsend('0');
								return;
							}
						default:
							{
								if( commandstorage_write(&gCmdBuf.cmd_buf[2], (gCmdBuf.cmd_counter - 4)))
								{
									USARTsend('G');
									USARTsend('C');
								}
								else 
									gERROR.eeprom_failure = 1;
							}
					}							
					
                }
#ifndef NO_CRC
                else
                {
                    // *** Do some error handling in case of an CRC failure here
					gERROR.crc_failure = 1;
                    return;
                }
#endif
			}
		}
	}
}

void commandstorage_execute_commands()
{
	// *** get the pointer to commands in the EEPROM
	struct led_cmd nextCmd;

	// read next command from eeprom and move command pointer in eeprom to the next command (TRUE)
	struct led_cmd *result = commandstorage_read(&nextCmd);
		
	if(0 != result)
	{
#ifdef TEST
USARTsend_str("executeCommand");
#endif
		// *** commands available, check what to do
		switch(nextCmd.cmd) 
		{	
			case SET_COLOR: 
			{
#ifdef TEST
				USARTsend_str("SET_COLOR");
				USARTsend_num(nextCmd.data.set_color.addr[0],'A');
				USARTsend_num(nextCmd.data.set_color.addr[1],'A');
				USARTsend_num(nextCmd.data.set_color.addr[2],'A');
				USARTsend_num(nextCmd.data.set_color.addr[3],'A');
				USARTsend_num(nextCmd.data.set_color.red,'R');
				USARTsend_num(nextCmd.data.set_color.green,'G');
				USARTsend_num(nextCmd.data.set_color.blue,'B');
#endif
				ledstrip_set_color(&nextCmd.data.set_color);
				break;
			}
			case SET_FADE:
			{
				ledstrip_set_fade(&nextCmd.data.set_fade);
				break;
			}
			case WAIT:
			{
				struct cmd_wait *pCmd = &nextCmd.data.wait;
#ifdef TEST
				USARTsend_num(pCmd->valueH,'H');
				USARTsend_num(pCmd->valueL,'L');
#endif
				
				gCmdBuf.WaitValue.high8 = pCmd->valueH;
				gCmdBuf.WaitValue.low8 = pCmd->valueL;
				break;
			}
			case SET_RUN: {break;}
		}
	}
}

void commandstorage_wait_interrupt()
{
	if(gCmdBuf.WaitValue != 0) 
		gCmdBuf.WaitValue = --gCmdBuf.WaitValue;					
}	

void commandstorage_init()
{
	/** EEPROM contains FF in every cell after inital start,
	*** so I have to delete the pointer address
	*** otherwise the PIC thinks he has the EEPROM full with commands
	**/
#ifndef TEST_COMMAND
	if (GetEepromPointer(CmdPointerAddr) == 0xff)
#endif
		SetEepromPointer(CmdPointerAddr, 0);
#ifdef TEST_COMMAND
	uns16 cmdptr = GetEepromPointer(CmdPointerAddr);
	USARTsend_num(cmdptr.high8, 'H');
	USARTsend_num(cmdptr.low8, 'L');
#endif
	gCmdBuf.LoopMode = 0;
	gCmdBuf.WaitValue = 0;

	// set loop pointer address to start
	SetEepromPointer(CmdLoopPointerAddr, 0);
}

