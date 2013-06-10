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

#include "FwRequest.h"
#include "FwResponse.h"

#include <algorithm>
#include <iostream>

namespace WyLight {

class FwCommand {
	FwCommand& operator=( const FwCommand& ) = delete;

protected:
	FwCommand(FwRequest* const req, FwResponse* const resp) : mRequest(req), mResponse(resp) {};
	
	FwRequest* const mRequest;
	FwResponse* const mResponse;

	
public:
	virtual ~FwCommand(void) { };
	FwRequest* const GetRequest(void) const {return mRequest; };
	FwResponse* const GetResponse(void) const {return mResponse; };
};

class FwCmdScript : public FwCommand
{
	SimpleResponse mResponse;
protected:
	FwCmdScript(FwRequest* const req, uint8_t cmd) : FwCommand(req, &mResponse), mResponse(cmd) {};

public:
	virtual bool Equals(const FwCmdScript& ref) const {
		return (typeid(*this) == typeid(ref));
	};
	virtual	std::ostream& Write(std::ostream& out, size_t& indentation) const = 0;
};

class FwCmdWait : public FwCmdScript
{
	FwReqWait mRequest;
public:
	static const std::string TOKEN;
	FwCmdWait(std::istream& is) : FwCmdScript(&mRequest, WAIT), mRequest(is) {};
	FwCmdWait(uint16_t waitTime) : FwCmdScript(&mRequest, WAIT), mRequest(waitTime) {};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		mRequest.setTimeValue(timeValue);
	};
	
	virtual uint16_t getTimeValue(void) const
	{
		return mRequest.getTimeValue();
	};

	bool Equals(const FwCmdScript& ref) const
	{
		if(!FwCmdScript::Equals(ref)) {
			return false;
		}
		auto r = reinterpret_cast<const FwCmdWait&>(ref);
		return getTimeValue() == r.getTimeValue();
	};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return mRequest.Write(out, indentation);
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

class FwCmdLoopOff : public FwCmdScript
{
	FwReqLoopOff mRequest;
public:
	static const std::string TOKEN;
	FwCmdLoopOff(std::istream& is) : FwCmdScript(&mRequest, LOOP_OFF), mRequest(is) {};
	FwCmdLoopOff(uint8_t numLoops = 0) : FwCmdScript(&mRequest, LOOP_OFF), mRequest(numLoops) {};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return mRequest.Write(out, indentation);
	};
};

class FwCmdLoopOn : public FwCmdScript
{
	FwReqLoopOn mRequest;
public:
	static const std::string TOKEN;
	FwCmdLoopOn(void) : FwCmdScript(&mRequest, LOOP_ON) {};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return mRequest.Write(out, indentation);
	};
};

class FwCmdSetColorDirect : public FwCommand
{
public:
	FwCmdSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) : FwCommand(new FwReqSetColorDirect(pBuffer, bufferLength), new SimpleResponse(SET_COLOR_DIRECT)) {};
};

class FwCmdSetFade : public FwCmdScript
{
	FwReqSetFade mRequest;
public:
	static const std::string TOKEN;
	FwCmdSetFade(std::istream& is) : FwCmdScript(&mRequest, SET_FADE), mRequest(is) {};
	FwCmdSetFade(uint32_t argb, uint16_t fadeTime = 0, uint32_t addr = 0xffffffff, bool parallelFade = false) : FwCmdScript(&mRequest, SET_FADE), mRequest(argb, fadeTime, addr, parallelFade) {};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		((FwReqSetFade*)(this->GetRequest()))->setTimeValue(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ((FwCmdSetFade*)(this->GetRequest()))->getTimeValue();
	};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return mRequest.Write(out, indentation);
	};
};

class FwCmdSetGradient : public FwCmdScript
{
	FwReqSetGradient mRequest;
public:
	static const std::string TOKEN;
	FwCmdSetGradient(std::istream& is) : FwCmdScript(&mRequest, SET_FADE), mRequest(is) {};
	FwCmdSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime = 0, bool parallelFade = false, uint8_t length = NUM_OF_LED, uint8_t offset = 0) : FwCmdScript(&mRequest, SET_FADE), mRequest(argb_1, argb_2, fadeTime,  parallelFade, length, offset) {};
	
	virtual void setTimeValue(uint16_t timeValue)
	{
		((FwReqSetGradient*)(this->GetRequest()))->setTimeValue(timeValue);
	};
	
	virtual uint16_t getTimeValue(void)
	{
		return ((FwReqSetGradient*)(this->GetRequest()))->getTimeValue();
	};
	
	std::ostream& Write(std::ostream& out, size_t& indentation) const {
		return mRequest.Write(out, indentation);
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
