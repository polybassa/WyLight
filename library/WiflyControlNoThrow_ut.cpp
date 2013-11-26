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

using namespace WyLight;

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

/***** Wrappers ****/
ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) : mSock(0), mSockAddr(addr, port) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) throw (ConnectionLost, FatalError) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t *pBuffer, size_t length, timeval *timeout) const throw (FatalError) {
	return 0;
}
size_t TcpSocket::Send(const uint8_t *frame, size_t length) const {
	return 0;
}
ComProxy::ComProxy(const TcpSocket& sock) : mSock (sock) {}
TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock (sock) {}
UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast) throw (FatalError) : ClientSocket(addr, port, SOCK_DGRAM) {}
size_t UdpSocket::Send(const uint8_t *frame, size_t length) const {
	return length;
}


Control::Control(uint32_t addr, uint16_t port)
	: mTcpSock(addr, port), mUdpSock(addr, port, false, 0), mProxy(mTcpSock), mTelnet(mTcpSock)
{}

static WiflyError g_ErrorCode;

static void throwExceptions()
{
	switch(g_ErrorCode) {
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



const size_t FwCmdScript::INDENTATION_MAX;
const char FwCmdScript::INDENTATION_CHARACTER;

const std::string FwCmdSetFade::TOKEN("fade");
const std::string FwCmdSetGradient::TOKEN("gradient");
const std::string FwCmdLoopOn::TOKEN("loop");
const std::string FwCmdLoopOff::TOKEN("loop_off");
const std::string FwCmdWait::TOKEN("wait");

void Control::BlEnableAutostart(void) const throw(ConnectionTimeout, FatalError) {
	throwExceptions();
}

void Control::BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError) {
	throwExceptions();
}

void Control::BlEraseFlash(void) const throw(ConnectionTimeout, FatalError) {
	throwExceptions();
}

void Control::BlProgramFlash(const std::string& filename) const throw (ConnectionTimeout, FatalError) {
	throwExceptions();
}

void Control::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
	throwExceptions();
	out.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
	while(numBytes)
	{
		numBytes--;
		out.put((char)0xff);
	}
}


void Control::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
	throwExceptions();
	out.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
	while(numBytes)
	{
		numBytes--;
		out.put((char)0xff);
	}
}

void Control::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
	throwExceptions();
	out.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
	while(numBytes)
	{
		numBytes--;
		out.put((char)0xff);
	}
}

uint16_t Control::BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError) {
	throwExceptions(); return 0;
}

void Control::BlReadInfo(BlInfo& info) const throw (ConnectionTimeout, FatalError) {
	throwExceptions();
}

void Control::BlRunApp(void) const throw (ConnectionTimeout, FatalError) {
	throwExceptions();
}

std::string Control::ConfGetSsid(void) const {
	return "";
}

bool Control::ConfSetDefaults(void) const {
	return true;
}

bool Control::ConfSetWlan(const std::string& phrase, const std::string& ssid) const {
	return true;
}

bool Control::ConfChangeWlanChannel(void) const {
	return true;
}

bool Control::ConfSetDeviceId(const std::string& name) const {
	return true;
}

bool Control::ConfRebootWlanModule() const {
	return true;
}

bool Control::ConfModuleAsSoftAP(const std::string& accesspointName) const {
	return true;
}

bool Control::ConfModuleForWlan(const std::string &phrase, const std::string &ssid, const std::string& name) const {
	return true;
}

bool Control::ConfSetParameters(std::list<std::string> commands) const {
	return true;
}

std::string Control::FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {
	throwExceptions(); return "";
}

void Control::FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {
	throwExceptions();
}

std::string Control::FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {
	throwExceptions(); return "";
}

uint16_t Control::FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull) {
	throwExceptions(); return 0;
}

uint16_t Control::ExtractFwVersion(const std::string& pFilename) const {
	throwExceptions(); return 0;
}

Control& Control::operator<<(FwCommand&& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
	throwExceptions();
	return *this;
}

size_t ut_WiflyControlNoThrow_FwFunctions(void)
{
	TestCaseBegin();

	WiflyError mError[] = {CONNECTION_TIMEOUT, FATAL_ERROR, SCRIPT_FULL, NO_ERROR};

	ControlNoThrow testee(0, 0);

	std::string tempStr = "";
	uint16_t tempValue;
	tm tempTime;
	std::vector<uint8_t> buffer;

	for(auto e : mError) {
		g_ErrorCode = e;
		CHECK(e == testee.FwLoopOn());
		CHECK(e == testee.FwClearScript());
		CHECK(e == testee.FwGetCycletime(tempStr));
		CHECK(e == testee.FwGetRtc(tempTime));
		CHECK(e == testee.FwGetTracebuffer(tempStr));
		CHECK(e == testee.FwGetVersion(tempValue));
		CHECK(e == testee.FwLoopOff(0));
		CHECK(e == testee.FwSetRtc(tempTime));
		CHECK(e == testee.FwSetWait(0));
		CHECK(e == testee.FwStartBl());
		CHECK(e == testee.FwSetFade(0));
		CHECK(e == testee.FwStartBl());
		CHECK(e == testee.FwSetColorDirect(buffer));
		CHECK(e == testee.FwSetGradient(0,0));
	}

	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_ConfFunctions(void)
{
	TestCaseBegin();

	ControlNoThrow testee(0, 0);

	std::string tempStr = "";

	CHECK(NO_ERROR == testee.ConfGetSsid(tempStr));
	CHECK(NO_ERROR == testee.ConfModuleAsSoftAP(tempStr));
	CHECK(NO_ERROR == testee.ConfModuleForWlan(tempStr,tempStr,tempStr));
	CHECK(NO_ERROR == testee.ConfChangeWlanChannel());
	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlFunctions(void)
{
	TestCaseBegin();

	WiflyError mError[] = {CONNECTION_TIMEOUT, FATAL_ERROR, NO_ERROR};

	ControlNoThrow testee(0, 0);

	std::string tempStr = "";
	BlInfo info;
	uint16_t tempValue;


	for(auto e : mError) {
		g_ErrorCode = e;
		CHECK(e == testee.BlRunApp());
		CHECK(e == testee.BlReadInfo(info));
		CHECK(e == testee.BlReadFwVersion(tempValue));
		CHECK(e == testee.BlProgramFlash(tempStr));
		CHECK(e == testee.BlEraseFlash());
		CHECK(e == testee.BlEraseEeprom());
		CHECK(e == testee.BlEnableAutostart());
	}

	WiflyError mError2[] = {CONNECTION_TIMEOUT, FATAL_ERROR, NO_ERROR, INVALID_PARAMETER};

	for(auto e : mError2) {
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

	ControlNoThrow testee(0, 0);

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
		if(mStream.eof()) break;

		CHECK(0xff == byte);
		counter++;
	}
	while(!mStream.eof());
		CHECK(counter == length);

	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlReadCrcFlash(void)
{
	TestCaseBegin();

	ControlNoThrow testee(0, 0);

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
		if(mStream.eof()) break;

		CHECK(0xff == byte);
		counter++;
	}
	while(!mStream.eof());
		CHECK(counter == length);

	TestCaseEnd();
}

size_t ut_WiflyControlNoThrow_BlReadEeprom(void)
{
	TestCaseBegin();

	ControlNoThrow testee(0, 0);

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
		if(mStream.eof()) break;

		CHECK(0xff == byte);
		counter++;
	}
	while(!mStream.eof());
		CHECK(counter == length);

	TestCaseEnd();
}



int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_WiflyControlNoThrow_FwFunctions);
	RunTest(true, ut_WiflyControlNoThrow_ConfFunctions);
	RunTest(true, ut_WiflyControlNoThrow_BlFunctions);
	RunTest(true, ut_WiflyControlNoThrow_BlReadFlash);
	RunTest(true, ut_WiflyControlNoThrow_BlReadCrcFlash);
	RunTest(true, ut_WiflyControlNoThrow_BlReadEeprom);
	UnitTestMainEnd();
}
