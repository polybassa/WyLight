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
#include "ScriptCtrl.h"
#include "ledstrip.h"
#include "rtc.h"
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

		// *** do I wait for databytes?
		if(g_CmdBuf.frame_counter == 0)
		{
			// *** I don't wait for databytes
			// *** Do I receive a Start_of_Text sign
			if(new_byte == STX)
			{
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
					ScriptCtrl_Add(&g_CmdBuf.cmd_buf[2]);
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

