/**
 Copyright (C) 2012 Nils Weiss, Patrick Bruenn.
 
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

#include "BlRequest.h"

/**************** includes, classes and functions for wrapping ****************/
#include "ClientSocket.h"
#include "string.h"

ClientSocket::ClientSocket(const char* pAddr, short port, int style) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}

BlInfo dummyBlInfo = {0xDE, 0xAD, 0xAF, 0xFE, 0xFF, 0x4, 0x0, 0xB0, 0xB1, 0xE5, 0x00, 0xCA, 0xCA};

unsigned char g_TestSocketRecvBuffer[10240];
size_t g_TestSocketRecvBufferSize;
unsigned char g_TestSocketSendBuffer[10240];
size_t g_TestSocketSendBufferSize;
TestSocket::TestSocket(const char* pAddr, short port) : ClientSocket(pAddr, port, 0) {}

size_t TestSocket::Recv(unsigned char* pBuffer, size_t length, unsigned long timeoutTmms) const
{
	if(length >= g_TestSocketRecvBufferSize)
	{
		memcpy(pBuffer, g_TestSocketRecvBuffer, length);
		return g_TestSocketRecvBufferSize;
	}
	return 0;
}

int TestSocket::Send(const unsigned char* frame, size_t length) const
{
	Trace_String("Send: ");
	for(size_t i = 0; i < length; i++) Trace_Hex(frame[i]);
	Trace_String("\n");

	/* Sync */
	if((sizeof(BL_SYNC) == length) && (0 == memcmp(BL_SYNC, frame, sizeof(BL_SYNC))))
	{
		Trace_String("BlSync\n");
		g_TestSocketRecvBuffer[0] = BL_STX;
		g_TestSocketRecvBufferSize = 1;
		return length;
	}

	/* BlReadInfoRequest */
	if((frame[0] == 0) && (frame[1] == 0) && (frame[2] == 0) && (frame[3] == BL_ETX))
	{
		size_t t = 0;
		Trace_String("BlReadInfoRequest\n");
		g_TestSocketRecvBuffer[t++] = BL_STX;
		g_TestSocketRecvBuffer[t++] = BL_STX;
		g_TestSocketRecvBuffer[t++] = BL_STX;
		g_TestSocketRecvBuffer[t++] = BL_STX;
		g_TestSocketRecvBuffer[t++] = 0xDE; // sizeLow
		g_TestSocketRecvBuffer[t++] = 0xAD; // sizeHigh
		g_TestSocketRecvBuffer[t++] = 0xAF; // versionMajor
		g_TestSocketRecvBuffer[t++] = 0xFE; // versionMinor
		g_TestSocketRecvBuffer[t++] = 0xFF; // cmdmaskHigh
		g_TestSocketRecvBuffer[t++] = BL_DLE;
		g_TestSocketRecvBuffer[t++] = 0x04; // PIC18
		g_TestSocketRecvBuffer[t++] = 0xB0; // startLow
		g_TestSocketRecvBuffer[t++] = 0xB1; // startHigh
		g_TestSocketRecvBuffer[t++] = 0xE5; // startU
		g_TestSocketRecvBuffer[t++] = 0x00; // zero
		g_TestSocketRecvBuffer[t++] = 0xCA; // crcLow;
		g_TestSocketRecvBuffer[t++] = 0xCA; // crcHigh;
		g_TestSocketRecvBuffer[t++] = BL_ETX; // crcHigh;
		g_TestSocketRecvBufferSize = t;
		return length;
	}
	return -7;
}

/******************************* test functions *******************************/
int ut_BlProxy_MaskControlCharacters(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char sendBuffer[256];
	unsigned char recvBuffer[sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + 1];

	/* prepare send buffer with test data */
	for(size_t i = 0; i < sizeof(sendBuffer); i++)
	{
		sendBuffer[i] = i;
	}

	/* test MaskControlCharacters() with to small target buffer */
	size_t bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer) / 2);
	assert(0 == bytesWritten);

	/* mask control characters */
	bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer));
	assert(256 + BL_CRTL_CHAR_NUM == bytesWritten);

	/* and unmask everything again */
	bytesWritten = proxy.UnmaskControlCharacters(recvBuffer, bytesWritten);
	assert(256 == bytesWritten);
	assert(0 == memcmp(sendBuffer, recvBuffer, 256));
	
	return 0;
}

int ut_BlProxy_Send(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char response[512];

	BlReadInfoRequest request;

	size_t bytesReceived = proxy.Send(request, response, sizeof(response));

	printf("read %d - %d\n", bytesReceived, sizeof(BlInfo));
	assert(sizeof(BlInfo) == bytesReceived);

	const unsigned char* pDummy = reinterpret_cast<const unsigned char*>(&dummyBlInfo);
	for(size_t i = 0; i < bytesReceived; i++)
	{
		Trace_Hex(response[i]);
		Trace_Hex(pDummy[i]);
		Trace_String("\n");
	}
	assert(0 == memcmp(&dummyBlInfo, response, bytesReceived));
	
	
	return 0;
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(ut_BlProxy_MaskControlCharacters);
	RunTest(ut_BlProxy_Send);
	UnitTestMainEnd();
}

