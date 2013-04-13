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
	FwRequest() : mSize(1) {}
	FwRequest(size_t size) : mSize(1 + size) {};
	FwRequest( const FwRequest& other ) = default;
	FwRequest& operator=( const FwRequest& ) = default;
	const size_t mSize;
	struct led_cmd mReqFrame;
public:
	const uint8_t* GetData(void) const { return reinterpret_cast<const uint8_t*>(&mReqFrame); };
	size_t GetSize() const { return mSize; };
};

class FwReqWait : public FwRequest
{
public:
	FwReqWait(uns16 waitTime) : FwRequest(sizeof(cmd_wait)) { mReqFrame.cmd = WAIT; mReqFrame.data.wait.waitTmms = htons(waitTime); };
};

class FwReqClearScript : public FwRequest
{
public:
	FwReqClearScript() : FwRequest() { mReqFrame.cmd = CLEAR_SCRIPT; };
};

class FwReqGetCycletime : public FwRequest
{
public:
	FwReqGetCycletime() : FwRequest() { mReqFrame.cmd = GET_CYCLETIME; };
};

class FwReqGetRtc : public FwRequest
{
public:
	FwReqGetRtc() : FwRequest() { mReqFrame.cmd = GET_RTC; };
};

class FwReqGetTracebuffer : public FwRequest
{
public:
	FwReqGetTracebuffer() : FwRequest() { mReqFrame.cmd = GET_TRACE; };
};

class FwReqGetVersion : public FwRequest
{
public:
	FwReqGetVersion() : FwRequest() { mReqFrame.cmd = GET_FW_VERSION; };
};

class FwReqLoopOff : public FwRequest
{
public:
	FwReqLoopOff(uint8_t numLoops) : FwRequest(sizeof(cmd_loop_end)) { mReqFrame.cmd = LOOP_OFF; mReqFrame.data.loopEnd.numLoops = numLoops; };
};

class FwReqLoopOn : public FwRequest
{
public:
	FwReqLoopOn() : FwRequest() { mReqFrame.cmd = LOOP_ON; };
};

class FwReqSetColorDirect : public FwRequest
{
public:
	FwReqSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) : FwRequest(sizeof(struct cmd_set_color_direct))
	{
		static const size_t maxBufferLength = NUM_OF_LED * 3;
		mReqFrame.cmd = SET_COLOR_DIRECT;
		bufferLength = std::min(bufferLength, maxBufferLength);
		memcpy(mReqFrame.data.set_color_direct.ptr_led_array, pBuffer, bufferLength);
		memset(mReqFrame.data.set_color_direct.ptr_led_array + bufferLength, 0, maxBufferLength - bufferLength);
	};
};

class FwReqSetFade : public FwRequest
{
public:
	FwReqSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade) : FwRequest(sizeof(cmd_set_fade))
	{
		mReqFrame.cmd = SET_FADE;
		SetAddrRgb(mReqFrame.data.set_fade, addr, argb);
		
		// ommit fadeTime == 0
		fadeTime = ((0 == fadeTime) ? 1 : fadeTime);
		mReqFrame.data.set_fade.fadeTmms = htons(fadeTime);
		mReqFrame.data.set_fade.parallelFade = (parallelFade ? 1 : 0);
	};
	FwReqSetFade(uint32_t argb) : FwReqSetFade(argb, 0, 0xffffffff, false) {};
	FwReqSetFade(uint32_t argb, uint16_t fadeTime) : FwReqSetFade(argb, fadeTime, 0xffffffff, false) {};
	FwReqSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr) : FwReqSetFade(argb, fadeTime, addr, false) {};
	
};

class FwReqSetRtc : public FwRequest
{
public:
	FwReqSetRtc(const tm& timeValue) : FwRequest(sizeof(rtc_time))
	{
		mReqFrame.cmd = SET_RTC;
		mReqFrame.data.set_rtc.tm_sec  = (uns8) timeValue.tm_sec;
		mReqFrame.data.set_rtc.tm_min  = (uns8) timeValue.tm_min;
		mReqFrame.data.set_rtc.tm_hour = (uns8) timeValue.tm_hour;
		mReqFrame.data.set_rtc.tm_mday = (uns8) timeValue.tm_mday;
		mReqFrame.data.set_rtc.tm_mon  = (uns8) timeValue.tm_mon;
		mReqFrame.data.set_rtc.tm_year = (uns8) timeValue.tm_year;
		mReqFrame.data.set_rtc.tm_wday = (uns8) timeValue.tm_wday;
	};
};

class FwReqStartBl : public FwRequest
{
public:
	FwReqStartBl() : FwRequest() { mReqFrame.cmd = START_BL; };
};




#endif
