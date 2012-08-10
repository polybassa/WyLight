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
#include "trace.h"

//*********************** PRIVATE FUNCTIONS *********************************************
uns16 GetEepromPointer(uns16 PointerAddr)
{
	uns16 temp;
#ifndef X86
	temp.low8 = Eeprom_Read(PointerAddr);
	temp.high8 = Eeprom_Read(PointerAddr+1);
#else
	temp = 0x00ff & Eeprom_Read(PointerAddr);
	temp |= Eeprom_Read(PointerAddr+1) << 8;
#endif

	return temp;
}

void SetEepromPointer(uns16 PointerAddr, uns16 Value)
{
#ifndef X86
	Eeprom_Write(PointerAddr, Value.low8);
	Eeprom_Write(PointerAddr+1, Value.high8);
#else
	Eeprom_Write(PointerAddr, Value & 0x00ff);
	Eeprom_Write(PointerAddr+1, (Value & 0xff00) >> 8);
#endif
}
//*********************** PUBLIC FUNCTIONS *********************************************

struct CommandBuffer g_CmdBuf;

struct led_cmd* Commandstorage_Read(struct led_cmd *pDest)
{
	// if the commandstorage is waiting
	if(g_CmdBuf.WaitValue == 0) 
	{
		//check parameter
		if(0 == pDest) return 0;

		//commands available in eeprom?
		uns16 nextCmd = GetEepromPointer(CMD_POINTER_ADDRESS);
		if(0 == nextCmd) return 0;
	
		if(g_CmdBuf.LoopMode)
		{
			nextCmd = GetEepromPointer(LOOP_POINTER_ADDRESS);
			if(0 == nextCmd)
				nextCmd = Eeprom_Read(CMD_POINTER_ADDRESS);
			SetEepromPointer(LOOP_POINTER_ADDRESS,nextCmd);
		}

		//read command from eeprom
		Eeprom_ReadBlock((unsigned char*)pDest,nextCmd - CmdWidth, CmdWidth);

		//update the CmdPointer?
		if(g_CmdBuf.LoopMode)
			SetEepromPointer(LOOP_POINTER_ADDRESS,nextCmd - CmdWidth);		
		else
			SetEepromPointer(CMD_POINTER_ADDRESS,nextCmd - CmdWidth);
#ifdef TEST			
		UART_SendString("Read_Done");
#endif
		return pDest;
	}
	else return 0;
}

bit Commandstorage_Write(unsigned char *pSrc, unsigned char length)
{
	//check parameter
	if(0 == pSrc) return FALSE;
	
	//enought free space in eeprom?
	uns16 nextCmd = GetEepromPointer(CMD_POINTER_ADDRESS);
	if(nextCmd >= (CMD_POINTER_ADDRESS - CmdWidth + 1)) 
		return FALSE;

	//increase the command pointer in eeprom
	SetEepromPointer(CMD_POINTER_ADDRESS,nextCmd + (uns16)CmdWidth);
		
	//write data to eeprom
	Eeprom_WriteBlock(pSrc, nextCmd, length);
		
	return TRUE;
}

void Commandstorage_GetCommands()
{	
	if(RingBuf_HasError)
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		Commandstorage_Clear();
	}

	if(RingBufIsNotEmpty)
	{
		// *** preload variables and 
		// *** get new_byte from ringbuffer
		unsigned char new_byte, temp;
		temp = 0;
		// *** get new byte
		new_byte = RingBuf_Get();
		Trace_String("BYTE:");
		Trace_Number(new_byte,'b');

#ifdef TEST_COMMAND
			UART_SendString("FRAMECOUNTER");
			UART_SendNumber(g_CmdBuf.frame_counter,' ');
#endif
		// *** do I wait for databytes?
		if(g_CmdBuf.frame_counter == 0)
		{
			// *** I don't wait for databytes
			// *** Do I receive a Start_of_Text sign
			if(new_byte == STX)
			{
#ifdef TEST_COMMAND
				UART_SendString("STX_detected  ");
#endif
				// *** increse the cmd_counter
				g_CmdBuf.cmd_counter = 1;
				// *** Write the startsign at the begin of the buffer
				g_CmdBuf.cmd_buf[0] = new_byte;
                // *** Reset crc Variables
                Crc_NewCrc(&g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
                // *** add new_byte to Crc_BuildCrc checksum
                Crc_AddCrc(new_byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
			}
			else
			{	
				// *** to avoid arrayoverflow
				temp = FRAMELENGTH - 2;
				// *** check if I get the framelength byte
				if((new_byte < temp) && (g_CmdBuf.cmd_counter == 1))
				{
#ifdef TEST_COMMAND
					UART_SendString("FRAME_LEN_detected  ");
#endif
					g_CmdBuf.frame_counter = new_byte;
					g_CmdBuf.cmd_buf[1] = new_byte;
					g_CmdBuf.cmd_counter = 2;
                    // *** add new_byte to Crc_BuildCrc checksum
                    Crc_AddCrc(new_byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
				}
			}
		}
		else
		{
			// *** I wait for Databytes, so I save all bytes 
			// *** that I get until my framecounter is > 0
			g_CmdBuf.cmd_buf[g_CmdBuf.cmd_counter] = new_byte;
			g_CmdBuf.cmd_counter++;
			
            // *** add new_byte to Crc_BuildCrc checksum
			if(g_CmdBuf.frame_counter > 2)
            Crc_AddCrc(new_byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
			g_CmdBuf.frame_counter--;
			// *** now I have to check if my framecounter is null.
			// *** If it's null my string is complete 
			// *** and I can give the string to the Crc_BuildCrc check function.
			if(g_CmdBuf.frame_counter == 0)
			{
#ifndef NO_CRC
                // *** verify Crc_BuildCrc checksum
                if( (g_CmdBuf.CrcL == g_CmdBuf.cmd_buf[g_CmdBuf.cmd_counter - 1]) &&
                    (g_CmdBuf.CrcH == g_CmdBuf.cmd_buf[g_CmdBuf.cmd_counter - 2]) )
#endif
                {
					// *** Execute the simple Commands
					switch(g_CmdBuf.cmd_buf[2])
					{
						case DELETE: 
							{
								SetEepromPointer(CMD_POINTER_ADDRESS,0);
								SetEepromPointer(LOOP_POINTER_ADDRESS,0);
								UART_Send('D');
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
								g_CmdBuf.LoopMode = 1;
								UART_Send('L');
								UART_Send('1');
								return;
							}
						case LOOP_OFF:
							{	
								g_CmdBuf.LoopMode = 0;
								g_CmdBuf.WaitValue = 0;
								UART_Send('L');
								UART_Send('0');
								return;
							}
						case START_BL:
							{
							#ifndef X86
								UART_SendString("Leaving Application --> Starting Bootloader");
								Eeprom_Write(0x3ff, 0xff);
								softReset();
							#endif
							}
						default:
							{
								if( Commandstorage_Write(&g_CmdBuf.cmd_buf[2], (g_CmdBuf.cmd_counter - 4)))
								{
									UART_Send('G');
									UART_Send('C');
								}
								else 
									g_ErrorBits.EepromFailure = 1;
							}
					}							
					
                }
#ifndef NO_CRC
                else
                {
                    // *** Do some error handling in case of an Crc_BuildCrc failure here
					g_ErrorBits.CrcFailure = 1;
                    return;
                }
#endif
			}
		}
	}
}

void Commandstorage_ExecuteCommands()
{
	// *** get the pointer to commands in the EEPROM
	struct led_cmd nextCmd;

	// read next command from eeprom and move command pointer in eeprom to the next command (TRUE)
	struct led_cmd *result = Commandstorage_Read(&nextCmd);
		
	if(0 != result)
	{
#ifdef TEST
		UART_SendString("executeCommand");
#endif
		// *** commands available, check what to do
		switch(nextCmd.cmd) 
		{	
			case SET_COLOR: 
			{
#ifdef TEST
				UART_SendString("SET_COLOR");
				UART_SendNumber(nextCmd.data.set_color.addr[0],'A');
				UART_SendNumber(nextCmd.data.set_color.addr[1],'A');
				UART_SendNumber(nextCmd.data.set_color.addr[2],'A');
				UART_SendNumber(nextCmd.data.set_color.addr[3],'A');
				UART_SendNumber(nextCmd.data.set_color.red,'R');
				UART_SendNumber(nextCmd.data.set_color.green,'G');
				UART_SendNumber(nextCmd.data.set_color.blue,'B');
#endif
				Ledstrip_SetColor(&nextCmd.data.set_color);
				break;
			}
			case SET_FADE:
			{
#ifdef TEST
				UART_SendString("SET_FADE");
				UART_SendNumber(nextCmd.data.set_fade.addr[0],'A');
				UART_SendNumber(nextCmd.data.set_fade.addr[1],'A');
				UART_SendNumber(nextCmd.data.set_fade.addr[2],'A');
				UART_SendNumber(nextCmd.data.set_fade.addr[3],'A');
				UART_SendNumber(nextCmd.data.set_fade.red,'R');
				UART_SendNumber(nextCmd.data.set_fade.green,'G');
				UART_SendNumber(nextCmd.data.set_fade.blue,'B');
				UART_SendNumber(nextCmd.data.set_fade.fadeTmms.high8,'H');
				UART_SendNumber(nextCmd.data.set_fade.fadeTmms.low8,'L');
#endif
				Ledstrip_SetFade(&nextCmd.data.set_fade);
				break;
			}
			case WAIT:
			{
				struct cmd_wait *pCmd = &nextCmd.data.wait;
#ifdef TEST
				UART_SendNumber(pCmd->valueH,'H');
				UART_SendNumber(pCmd->valueL,'L');
#endif

#ifndef X86
				g_CmdBuf.WaitValue.high8 = pCmd->valueH;
				g_CmdBuf.WaitValue.low8 = pCmd->valueL;
#else
				g_CmdBuf.WaitValue = pCmd->valueH << 8;
				g_CmdBuf.WaitValue |= 0x00ff & pCmd->valueL;
#endif
				break;
			}
			case SET_RUN: {break;}
		}
	}
}

void Commandstorage_Init()
{
	/** EEPROM contains FF in every cell after inital start,
	*** so I have to delete the pointer address
	*** otherwise the PIC thinks he has the EEPROM full with commands
	**/
#ifndef TEST_COMMAND
	if (GetEepromPointer(CMD_POINTER_ADDRESS) == 0xff)
#endif
		SetEepromPointer(CMD_POINTER_ADDRESS, 0);
#ifdef TEST_COMMAND
	uns16 cmdptr = GetEepromPointer(CMD_POINTER_ADDRESS);
	UART_SendNumber(cmdptr.high8, 'H');
	UART_SendNumber(cmdptr.low8, 'L');
#endif
	g_CmdBuf.LoopMode = 0;
	g_CmdBuf.WaitValue = 0;

	// set loop pointer address to start
	SetEepromPointer(LOOP_POINTER_ADDRESS, 0);
}

