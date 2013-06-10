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



class FwRequest
{
protected:
	FwRequest(uint8_t cmd, size_t size = 0) : mSize(1 + size) { mReqFrame.cmd = cmd; };
	
	const size_t mSize;
	struct led_cmd mReqFrame;
public:
	const uint8_t* GetData(void) const { return reinterpret_cast<const uint8_t*>(&mReqFrame); };
	size_t GetSize(void) const { return mSize; };
};

class FwReqScript : public FwRequest
{
protected:
	FwReqScript(uint8_t cmd, size_t size = 0) : FwRequest(cmd, size) {};

public:
	static const size_t INDENTATION_MAX = 10;
	static const char INDENTATION_CHARACTER = ' ';
	virtual std::ostream& Write(std::ostream& out, size_t& indentation) const {
		const size_t numCharacters = std::min(INDENTATION_MAX, 2 * indentation);
		for(size_t i = 0; i < numCharacters; ++i) {
			out << INDENTATION_CHARACTER;
		}
		return out;
	};
};

class FwReqWait : public FwReqScript
{
public:
	FwReqWait(std::istream& is) : FwReqScript(WAIT, sizeof(cmd_wait)) {
		uint16_t waitTime;
		is >> waitTime;
		setTimeValue(waitTime);
	};
	FwReqWait(uint16_t waitTime) : FwReqScript(WAIT, sizeof(cmd_wait)) {
		setTimeValue(waitTime);
	};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		timeValue = ((0 == timeValue) ? 1 : timeValue);
		mReqFrame.data.wait.waitTmms = htons(timeValue);
	};
	
	virtual uint16_t getTimeValue(void) const
	{
		return ntohs(mReqFrame.data.wait.waitTmms);
	};

	std::ostream& Write(std::ostream& out, size_t& indentation) const
	{
		return FwReqScript::Write(out, indentation) << "wait " << std::dec << htons(mReqFrame.data.wait.waitTmms);
	}
};

struct FwReqClearScript : public FwRequest
{
	FwReqClearScript(void) : FwRequest(CLEAR_SCRIPT) {};
};

struct FwReqGetCycletime : public FwRequest
{
	FwReqGetCycletime(void) : FwRequest(GET_CYCLETIME) {};
};

struct FwReqGetRtc : public FwRequest
{
	FwReqGetRtc(void) : FwRequest(GET_RTC) {};
};

struct FwReqGetTracebuffer : public FwRequest
{
	FwReqGetTracebuffer(void) : FwRequest(GET_TRACE) {};
};

class FwReqGetVersion : public FwRequest
{
public:
	FwReqGetVersion(void) : FwRequest(GET_FW_VERSION) {};
};

class FwReqLoopOff : public FwReqScript
{
public:
	FwReqLoopOff(std::istream& is) : FwReqScript(LOOP_OFF, sizeof(cmd_loop_end)) {
		int value;
		is >> value;
		mReqFrame.data.loopEnd.numLoops = (uint8_t)value;
	};
	FwReqLoopOff(uint8_t numLoops) : FwReqScript(LOOP_OFF, sizeof(cmd_loop_end)) {
		mReqFrame.data.loopEnd.numLoops = numLoops;
	};

	std::ostream& Write(std::ostream& out, size_t& indentation) const
	{
		return FwReqScript::Write(out, --indentation) << "loop_off " << std::dec << (int)mReqFrame.data.loopEnd.numLoops;
	};
};

class FwReqLoopOn : public FwReqScript
{
public:
	FwReqLoopOn(void) : FwReqScript(LOOP_ON) {};

	std::ostream& Write(std::ostream& out, size_t& indentation) const
	{
		FwReqScript::Write(out, indentation) << "loop";
		++indentation;
		return out;
	}
};

class FwReqSetColorDirect : public FwRequest
{
public:
	FwReqSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) : FwRequest(SET_COLOR_DIRECT, sizeof(struct cmd_set_color_direct))
	{
		static const size_t maxBufferLength = NUM_OF_LED * 3;
		bufferLength = std::min(bufferLength, maxBufferLength);
		memcpy(mReqFrame.data.set_color_direct.ptr_led_array, pBuffer, bufferLength);
		memset(mReqFrame.data.set_color_direct.ptr_led_array + bufferLength, 0, maxBufferLength - bufferLength);
	};
};

class FwReqSetFade : public FwReqScript
{
	WiflyColor addr;
	WiflyColor argb;
	uint16_t fadeTime;
	bool parallelFade;
public:
	FwReqSetFade(std::istream& is) : FwReqScript(SET_FADE, sizeof(cmd_set_fade)) {
		is >> addr >> argb >> parallelFade >> fadeTime;
		SetAddrRgb(mReqFrame.data.set_fade, addr.argb(), argb.argb());
		mReqFrame.data.set_fade.parallelFade = parallelFade;
		this->setTimeValue(fadeTime);
	};
	FwReqSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade) : FwReqScript(SET_FADE, sizeof(cmd_set_fade))
	{
		SetAddrRgb(mReqFrame.data.set_fade, addr, argb);
		mReqFrame.data.set_fade.parallelFade = parallelFade;
		this->setTimeValue(fadeTime);
	};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		fadeTime = timeValue;
		timeValue = ((0 == timeValue) ? 1 : timeValue);
		mReqFrame.data.set_fade.fadeTmms = htons(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ntohs(mReqFrame.data.set_fade.fadeTmms);
	};

	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return FwReqScript::Write(out, indentation) << "fade " << std::hex << addr << ' ' << argb << ' ' << std::dec << parallelFade << ' ' << fadeTime;
	};
};

class FwReqSetGradient : public FwReqScript
{
	WiflyColor argb_1;
	WiflyColor argb_2;
	uint16_t fadeTime;
	bool parallelFade;
	uint8_t length;
	uint8_t offset;	
public:
	FwReqSetGradient(std::istream& is) : FwReqScript(SET_GRADIENT, sizeof(cmd_set_gradient)) {
		int __length;
		int __offset;
		is >> argb_1 >> argb_2 >> parallelFade >> __offset >> __length >> fadeTime;
		offset = __offset;
		length = __length;
		if (offset > 0x7f) throw FatalError("Invalid Parameter, offset is greater than 127");
		
		SetRgb_1(mReqFrame.data.set_gradient, argb_1.argb());
		SetRgb_2(mReqFrame.data.set_gradient, argb_2.argb());
		
		mReqFrame.data.set_gradient.numberOfLeds = length;
		mReqFrame.data.set_gradient.setOffset(offset);
		mReqFrame.data.set_gradient.setParallelFade(parallelFade);
		
		this->setTimeValue(fadeTime);
	};
	FwReqSetGradient(uint32_t __argb_1, uint32_t __argb_2, uint16_t __fadeTime, bool __parallelFade, uint8_t __length, uint8_t __offset) : FwReqScript(SET_GRADIENT, sizeof(cmd_set_gradient))
	{
		if (offset > 0x7f) throw FatalError("Invalid Parameter, offset is greater than 127");

		argb_1.argb(__argb_1);
		argb_2.argb(__argb_2);
		fadeTime = __fadeTime;
		parallelFade = __parallelFade;
		length = __length;
		offset = __offset;	
		
		SetRgb_1(mReqFrame.data.set_gradient, argb_1.argb());
		SetRgb_2(mReqFrame.data.set_gradient, argb_2.argb());
		
		mReqFrame.data.set_gradient.numberOfLeds = length;
		mReqFrame.data.set_gradient.setOffset(offset);
		mReqFrame.data.set_gradient.setParallelFade(parallelFade);
		
		this->setTimeValue(fadeTime);
	};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		timeValue = ((0 == timeValue) ? 1 : timeValue);
		mReqFrame.data.set_gradient.fadeTmms = htons(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ntohs(mReqFrame.data.set_gradient.fadeTmms);
	};

	std::ostream& Write(std::ostream& out, size_t& indentation) const
	{
		return FwReqScript::Write(out, indentation) << "gradient " << argb_1 << ' ' << argb_2 << ' ' << std::dec << parallelFade << ' ' << (int)offset << ' ' << (int)length << ' ' << fadeTime;
	};
};

class FwReqSetRtc : public FwRequest
{
public:
	FwReqSetRtc(const tm& timeValue) : FwRequest(SET_RTC, sizeof(rtc_time))
	{
		mReqFrame.data.set_rtc.tm_sec  = (uns8) timeValue.tm_sec;
		mReqFrame.data.set_rtc.tm_min  = (uns8) timeValue.tm_min;
		mReqFrame.data.set_rtc.tm_hour = (uns8) timeValue.tm_hour;
		mReqFrame.data.set_rtc.tm_mday = (uns8) timeValue.tm_mday;
		mReqFrame.data.set_rtc.tm_mon  = (uns8) timeValue.tm_mon;
		mReqFrame.data.set_rtc.tm_year = (uns8) timeValue.tm_year;
		mReqFrame.data.set_rtc.tm_wday = (uns8) timeValue.tm_wday;
	};
};

struct FwReqStartBl : public FwRequest
{
	FwReqStartBl(void) : FwRequest(START_BL) {};
};
}
#endif
