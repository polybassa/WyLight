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

#include <iostream>
#include "FwRequest.h"
#include "FwResponse.h"

namespace WyLight {

class FwCommand {
	FwCommand( const FwCommand& other ) = delete;
	FwCommand& operator=( const FwCommand& ) = delete;

protected:
	FwCommand(FwRequest* const req, FwResponse* const resp) : mRequest(req), mResponse(resp) {};
	
	FwRequest* const mRequest;
	FwResponse* const mResponse;

	
public:
	FwRequest* const GetRequest(void) const {return mRequest; };
	FwResponse* const GetResponse(void) const {return mResponse; };
};

class FwCmdWait : public FwCommand
{
public:
	FwCmdWait(uint16_t waitTime) : FwCommand(new FwReqWait(waitTime), new SimpleResponse(WAIT)) {};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		((FwReqWait*)(this->GetRequest()))->setTimeValue(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ((FwReqWait*)(this->GetRequest()))->getTimeValue();
	};
};

class FwCmdClearScript : public FwCommand
{
public:
	FwCmdClearScript(void) : FwCommand(new FwReqClearScript(), new SimpleResponse(CLEAR_SCRIPT)) {};
};

class FwCmdGetRtc : public FwCommand
{
public:
	FwCmdGetRtc(void) : FwCommand(new FwReqGetRtc(), new SimpleResponse(GET_RTC)) {};
};

class FwCmdGetTracebuffer : public FwCommand
{
public:
	FwCmdGetTracebuffer(void) : FwCommand(new FwReqGetTracebuffer(), new SimpleResponse(GET_TRACE)) {};
};

class FwCmdGetVersion : public FwCommand
{
public:
	FwCmdGetVersion(void) : FwCommand(new FwReqGetVersion(), new SimpleResponse(GET_FW_VERSION)) {};
};

class FwCmdLoopOff : public FwCommand
{
public:
	FwCmdLoopOff(uint8_t numLoops = 0) : FwCommand(new FwReqLoopOff(numLoops), new SimpleResponse(LOOP_OFF)) {};
};

class FwCmdLoopOn : public FwCommand
{
public:
	FwCmdLoopOn(void) : FwCommand(new FwReqLoopOn(), new SimpleResponse(LOOP_ON)) {};
};

class FwCmdSetColorDirect : public FwCommand
{
public:
	FwCmdSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) : FwCommand(new FwReqSetColorDirect(pBuffer, bufferLength), new SimpleResponse(SET_COLOR_DIRECT)) {};
};

class FwCmdSetFade : public FwCommand
{
public:
	FwCmdSetFade(uint32_t argb, uint16_t fadeTime = 0, uint32_t addr = 0xffffffff, bool parallelFade = false) : FwCommand(new FwReqSetFade(argb, fadeTime, addr, parallelFade), new SimpleResponse(SET_FADE)) {};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		((FwReqSetFade*)(this->GetRequest()))->setTimeValue(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ((FwCmdSetFade*)(this->GetRequest()))->getTimeValue();
	};
};

class FwCmdSetGradient : public FwCommand
{
public:
	FwCmdSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime = 0, bool parallelFade = false, uint8_t length = NUM_OF_LED, uint8_t offset = 0) : FwCommand(new FwReqSetGradient(argb_1, argb_2, fadeTime,  parallelFade, length, offset), new SimpleResponse(SET_GRADIENT)) {};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		((FwReqSetGradient*)(this->GetRequest()))->setTimeValue(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ((FwReqSetGradient*)(this->GetRequest()))->getTimeValue();
	};
};


class FwCmdSetRtc : public FwCommand
{
public:
	FwCmdSetRtc(const tm& timeValue) : FwCommand(new FwReqSetRtc(timeValue), new SimpleResponse(SET_RTC)) {};
};

class FwCmdStartBl : public FwCommand
{
public:
	FwCmdStartBl(void) : FwCommand(new FwReqStartBl(), new SimpleResponse(START_BL)) {};
};

}

#endif /* defined(__WiflyControlCli__FwCommand__) */
