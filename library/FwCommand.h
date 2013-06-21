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

#ifndef __WiflyControlCli__FwCommand__
#define __WiflyControlCli__FwCommand__

#include "FwResponse.h"
#include "WiflyColor.h"

#include <algorithm>
#include <iostream>

namespace WyLight {

class FwCommand
{
	const size_t mSize;
protected:
	struct led_cmd mReqFrame;
	FwCommand(uint8_t cmd, size_t size = 0) : mSize(1 + size) {	mReqFrame.cmd = cmd; };
	virtual ~FwCommand(void) = default;
	
public:
	const uint8_t* GetData(void) const { return reinterpret_cast<const uint8_t*>(&mReqFrame);	};
	size_t GetSize(void) const { return mSize; };
	virtual FwResponse& GetResponse(void) = 0;
};

struct FwCmdGet : public FwCommand
{
	FwCmdGet(uint8_t cmd) : FwCommand(cmd) {};
};

class FwCmdSimple : public FwCommand
{
	FwResponse mResponse;
protected:
	FwCmdSimple(uint8_t cmd, size_t size = 0) : FwCommand(cmd, size), mResponse(cmd) {};
public:
	FwResponse& GetResponse(void) { return mResponse;	};
};

class FwCmdScript : public FwCmdSimple
{
protected:
	FwCmdScript(uint8_t cmd, size_t size = 0) : FwCmdSimple(cmd, size) {};

public:
	static const size_t INDENTATION_MAX = 10;
	static const char INDENTATION_CHARACTER = ' ';

	virtual bool Equals(const FwCmdScript& ref) const {
		return (typeid(*this) == typeid(ref));
	};

	virtual	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		const size_t numCharacters = std::min(INDENTATION_MAX, 2 * indentation);
		return out << std::string(numCharacters, INDENTATION_CHARACTER);
	};
};

class FwCmdWait : public FwCmdScript
{
public:
	static const std::string TOKEN;
	FwCmdWait(std::istream& is) : FwCmdScript(WAIT, sizeof(cmd_wait)) {
		uint16_t waitTime;
		is >> waitTime;
		mReqFrame.data.wait.waitTmms = htons(std::max((uint16_t)1, waitTime));		
	};

	FwCmdWait(uint16_t waitTime) : FwCmdScript(WAIT, sizeof(cmd_wait)) {
		mReqFrame.data.wait.waitTmms = htons(std::max((uint16_t)1, waitTime));
	};

	bool Equals(const FwCmdScript& ref) const
	{
		if(!FwCmdScript::Equals(ref)) {
			return false;
		}
		auto r = reinterpret_cast<const FwCmdWait&>(ref);
		return mReqFrame.data.wait.waitTmms == r.mReqFrame.data.wait.waitTmms;
	};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return FwCmdScript::Write(out, indentation) << TOKEN << ' ' << std::dec << ntohs(mReqFrame.data.wait.waitTmms);
	};
};

struct FwCmdClearScript : public FwCmdSimple
{
	FwCmdClearScript(void) : FwCmdSimple(CLEAR_SCRIPT) {};
};

struct FwCmdGetCycletime : public FwCmdGet
{
	CycletimeResponse mResponse;
	FwCmdGetCycletime(void) : FwCmdGet(GET_CYCLETIME) {};
	FwResponse& GetResponse(void) { return mResponse;	};
};

struct FwCmdGetRtc : public FwCmdGet
{
	RtcResponse mResponse;
	FwCmdGetRtc(void) : FwCmdGet(GET_RTC) {};
	FwResponse& GetResponse(void) { return mResponse;	};
};

struct FwCmdGetTracebuffer : public FwCmdGet
{
	TracebufferResponse mResponse;
	FwCmdGetTracebuffer(void) : FwCmdGet(GET_TRACE) {};
	FwResponse& GetResponse(void) { return mResponse;	};
};

struct FwCmdGetVersion : public FwCmdGet
{
	FirmwareVersionResponse mResponse;
	FwCmdGetVersion(void) : FwCmdGet(GET_FW_VERSION) {};
	FwResponse& GetResponse(void) { return mResponse;	};
};

struct FwCmdLoopOff : public FwCmdScript
{
	static const std::string TOKEN;
	FwCmdLoopOff(std::istream& is) : FwCmdScript(LOOP_OFF, sizeof(cmd_loop_end)) {
		int numLoops;
		is >> numLoops;
		mReqFrame.data.loopEnd.numLoops = (uint8_t)numLoops;		
	};

	FwCmdLoopOff(uint8_t numLoops = 0) : FwCmdScript(LOOP_OFF, sizeof(cmd_loop_end)) {
		mReqFrame.data.loopEnd.numLoops = numLoops;
	};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return FwCmdScript::Write(out, --indentation) << TOKEN << ' ' << std::dec << (int)mReqFrame.data.loopEnd.numLoops;
	};
};

struct FwCmdLoopOn : public FwCmdScript
{
	static const std::string TOKEN;
	FwCmdLoopOn(void) : FwCmdScript(LOOP_ON) {};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		FwCmdScript::Write(out, indentation) << TOKEN;
		++indentation;
		return out;
	};
};

class FwCmdSetColorDirect : public FwCmdSimple
{
public:
	FwCmdSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) : FwCmdSimple(SET_COLOR_DIRECT, sizeof(cmd_set_color_direct)) {
		static const size_t maxBufferLength = NUM_OF_LED * 3;
		bufferLength = std::min(bufferLength, maxBufferLength);
		memcpy(mReqFrame.data.set_color_direct.ptr_led_array, pBuffer, bufferLength);
		memset(mReqFrame.data.set_color_direct.ptr_led_array + bufferLength, 0, maxBufferLength - bufferLength);
	};
};

struct FwCmdSetFade : public FwCmdScript
{
	static const std::string TOKEN;
	FwCmdSetFade(std::istream& is) : FwCmdScript(SET_FADE, sizeof(cmd_set_fade)) {
		WiflyColor addr, argb;
		uint16_t fadeTime;
		bool parallelFade;
		is >> addr >> argb >> parallelFade >> fadeTime;
		mReqFrame.data.set_fade.Set(addr.argb(), argb.argb(), (uint8_t)parallelFade, fadeTime);
	};
	FwCmdSetFade(uint32_t argb, uint16_t fadeTime = 0, uint32_t addr = 0xffffffff, bool parallelFade = false)
	: FwCmdScript(SET_FADE, sizeof(cmd_set_fade)) {
		mReqFrame.data.set_fade.Set(addr, argb, (uint8_t)parallelFade, fadeTime);	
	};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		FwCmdScript::Write(out, indentation) << TOKEN << ' ';
		return mReqFrame.data.set_fade.Write(out, indentation);
	};
};

struct FwCmdSetGradient : public FwCmdScript
{
	static const std::string TOKEN;
	FwCmdSetGradient(std::istream& is) : FwCmdScript(SET_GRADIENT, sizeof(cmd_set_gradient)) {		
		WiflyColor argb_1, argb_2;
		uint16_t fadeTime;
		int parallel, length, offset;	
		is >> argb_1 >> argb_2 >> parallel >> offset >> length >> fadeTime;
		mReqFrame.data.set_gradient.Set(argb_1.argb(), argb_2.argb(), parallel, (uint8_t)offset, (uint8_t)length, fadeTime);
	};

	FwCmdSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime = 0, bool parallelFade = false, uint8_t length = NUM_OF_LED, uint8_t offset = 0) : FwCmdScript(SET_GRADIENT, sizeof(cmd_set_gradient)) {

		mReqFrame.data.set_gradient.Set(argb_1, argb_2, parallelFade, offset, length, fadeTime);
};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		FwCmdScript::Write(out, indentation) << TOKEN << ' ';
		return mReqFrame.data.set_gradient.Write(out, indentation);
	};
};

struct FwCmdSetRtc : public FwCmdSimple
{
	FwCmdSetRtc(const tm& timeValue) : FwCmdSimple(SET_RTC, sizeof(rtc_time))
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

/**
 * Stops firmware and script controller execution and start the bootloader of the wifly device
 */
struct FwCmdStartBl : public FwCmdSimple
{
	FwCmdStartBl(void) : FwCmdSimple(START_BL) {};
};

}

#endif /* defined(__WiflyControlCli__FwCommand__) */
