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


#include <stdio.h>
#include "trace.h"
#include "unittest.h"
#include "WiflyControlNoThrow.h"

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

/***** Wrappers ****/
ClientSocket::ClientSocket(uint32_t, uint16_t, int) throw (FatalError) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) throw (ConnectionLost, FatalError) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const throw (FatalError) { return 0;}
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const {return 0; }
ComProxy::ComProxy(const TcpSocket& sock) : mSock (sock) {}
TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock (sock) {}

WiflyControl::WiflyControl(uint32_t addr, uint16_t port)
: mSock(addr, port), mProxy(mSock), mTelnet(mSock)
{
}

static WiflyError g_ErrorCode;

static void throwExceptions()
{
	switch (g_ErrorCode) {
		case CONNECTION_TIMEOUT:
			throw ConnectionTimeout("");
		case CONNECTION_LOST:
			throw ConnectionLost("", 0, 0);
		case INVALID_PARAMETER:
			throw InvalidParameter("");
		case SCRIPT_FULL:
			throw ScriptBufferFull();
		case FATAL_ERROR:
			throw FatalError("");
		default:
			return;
	}
}

void WiflyControl::BlEnableAutostart(void) const throw(ConnectionTimeout, FatalError){ throwExceptions(); }

void WiflyControl::BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError){throwExceptions(); }

void WiflyControl::BlEraseFlash(void) const throw(ConnectionTimeout, FatalError){throwExceptions(); }

void WiflyControl::BlProgramFlash(const std::string& filename) const throw (ConnectionTimeout, FatalError) {throwExceptions(); }

void WiflyControl::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
	throwExceptions();
	while(numBytes)
	{
		numBytes--;
		out << 0xff;
	}
}


void WiflyControl::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
	throwExceptions();
	while(numBytes)
	{
		numBytes--;
		out << 0xff;
	}
}

void WiflyControl::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
	throwExceptions();
	while(numBytes)
	{
		numBytes--;
		out << 0xff;
	}
}

std::string WiflyControl::BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError) {throwExceptions(); return ""; }

void WiflyControl::BlReadInfo(BlInfo& info) const throw (ConnectionTimeout, FatalError){throwExceptions(); }

void WiflyControl::BlRunApp(void) const throw (ConnectionTimeout, FatalError){throwExceptions(); }

std::string WiflyControl::ConfGetSsid(void) const {return ""; }

bool WiflyControl::ConfSetDefaults(void) const {return true; }

bool WiflyControl::ConfSetWlan(const std::string& phrase, const std::string& ssid) const {return true; }

bool WiflyControl::ConfSetDeviceId(const std::string& name) const {return true; }

bool WiflyControl::ConfRebootWlanModul() const {return true; }

void WiflyControl::FwClearScript(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

std::string WiflyControl::FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); return ""; }

void WiflyControl::FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull){throwExceptions(); }

std::string WiflyControl::FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); return ""; }

std::string WiflyControl::FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); return ""; }

void WiflyControl::FwLoopOff(uint8_t numLoops) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwLoopOn(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwSetFade(const std::string& rgb, uint16_t fadeTime, const std::string& addr, bool parallelFade) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwSetRtc(const tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwSetWait(uint16_t waitTime) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwStartBl(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {throwExceptions(); }

void WiflyControl::FwSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime, bool parallelFade, uint8_t length, uint8_t offset) { throwExceptions(); }

size_t ut_WiflyControlNoThrow_FwFunctions(void)
{
	TestCaseBegin();
	
	WiflyError mError[] = {CONNECTION_TIMEOUT, FATAL_ERROR, SCRIPT_FULL, NO_ERROR};
	
	WiflyControlNoThrow testee(0, 0);
	
	std::string tempStr = "";
	tm tempTime;
	uint8_t buffer;
	
	for( auto e : mError)
	{
		g_ErrorCode = e;
		CHECK(e == testee.FwLoopOn());
		CHECK(e == testee.FwClearScript());
		CHECK(e == testee.FwGetCycletime(tempStr));
		CHECK(e == testee.FwGetRtc(tempTime));
		CHECK(e == testee.FwGetTracebuffer(tempStr));
		CHECK(e == testee.FwGetVersion(tempStr));
		CHECK(e == testee.FwLoopOff(0));
		CHECK(e == testee.FwSetRtc(tempTime));
		CHECK(e == testee.FwSetWait(0));
		CHECK(e == testee.FwStartBl());
		CHECK(e == testee.FwSetFade(0));
		CHECK(e == testee.FwStartBl());
		CHECK(e == testee.FwSetColorDirect(&buffer, sizeof(buffer)));
		CHECK(e == testee.FwSetGradient(0,0));
	}
	
	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_ConfFunctions(void)
{
	TestCaseBegin();
	
	WiflyControlNoThrow testee(0, 0);
	
	std::string tempStr = "";
		
	CHECK(NO_ERROR == testee.ConfGetSsid(tempStr));
	CHECK(NO_ERROR == testee.ConfSetWlan(tempStr, tempStr));
	CHECK(NO_ERROR == testee.ConfSetDefaults());
	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlFunctions(void)
{
	TestCaseBegin();
	
	WiflyError mError[] = {CONNECTION_TIMEOUT, FATAL_ERROR, NO_ERROR};
	
	WiflyControlNoThrow testee(0, 0);
	
	std::string tempStr = "";
	BlInfo info;

	
	for( auto e : mError)
	{
		g_ErrorCode = e;
		CHECK(e == testee.BlRunApp());
		CHECK(e == testee.BlReadInfo(info));
		CHECK(e == testee.BlReadFwVersion(tempStr));
		CHECK(e == testee.BlProgramFlash(tempStr));
		CHECK(e == testee.BlEraseFlash());
		CHECK(e == testee.BlEraseEeprom());
		CHECK(e == testee.BlEnableAutostart());
	}
	
	WiflyError mError2[] = {CONNECTION_TIMEOUT, FATAL_ERROR, NO_ERROR, INVALID_PARAMETER};

	for( auto e : mError2)
	{
		g_ErrorCode = e;
		CHECK(e == testee.BlReadCrcFlash(std::cout, 0, 0));
		CHECK(e == testee.BlReadEeprom(std::cout, 0, 0));
		CHECK(e == testee.BlReadFlash(std::cout, 0, 0));
	}
	
	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlReadFlash(void)
{
	TestCaseBegin();
	
	WiflyControlNoThrow testee(0, 0);
	
	g_ErrorCode = NO_ERROR;
	
	std::stringstream mStream;
	mStream.clear();
	
	size_t length = 1000;
	size_t counter = 0;
	
	testee.BlReadFlash(mStream, 0,length);
	do
	{
		uint8_t byte = 0;
		mStream >> byte;
		if(mStream.eof())break;
		
		CHECK(0xff == byte);
		counter++;
	}while(!mStream.eof());
	CHECK(counter == length);
	
	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlReadCrcFlash(void)
{
	TestCaseBegin();
	
	WiflyControlNoThrow testee(0, 0);
	
	g_ErrorCode = NO_ERROR;
	
	std::stringstream mStream;
	mStream.clear();
	
	size_t length = 1000;
	size_t counter = 0;
	
	testee.BlReadCrcFlash(mStream, 0,length);
	do
	{
		uint8_t byte = 0;
		mStream >> byte;
		if(mStream.eof())break;
		
		CHECK(0xff == byte);
		counter++;
	}while(!mStream.eof());
	CHECK(counter == length);
	
	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlReadEeprom(void)
{
	TestCaseBegin();
	
	WiflyControlNoThrow testee(0, 0);
	
	g_ErrorCode = NO_ERROR;
	
	std::stringstream mStream;
	mStream.clear();
	
	size_t length = 1000;
	size_t counter = 0;
	
	testee.BlReadEeprom(mStream, 0,length);
	do
	{
		uint8_t byte = 0;
		mStream >> byte;
		if(mStream.eof())break;
		
		CHECK(0xff == byte);
		counter++;
	}while(!mStream.eof());
	CHECK(counter == length);
	
	TestCaseEnd();
}



int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_WiflyControlNoThrow_FwFunctions);
	RunTest(true, ut_WiflyControlNoThrow_ConfFunctions);
	RunTest(true, ut_WiflyControlNoThrow_BlFunctions);
	RunTest(false, ut_WiflyControlNoThrow_BlReadFlash);
	RunTest(false, ut_WiflyControlNoThrow_BlReadCrcFlash);
	RunTest(false, ut_WiflyControlNoThrow_BlReadEeprom);
	UnitTestMainEnd();
}