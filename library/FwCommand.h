/*
 Copyright (C) 2013 Nils Weiss, Patrick Bruenn.
 
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

#ifndef WiflyControlCli_FwCommand_h
#define WiflyControlCli_FwCommand_h

#include "wifly_cmd.h"
#include <stdio.h>
#include <stdint.h>

struct FwCommand
{

	FwCommand(size_t size, uint8_t cmd) : mSize(size), mCmdFrame{cmd} {};
	FwCommand( const FwCommand& other ) = delete; 
	FwCommand& operator=( const FwCommand& ) = delete; 
	const size_t mSize;
	const struct led_cmd mCmdFrame;
	const uint8_t* GetData(void) const { return reinterpret_cast<const uint8_t*>(&mCmdFrame); };
	size_t GetSize() const { return mSize; };
};

struct LedCommand : public FwCommand
{
	LedCommand(size_t size, uint8_t cmd) : FwCommand(size, cmd) {};
};

struct FwCmdWait : public LedCommand
{
	FwCmdWait() : LedCommand(1,WAIT){};
};


#endif
