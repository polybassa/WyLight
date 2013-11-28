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

#include "unittest.h"
#include "TelnetProxy.h"
#include <algorithm>
#include <limits>
#include <sstream>
#include <string>
#include <time.h>
#include <unistd.h>

/**************** includes, classes and functions for wrapping ****************/

using namespace WyLight;

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;
static const std::string AOK_STRING(AOK);

ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) : mSock(0), mSockAddr(addr, port) {}
ClientSocket::~ClientSocket(void) {}

uint8_t g_TestSocketRecvBuffer[10240];
size_t g_TestSocketRecvBufferPos = 0;
size_t g_TestSocketRecvBufferSize = 0;
uint8_t g_TestSocketSendBuffer[10240];
size_t g_TestSocketSendBufferPos = 0;
timespec g_TestSocketSendDelay;

void SetDelay(timeval& delay)
{
	g_TestSocketSendDelay.tv_sec = delay.tv_sec;
	g_TestSocketSendDelay.tv_nsec = delay.tv_usec * 1000;
}

TcpSocket::TcpSocket(uint32_t addr, uint16_t port) throw (ConnectionLost, FatalError) : ClientSocket(addr, port, 0)
{
	g_TestSocketSendDelay.tv_sec = 0;
	g_TestSocketSendDelay.tv_nsec = 0;
}

/**
 * For each call to Recv() we only return one byte of data to simulate a very
 * fragmented response from pic.
 */
size_t TcpSocket::Recv(uint8_t *pBuffer, size_t length, timeval *timeout) const throw (FatalError)
{
	nanosleep(&g_TestSocketSendDelay, NULL);
	Trace(ZONE_VERBOSE, "%p %u of %u wait for %u\n", pBuffer, g_TestSocketRecvBufferPos, g_TestSocketRecvBufferSize, length);
	if(g_TestSocketRecvBufferPos < g_TestSocketRecvBufferSize) {
		*pBuffer = g_TestSocketRecvBuffer[g_TestSocketRecvBufferPos];
		g_TestSocketRecvBufferPos++;
		return 1;
	}
	return 0;
}

size_t TcpSocket::Send(const uint8_t *frame, size_t length) const
{
	TraceBuffer(ZONE_INFO, frame, length, "%02x ", "%s:", __FUNCTION__);
		memcpy(g_TestSocketSendBuffer + g_TestSocketSendBufferPos, frame, length);
	g_TestSocketSendBufferPos += length;

	// prepare response for entering telnet mode
	if((frame[0] == '$') && (frame[1] == '$') && (frame[2] == '$')) {
		static const std::string RESPONSE("CMD\r\n\r\n" PROMPT);
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = RESPONSE.size();
		memcpy(g_TestSocketRecvBuffer, RESPONSE.data(), g_TestSocketRecvBufferSize);
		return length;
	}

	// echo back
		memcpy(g_TestSocketRecvBuffer + g_TestSocketRecvBufferSize, frame, length);
	g_TestSocketRecvBufferSize += length;

	// echo back CRLF
		memcpy(g_TestSocketRecvBuffer + g_TestSocketRecvBufferSize, "\r\n", 2);
	g_TestSocketRecvBufferSize += 2;

	// append AOK
		memcpy(g_TestSocketRecvBuffer + g_TestSocketRecvBufferSize, AOK_STRING.data(), AOK_STRING.size());
	g_TestSocketRecvBufferSize += AOK_STRING.size();
	return length;
}

size_t ut_TelnetProxy_CloseAndSave(void)
{
	static const std::string CLOSE_CMD("save\r\nexit\r\n");
	static const std::string RESPONSE("save\r\n\r\nStoring in config\r\n<4.00> exit\r\n\r\nEXIT\r\n");
	TestCaseBegin();
	TcpSocket sock {0, 0};
	TelnetProxy testee {sock};

	//test with save
	g_TestSocketRecvBufferPos = 0;
	g_TestSocketRecvBufferSize = RESPONSE.size();
	memcpy(g_TestSocketRecvBuffer, RESPONSE.data(), g_TestSocketRecvBufferSize);
	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(testee.Close(true));
	CHECK(CLOSE_CMD.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, CLOSE_CMD.data(), CLOSE_CMD.size()));
	TestCaseEnd();
}

size_t ut_TelnetProxy_CloseWithoutSave(void)
{
	static const std::string CLOSE_CMD("exit\r\n");
	static const std::string RESPONSE("exit\r\n\r\nEXIT\r\n");
	TestCaseBegin();
	TcpSocket sock {0, 0};
	TelnetProxy testee {sock};

	//test with save
	g_TestSocketRecvBufferPos = 0;
	g_TestSocketRecvBufferSize = RESPONSE.size();
	memcpy(g_TestSocketRecvBuffer, RESPONSE.data(), g_TestSocketRecvBufferSize);
	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(testee.Close(false));
	CHECK(CLOSE_CMD.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, CLOSE_CMD.data(), CLOSE_CMD.size()));
	TestCaseEnd();
}

size_t ut_TelnetProxy_Open(void)
{
	static const std::string OPEN_CMD("$$$");
	TestCaseBegin();
	TcpSocket sock {0, 0};
	TelnetProxy testee {sock};

	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(testee.Open());
	CHECK(OPEN_CMD.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, OPEN_CMD.data(), OPEN_CMD.size()));
	TestCaseEnd();
}

namespace WyLight {
	size_t ut_TelnetProxy_Recv(void)
	{
		TestCaseBegin();
		TcpSocket sock {0, 0};
		TelnetProxy testee {sock};
		// test empty recv
		CHECK(testee.Recv(""));

		// test recv something
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = 4;
		memcpy(g_TestSocketRecvBuffer, "Test", g_TestSocketRecvBufferSize);
		CHECK(testee.Recv("Test"));
		TestCaseEnd();
	}

	size_t ut_TelnetProxy_RecvString(void)
	{
		static const std::string fullResponse("ssid not this ssid: this is my ssid!\r\nssid: XX" PROMPT);
		TestCaseBegin();
		TcpSocket sock {0, 0};
		TelnetProxy testee {sock};
		std::string response;

		// test empty key -> should receive the first line
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = fullResponse.size();
		memcpy(g_TestSocketRecvBuffer, fullResponse.data(), fullResponse.size());
		response.clear();

		CHECK(testee.RecvString("", response));
		CHECK(0 == response.compare("ssid not this ssid: this is my ssid!"));

		// test recv something
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = fullResponse.size();
		memcpy(g_TestSocketRecvBuffer, fullResponse.data(), fullResponse.size());
		response.clear();
		CHECK(testee.RecvString("ssid: ", response));
		CHECK(0 == response.compare("this is my ssid!"));
		TestCaseEnd();
	}
}

size_t ut_TelnetProxy_Send(void)
{
	const std::string cmd("FOO\r\n");
	TestCaseBegin();
	TcpSocket sock {0, 0};
	TelnetProxy testee {sock};
	// test wrong echo
	g_TestSocketRecvBufferPos = 0;
	g_TestSocketRecvBufferSize = 19;
	memcpy(g_TestSocketRecvBuffer, "BAR\r\n\r\nAOK" PROMPT, g_TestSocketRecvBufferSize);
	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(!testee.Send(cmd));
	CHECK(cmd.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, cmd.data(), cmd.size()));

	// test wrong implicit AOK response
	g_TestSocketRecvBufferPos = 0;
	memcpy(g_TestSocketRecvBuffer, "FOO\r\n\r\nERR" PROMPT, g_TestSocketRecvBufferSize);
	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(!testee.Send(cmd));
	CHECK(cmd.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, cmd.data(), cmd.size()));

	// test implicit AOK response
	g_TestSocketRecvBufferPos = 0;
	memcpy(g_TestSocketRecvBuffer, "FOO\r\n\r\nAOK" PROMPT, g_TestSocketRecvBufferSize);
	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(testee.Send(cmd));
	CHECK(cmd.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, cmd.data(), cmd.size()));

	// test good
	g_TestSocketRecvBufferPos = 0;
	memcpy(g_TestSocketRecvBuffer, "FOO\r\n\r\nBAR" PROMPT, g_TestSocketRecvBufferSize);
	g_TestSocketSendBufferPos = 0;
	memset(g_TestSocketSendBuffer, 0, sizeof(g_TestSocketSendBuffer));
	CHECK(testee.Send(cmd, "BAR" PROMPT));
	CHECK(cmd.size() == g_TestSocketSendBufferPos);
	CHECK(0 == memcmp(g_TestSocketSendBuffer, cmd.data(), cmd.size()));
	TestCaseEnd();
}
#include <iostream>
size_t ut_TelnetProxy_SendString(void)
{
	TestCaseBegin();
	static const std::string CRLF("\r\n");
	static const std::string cmd("HUHU");
	static const std::string cmdSetOptReplace("set opt replace 0x");
	std::string value;
	for(char c = std::numeric_limits<char>::max(); c > ' '; c--) {
		if(isprint(c) && !isalnum(c)) {
			value.append(1, c);
		}
	}

	TcpSocket sock {0, 0};
	TelnetProxy testee {sock};

	// test with no replacement char available
	g_TestSocketRecvBufferPos = 0;
	g_TestSocketRecvBufferSize = 0;
	g_TestSocketSendBufferPos = 0;
	CHECK(!testee.SendString(cmd, value));
	CHECK(0 == g_TestSocketSendBufferPos);

	// test with one replacement available
	char replacement = ' ';
	do {
		std::swap(replacement, value.back());
		std::stringstream hexStringReplacement;
		hexStringReplacement << std::hex << (int)replacement;
		const std::string hexReplacement(hexStringReplacement.str());
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = 0;
		g_TestSocketSendBufferPos = 0;
		std::string replacedValue;
		replacedValue.resize(value.size());
		std::replace_copy_if(value.begin(), value.end(), replacedValue.begin(), isblank, replacement);
		CHECK(testee.SendString(cmd, value));
		CHECK(cmdSetOptReplace.size() + hexReplacement.length() + CRLF.size() + cmd.size() + replacedValue.size() + CRLF.size() + cmdSetOptReplace.size() + hexReplacement.length() + CRLF.size() == g_TestSocketSendBufferPos);
		const uint8_t *pPos = g_TestSocketSendBuffer;
		CHECK_MEMCMP(pPos, cmdSetOptReplace.data(), cmdSetOptReplace.size());
		CHECK_MEMCMP(pPos, hexReplacement.data(),   hexReplacement.length());
		CHECK_MEMCMP(pPos, CRLF.data(),             CRLF.size());
		CHECK_MEMCMP(pPos, cmd.data(),              cmd.size());
		CHECK_MEMCMP(pPos, replacedValue.data(),    replacedValue.size());
		std::rotate(value.begin(), value.begin() + 1, value.end());
	}
	while(value.back() != ' ');
	TestCaseEnd();
}

int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_TelnetProxy_CloseAndSave);
	RunTest(true, ut_TelnetProxy_CloseWithoutSave);
	RunTest(true, ut_TelnetProxy_Open);
	RunTest(true, ut_TelnetProxy_Recv);
	RunTest(true, ut_TelnetProxy_RecvString);
	RunTest(true, ut_TelnetProxy_Send);
	RunTest(true, ut_TelnetProxy_SendString);
	UnitTestMainEnd();
}

