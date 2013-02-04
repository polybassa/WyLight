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

#ifndef _COMMANDIO_H_
#define _COMMANDIO_H_

#include "platform.h"
#include "wifly_cmd.h"

#define CMDFRAMELENGTH NUM_OF_LED*3+5

/** Statemachine STATES **/
#define CS_WaitForSTX 0
#define	CS_DeleteBuffer 1
#define	CS_UnMaskChar 2
#define CS_SaveChar 3

struct CommandBuffer{
    uns8 buffer[CMDFRAMELENGTH];
    uns8 counter;
    uns8 state;
    uns8 CrcH;
    uns8 CrcL;
};
extern bank2 struct CommandBuffer g_CmdBuf;

void CommandIO_Init();

void CommandIO_GetCommands();

void CommandIO_SendResponse(struct response_frame *pFrame);

#endif /* #ifndef _COMMANDSTORAGE_H_ */

