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

bank2 struct CommandBuffer g_CmdBuf;

void Commandstorage_GetCommands()
{	
	if(RingBuf_HasError(&g_RingBuf))
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		Commandstorage_Clear();
	}

	if(RingBuf_IsEmpty(&g_RingBuf))
	{
		return;
	}

	// *** get new_byte from ringbuffer
	uns8 new_byte = RingBuf_Get(&g_RingBuf);

		// *** do I wait for databytes?
		if(g_CmdBuf.frame_counter == 0)
		{
			// *** I don't wait for databytes
			// *** Do I receive a Start_of_Text sign
			if(new_byte == STX)
			{
				// *** increase the cmd_counter
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
				//Ledstrip_Test(7+3);
				// *** check if I get the framelength byte
				if((new_byte < (CMDFRAMELENGTH - 2)) && (g_CmdBuf.cmd_counter == 1))
				{
					g_CmdBuf.frame_counter = new_byte;
					g_CmdBuf.cmd_buf[1] = new_byte;
					g_CmdBuf.cmd_counter = 2;
					Crc_AddCrc(new_byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
				}
			}
		}
		else
		{
			//Ledstrip_Test(7+5);
			// *** I wait for Databytes, so I save all bytes 
			// *** that I get until my framecounter is > 0
			g_CmdBuf.cmd_buf[g_CmdBuf.cmd_counter] = new_byte;
			g_CmdBuf.cmd_counter++;
			
			// *** add new_byte to Crc_BuildCrc checksum
      Crc_AddCrc(new_byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
			g_CmdBuf.frame_counter--;

			/**
			 * if the frame_counter reaches zero, we already read all bytes of the
			 * frame including the crc at the end. So our calculated crc should be
			 * zero, now.
			 */
			if(g_CmdBuf.frame_counter == 0)
			{
				//Ledstrip_Test(2);
				Trace_String("Read ");
				Trace_Number(g_CmdBuf.cmd_counter);
				Trace_String(" bytes: \n");
				uns8 i;
				for(i = 0; i <= g_CmdBuf.cmd_counter; i++)
				{
					Trace_Hex(g_CmdBuf.cmd_buf[i]);
				}
				Trace_String("\n");
#ifndef NO_CRC
				// *** verify Crc_BuildCrc checksum
				if((0 == g_CmdBuf.CrcL) && (0 == g_CmdBuf.CrcH))
#endif
					{
						if(ScriptCtrl_Add(&g_CmdBuf.cmd_buf[2]))
						{
									UART_SendString("GC");
						}
						else
						{
									g_ErrorBits.EepromFailure = 1;
						}
					}
#ifndef NO_CRC
			else
			{
				// *** Do some error handling in case of an Crc_BuildCrc failure here
				g_ErrorBits.CrcFailure = 1;
				//Ledstrip_Test(4);
				Trace_Hex(g_CmdBuf.CrcL);
				Trace_Hex(g_CmdBuf.CrcH);
				Trace_Hex(g_CmdBuf.cmd_buf[g_CmdBuf.cmd_counter - 1]);
				Trace_Hex(g_CmdBuf.cmd_buf[g_CmdBuf.cmd_counter]);
				Trace_String("\n");
				return;
			}
#endif
			}
		  
		}
	
}

