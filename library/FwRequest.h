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

#ifndef WiflyControlCli_FwRequest_h
#define WiflyControlCli_FwRequest_h

#include "WiflyColor.h"
#include "WiflyControlException.h"
#include "wifly_cmd.h"

#include <iostream>
#include <limits>
#include <stdio.h>

namespace WyLight {

/**
 * Macro to reduce code redundancy, while converting two 32 bit values into
 * an address array and red, green, blue values.
 */
#define SetAddrRgb(REF, ADDRESS, ARGB) { \
REF.addr[3] = ((ADDRESS) & 0xff000000) >> 24; \
REF.addr[2] = ((ADDRESS) & 0x00ff0000) >> 16; \
REF.addr[1] = ((ADDRESS) & 0x0000ff00) >> 8; \
REF.addr[0] = ((ADDRESS) & 0x000000ff); \
REF.red = ((ARGB) & 0x00ff0000) >> 16; \
REF.green = ((ARGB) & 0x0000ff00) >> 8; \
REF.blue = (ARGB) & 0x000000ff; \
}

#define SetRgb_1(REF, ARGB) { \
REF.red_1 = ((ARGB) & 0x00ff0000) >> 16; \
REF.green_1 = ((ARGB) & 0x0000ff00) >> 8; \
REF.blue_1 = (ARGB) & 0x000000ff; \
}

#define SetRgb_2(REF, ARGB) { \
REF.red_2 = ((ARGB) & 0x00ff0000) >> 16; \
REF.green_2 = ((ARGB) & 0x0000ff00) >> 8; \
REF.blue_2 = (ARGB) & 0x000000ff; \
}


struct IFwRequest
{
	virtual const uint8_t* GetData(void) const = 0;
	virtual size_t GetSize(void) const = 0;
};

class FwRequest : public IFwRequest
{
protected:	
	const size_t mSize;
	struct led_cmd mReqFrame;
public:
	FwRequest(uint8_t cmd, size_t size = 0) : mSize(1 + size) { mReqFrame.cmd = cmd; };
	virtual const uint8_t* GetData(void) const { return reinterpret_cast<const uint8_t*>(&mReqFrame); };
	size_t GetSize(void) const { return mSize; };
};

}
#endif
