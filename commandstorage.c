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

#include "commandstorage.h"
#include "ScriptCtrl.h"
#include "trace.h"
#include "usart.h"
#include "RingBuf.h"
#include "crc.h"
#include "error.h"

bank2 struct CommandBuffer g_CmdBuf;

/** PRIVATE METHODE **/

void writeByte(uns8 byte)
{
    if(g_CmdBuf.counter < CMDFRAMELENGTH)
    {
	  g_CmdBuf.buffer[g_CmdBuf.counter] = byte;
	  g_CmdBuf.counter = g_CmdBuf.counter + 1;
	  Crc_AddCrc(byte, &g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
    }
    else
    {
	  g_ErrorBits.CmdBufOverflow = TRUE;
    }
}

void DeleteBuffer()
{
    g_CmdBuf.counter = 0;
    Crc_NewCrc(&g_CmdBuf.CrcH, &g_CmdBuf.CrcL);
}


void Commandstorage_Init()
{ 
    g_CmdBuf.state = CS_WaitForSTX;
    DeleteBuffer();
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
	if(g_ErrorBits.CmdBufOverflow)
	{
		Trace_String("Commandstorage_GetCommands(): CmdBufOverflow\n");
		return;
	}
  
	if(RingBuf_HasError(&g_RingBuf))
	{
		// *** if a RingBufError occure, I have to throw away the current command,
		// *** because the last byte was not saved. Commandstring is inconsistent
		Trace_String("Commandstorage_GetCommands(): RingBuf has error\n");
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
	      DeleteBuffer();
	      
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
			// [0] contains cmd_frame->length so we send [1]
#ifdef UNIT_TEST
			if(ScriptCtrl_Add((struct led_cmd*)&g_CmdBuf.buffer[1]))
#else
			if(ScriptCtrl_Add(&g_CmdBuf.buffer[1]))
#endif
			{
				UART_Send(STX);
				UART_Send('G');
				UART_Send('C');
				UART_Send(0xEC);
				UART_Send(0xFC);
				UART_Send(ETX);
			}
			else
			{
				g_ErrorBits.EepromFailure = 1;
			}
		  }
		  else
		  {
				g_ErrorBits.CrcFailure = 1;
				Trace_String("Crc error: ");
				Trace_Hex(g_CmdBuf.CrcL);
				Trace_Hex(g_CmdBuf.CrcH);
				Trace_Hex(g_CmdBuf.buffer[g_CmdBuf.counter - 2]);
				Trace_Hex(g_CmdBuf.buffer[g_CmdBuf.counter - 1]);
				Trace_String("\n");
		  }
		  break;
	      }
	      writeByte(new_byte);
	      break;
	  }
	}
}

