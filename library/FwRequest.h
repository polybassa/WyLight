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

#include "wifly_cmd.h"
#include <stdio.h>
#include <stdint.h>

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

class FwRequest
{
protected:
	FwRequest() {}
	FwRequest(size_t size) : mSize(1 + size) {};
	FwRequest( const FwRequest& other ) = default;
	FwRequest& operator=( const FwRequest& ) = default;
	const size_t mSize = 1;
	struct led_cmd mCmdFrame;
public:
	const uint8_t* GetData(void) const { return reinterpret_cast<const uint8_t*>(&mCmdFrame); };
	size_t GetSize() const { return mSize; };
};

class FwCmdWait : public FwRequest
{
public:
	FwCmdWait(uns16 waitTime) : FwRequest(sizeof(cmd_wait)) { mCmdFrame.cmd = WAIT; mCmdFrame.data.wait.waitTmms = htons(waitTime); };
};

class FwCmdClearScript : public FwRequest
{
public:
	FwCmdClearScript() : FwRequest() { mCmdFrame.cmd = CLEAR_SCRIPT; };
};

class FwCmdGetCycletime : public FwRequest
{
public:
	FwCmdGetCycletime() : FwRequest() { mCmdFrame.cmd = GET_CYCLETIME; };
};

class FwCmdGetRtc : public FwRequest
{
public:
	FwCmdGetRtc() : FwRequest() { mCmdFrame.cmd = GET_RTC; };
};

class FwCmdGetTracebuffer : public FwRequest
{
public:
	FwCmdGetTracebuffer() : FwRequest() { mCmdFrame.cmd = GET_TRACE; };
};

class FwCmdGetVersion : public FwRequest
{
public:
	FwCmdGetVersion() : FwRequest() { mCmdFrame.cmd = GET_FW_VERSION; };
};

class FwCmdLoopOff : public FwRequest
{
public:
	FwCmdLoopOff(uint8_t numLoops) : FwRequest(sizeof(cmd_loop_end)) { mCmdFrame.cmd = LOOP_OFF; mCmdFrame.data.loopEnd.numLoops = numLoops; };
};

class FwCmdLoopOn : public FwRequest
{
public:
	FwCmdLoopOn() : FwRequest() { mCmdFrame.cmd = LOOP_ON; };
};

class FwCmdSetColorDirect : public FwRequest
{
public:
	FwCmdSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) : FwRequest(sizeof(struct cmd_set_color_direct))
	{
		static const size_t maxBufferLength = NUM_OF_LED * 3;
		mCmdFrame.cmd = SET_COLOR_DIRECT;
		bufferLength = std::min(bufferLength, maxBufferLength);
		memcpy(mCmdFrame.data.set_color_direct.ptr_led_array, pBuffer, bufferLength);
		memset(mCmdFrame.data.set_color_direct.ptr_led_array + bufferLength, 0, maxBufferLength - bufferLength);
	};
};

class FwCmdSetFade : public FwRequest
{
public:
	FwCmdSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade) : FwRequest(sizeof(cmd_set_fade))
	{
		mCmdFrame.cmd = SET_FADE;
		SetAddrRgb(mCmdFrame.data.set_fade, addr, argb);
		
		// ommit fadeTime == 0
		fadeTime = ((0 == fadeTime) ? 1 : fadeTime);
		mCmdFrame.data.set_fade.fadeTmms = htons(fadeTime);
		mCmdFrame.data.set_fade.parallelFade = (parallelFade ? 1 : 0);
	};
};

class FwCmdSetRtc : public FwRequest
{
public:
	FwCmdSetRtc(const tm& timeValue) : FwRequest(sizeof(rtc_time))
	{
		mCmdFrame.cmd = SET_RTC;
		mCmdFrame.data.set_rtc.tm_sec  = (uns8) timeValue.tm_sec;
		mCmdFrame.data.set_rtc.tm_min  = (uns8) timeValue.tm_min;
		mCmdFrame.data.set_rtc.tm_hour = (uns8) timeValue.tm_hour;
		mCmdFrame.data.set_rtc.tm_mday = (uns8) timeValue.tm_mday;
		mCmdFrame.data.set_rtc.tm_mon  = (uns8) timeValue.tm_mon;
		mCmdFrame.data.set_rtc.tm_year = (uns8) timeValue.tm_year;
		mCmdFrame.data.set_rtc.tm_wday = (uns8) timeValue.tm_wday;
	};
};

class FwCmdStartBl : public FwRequest
{
public:
	FwCmdStartBl() : FwRequest() { mCmdFrame.cmd = START_BL; };
};




#endif
