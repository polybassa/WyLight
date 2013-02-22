/**
 Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.
 
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

#ifndef _WiflyControlResponse_h
#define _WiflyControlResponse_h

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "wifly_cmd.h"

class WiflyResponse
{
public:
	virtual void Init(response_frame* pData, size_t dataLength) = 0;
	bool IsValid(void) const { return mIsValid; };
	bool IsScriptBufferFull(void) const { return mIsScriptBufferFull; };
	
protected:
	WiflyResponse(void) : mIsValid(false), mIsScriptBufferFull(true) {};
	bool mIsValid;
	bool mIsScriptBufferFull;
};

class SimpleResponse : public WiflyResponse
{
public:
	SimpleResponse(uint8_t cmd) : mCmd(cmd) {};
	void Init(response_frame* pData, size_t dataLength)
	{
		mIsValid = (NULL != pData) && (4 <= dataLength) && (mCmd == pData->cmd);
		mIsScriptBufferFull = (pData->state == SCRIPTBUFFER_FULL);
	};
	
private:
	const uint8_t mCmd;
};

class RtcResponse : public SimpleResponse
{
public:
	RtcResponse(void) : SimpleResponse(GET_RTC) {};
	void Init(response_frame* pData, size_t dataLength)
	{
		SimpleResponse::Init(pData, dataLength);
		if(mIsValid && (dataLength >= 4 + sizeof(struct rtc_time)))
		{
			mTimeValue.tm_sec = pData->data.get_rtc.tm_sec;
			mTimeValue.tm_min = pData->data.get_rtc.tm_min;
			mTimeValue.tm_hour = pData->data.get_rtc.tm_hour;
			mTimeValue.tm_mday = pData->data.get_rtc.tm_mday;
			mTimeValue.tm_year = pData->data.get_rtc.tm_year;
			mTimeValue.tm_wday = pData->data.get_rtc.tm_wday;
			mTimeValue.tm_mon = pData->data.get_rtc.tm_mon;
		}
	};
	struct tm GetRealTime(void) const {return mTimeValue; };
	
private:
	struct tm mTimeValue;
};

class CycletimeResponse : public SimpleResponse
{
public:
	CycletimeResponse(void) : SimpleResponse(GET_CYCLETIME) {};
	void Init(response_frame* pData, size_t dataLength)
	{
		SimpleResponse::Init(pData, dataLength);
		if(mIsValid && (dataLength >= 4 + sizeof(uns16) * CYCLETIME_METHODE_ENUM_SIZE))
		{
			for (unsigned int i = 0; i < CYCLETIME_METHODE_ENUM_SIZE && i < dataLength / sizeof(uns16); i++)
			{
				mCycletimes[i] = ntohs(pData->data.get_max_cycle_times[i]);
			}
		}
	};
	void PrintCycletimes(std::ostream& out)
	{
		if(mIsValid)
		{
			out << "Cycletimes: " << endl;
			for( unsigned int i = 0; i < CYCLETIME_METHODE_ENUM_SIZE; i++)
			{
				out << std::setw(3) << std::dec << i + 1 << ": " << std::setw(8) << std::dec << mCycletimes[i] << " us" << endl;
			}

		}
	};

private:
	uns16 mCycletimes[CYCLETIME_METHODE_ENUM_SIZE];
};

class TracebufferResponse : public SimpleResponse
{
public:
	TracebufferResponse(void) : SimpleResponse(GET_TRACE), mMessageLength(0) {};
	void Init(response_frame* pData, size_t dataLength)
	{
		SimpleResponse::Init(pData, dataLength);
		if(mIsValid)
		{
			mMessageLength = dataLength - 4;
			for (unsigned int i = 0; i < dataLength; i++)
			{
				mTracebuffer[i] = (char) pData->data.get_trace_string[i];
			}
		}
	};
	void PrintTracebuffer(std::ostream& out)
	{
		if(mIsValid)
		{
			out << endl << "Tracebuffercontent: ";
			for(unsigned int i = 0; i < mMessageLength; i++) out << mTracebuffer[i];
			out << endl;
		}
	};
	
private:
	char mTracebuffer[RingBufferSize];
	unsigned int mMessageLength;
};

class FirmwareVersionResponse : public SimpleResponse
{
public:
	FirmwareVersionResponse(void) : SimpleResponse(GET_FW_VERSION) {};
	void Init(response_frame* pData, size_t dataLength)
	{
		SimpleResponse::Init(pData, dataLength);
		if(mIsValid && (dataLength >= 4 + sizeof(struct cmd_get_fw_version)))
		{
			mFwVersion.major = pData->data.version.major;
			mFwVersion.minor = pData->data.version.minor;
		}
	};
	void PrintFirmwareVersion(std::ostream& out)
	{
		if(mIsValid)
		{
			out << endl << "Firmwareversion: ";
			out << (int) mFwVersion.major << ".";
			out << (int) mFwVersion.minor;
			out << endl;
		}
	};
	
private:
	cmd_get_fw_version mFwVersion;
};

#endif
