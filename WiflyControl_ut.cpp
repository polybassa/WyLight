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

#include "unittest.h"
#include "WiflyControl.h"
#include <string>

// empty wrappers to satisfy the linker
ClientSocket::ClientSocket(uint32_t, uint16_t, int) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const { return 0;}
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const {return 0; }
ComProxy::ComProxy(uint32_t addr, uint16_t port) : mSock (addr, port) {}
int32_t ComProxy::Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync) const {	return -1; }
int32_t ComProxy::Send(cmd_frame const* pFrame, uint8_t* pResponse, size_t responseSize, bool doSync) const {	return -1; }

// wrapper to test WiflyControl
std::list<std::string> g_TestBuffer;

bool TestBufferEquals(const std::string msg)
{
	return false;
}

static bool g_ProxySaved = false;
static bool g_ProxyConnected = false;

bool ComProxy::TelnetClose(bool doSave) const
{
	g_ProxySaved = doSave;
	g_ProxyConnected = false;
	return true;
}

bool ComProxy::TelnetOpen(void) const
{
	return g_ProxyConnected = true;
}

bool ComProxy::TelnetSend(const std::string& message, const std::string& expectedResponse) const
{
	g_TestBuffer.push_back(message);
	return g_ProxyConnected;
}


// Testcases
size_t ut_WiflyControl_ConfSetDefaults(void)
{
	TestCaseBegin();
	WiflyControl testControl(0, 0);

	g_TestBuffer.clear();
	CHECK(testControl.ConfSetDefaults());
	CHECK(!g_ProxyConnected);

	static const std::string commands[] = {
		"set broadcast interval 1\r\n",   // to support fast broadcast recognition
		"set uart baud 115200\r\n",       // PIC uart parameter
		"set uart flow 0\r\n",            // PIC uart parameter
		"set uart mode 0\r\n",            // PIC uart parameter
		"set wlan rate 0\r\n",            // slowest datarate but highest range
	};

	for(size_t i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
	{
		CHECK(0 == commands[i].compare(g_TestBuffer.front()));
		g_TestBuffer.pop_front();
		
	}
	TestCaseEnd();
}

size_t ut_WiflyControl_ConfSetWlanPhrase(void)
{
	NOT_IMPLEMENTED();
}

size_t ut_WiflyControl_ConfSetWlanSsid(void)
{
	NOT_IMPLEMENTED();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_WiflyControl_ConfSetDefaults);
	RunTest(true, ut_WiflyControl_ConfSetWlanPhrase);
	RunTest(true, ut_WiflyControl_ConfSetWlanSsid);
	UnitTestMainEnd();
}

