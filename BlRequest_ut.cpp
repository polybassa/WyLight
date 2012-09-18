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
	if(length <= sizeof(g_TestSocketSendBufferSize))
	{
		memcpy(g_TestSocketSendBuffer, frame, length);
		g_TestSocketSendBufferSize = length;
		return length;
	}
	return 0;
}

/******************************* test functions *******************************/
int ut_BlProxy_Send(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char sendBuffer[BL_MAX_MESSAGE_LENGTH];
	unsigned char recvBuffer[BL_MAX_MESSAGE_LENGTH];

	/* prepare response in recv buffer */
	g_TestSocketRecvBuffer[0] = BL_STX;
	g_TestSocketRecvBufferSize = 1;

	BlReadInfoRequest request;
	
	proxy.Send(request, recvBuffer, sizeof(recvBuffer));

	assert(g_TestSocketSendBufferSize == 1);
	assert(0 == memcmp(g_TestSocketSendBuffer, reinterpret_cast<const unsigned char*>(&request), sizeof(request)));
	
	return 0;
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(ut_BlProxy_Send);
	UnitTestMainEnd();
}

