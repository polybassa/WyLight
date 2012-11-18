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

#ifdef _old_commandstorage_

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
						Commandstorage_Clear();
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
				Commandstorage_Clear();
				return;
			}
#endif
			}
		  
		}
	
}

#else

#include "platform.h"
#include "commandstorage.h"
#include "ScriptCtrl.h"
#include "trace.h"

bank2 struct CommandBuffer g_CmdBuf;

/** PRIVATE METHODE **/

void writeByte(uns8 byte)
{
    g_CmdBuf.buffer[g_CmdBuf.counter] = byte;
    g_CmdBuf.counter++;
    Crc_AddCrc(byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
}

void Commandstorage_Init()
{
    g_CmdBuf.counter = 0;
    g_CmdBuf.state = CS_WaitForSTX;
    Crc_NewCrc(&g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
}

/** STATEMACHINE FOR GetCommands:
 * All ASCII-Chars are seperatet in 4 Groups
 * 	Group1: STX
 * 	Group2: ETX
 * 	Group3: DLE
 * 	Group4: All Elements of ASCII-Table without STX,ETX,DLE. I will call it CHAR in further description
 * 
 * The Statemachine has 4 different states
 * 	state 0: Wait for STX 		--> representet from CS_WaitForSTX
 * 		read DLE or ETX or CHAR 	--> new state = state 0 (nothing happens)
 *		read STX			--> new state = state 1
 * 	
 * 	state 1: Delete commandbuffer 	--> representet from CS_DeleteBuffer
 * 		read STX			--> new state = state 1 (nothing happens)
 * 		read ETX			--> new state = state 0
 * 		read DLE			--> new state = state 2
 * 		read CHAR			--> new state = state 3, save CHAR to commandbuffer, increment counter
 * 
 * 	state 2: Read mask character	--> representet from CS_UnMaskChar
 * 		read STX or ETX or DLE or CHAR	--> new state = state 3, save byte to commandbuffer, increment counter
 * 
 * 	state 3: Save Char 		--> representet from CS_SaveChar
 * 		read CHAR			--> new state = state 3, save CHAR to commandbuffer, increment counter
 * 		read DLE			--> new state = state 2
 * 		read STX			--> new state = state 1
 * 		read ETX			--> new state = state 0, do CRC-check, save dataframe 
 * 
 * **/

void Commandstorage_GetCommands()
{	
	if(RingBuf_HasError(&g_RingBuf))
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		Commandstorage_Init();
		return;
	}
	
	if(RingBuf_IsEmpty(&g_RingBuf))
	{
		return;
	}
	
	// *** get new_byte from ringbuffer
	uns8 new_byte = RingBuf_Get(&g_RingBuf);
	
	switch(g_CmdBuf.state)
	{
	  case CS_WaitForSTX:
	  {
	      if(new_byte == STX)
	      {
		  g_CmdBuf.state = CS_DeleteBuffer;
	      }
	      break;
	  }
	  case CS_DeleteBuffer:
	  {
	      Commandstorage_Init();
	      
	      if(new_byte == STX)
	      {
		  break;
	      }
	      
	      if(new_byte == ETX)
	      {
		  g_CmdBuf.state = CS_WaitForSTX;
		  break;
	      }
	      
	      if(new_byte == DLE)
	      {
		  g_CmdBuf.state = CS_UnMaskChar;
		  break;
	      }
	      
	      writeByte(new_byte);
	      g_CmdBuf.state = CS_SaveChar;
	      break;
	  }
	  case CS_UnMaskChar:
	  {
	      writeByte(new_byte);
	      g_CmdBuf.state = CS_SaveChar;
	      break;
	  }
	  case CS_SaveChar:
	  {
	      if(new_byte == DLE)
	      {
		  g_CmdBuf.state = CS_UnMaskChar;
		  break;
	      }
	      if(new_byte == STX)
	      {
		  g_CmdBuf.state = CS_DeleteBuffer;
		  break;
	      }
	      if(new_byte == ETX)
	      {
		  g_CmdBuf.state = CS_WaitForSTX;

		  if((0 == g_CmdBuf.CrcL) && (0 == g_CmdBuf.CrcH)) 	/* CRC Check */
		  {
#ifndef UNIT_TEST
			if(ScriptCtrl_Add(&g_CmdBuf.buffer[0]))
			{
				UART_SendString("GC");
			}
			else
			{
				g_ErrorBits.EepromFailure = 1;
			}
#endif
		  }
		  else
		  {
			g_ErrorBits.CrcFailure = 1;

			Trace_Hex(g_CmdBuf.CrcL);
			Trace_Hex(g_CmdBuf.CrcH);
			Trace_Hex(g_CmdBuf.buffer[g_CmdBuf.counter - 1]);
			Trace_Hex(g_CmdBuf.buffer[g_CmdBuf.counter]);
			Trace_String("\n");
		  }
		  break;
	      }
	      writeByte(new_byte);
	      break;
	  }
	}
}

#endif /* _old_commandstorage_ */