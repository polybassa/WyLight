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

#define CRC_SIZE 2

ClientSocket::ClientSocket(const char* pAddr, short port, int style) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}

BlInfo dummyBlInfo = {0xDE, 0xAD, 0xAF, 0xFE, 0xFF, 0x4, 0x0, 0xB0, 0xB1, 0xE5, 0x00};
unsigned char dummyBlFlashReadResponseMasked[] = {BL_STX, 0xDE, 0xAD, BL_DLE, BL_DLE, 0xEF, 0xa0, 0x06, BL_ETX};
unsigned char dummyBlFlashReadResponsePure[] = {0xDE, 0xAD, BL_DLE, 0xEF};
unsigned char dummyBlFlashCrc16ResponseMasked[] = {BL_STX, 0xDE, 0xAD, BL_DLE, BL_DLE, 0xEF, BL_ETX};
unsigned char dummyBlFlashCrc16ResponsePure[] = {0xDE, 0xAD, BL_DLE, 0xEF};
unsigned char dummyBlFlashEraseResponseMasked[] = {BL_STX, 0x03, 0xCA, 0xCA, BL_ETX};
unsigned char dummyBlFlashEraseResponsePure[] = {0x03, 0xCA, 0xCA};

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
		g_TestSocketRecvBuffer[0] = BL_STX;
		g_TestSocketRecvBufferSize = 1;
		return length;
	}

	/* prepare response for BlInfoRequest */
	if((frame[0] == 0x00) && (frame[1] == 0) && (frame[2] == 0) && (frame[3] == BL_ETX))
	{
		size_t t = 0;
		Trace_String("BlInfoRequest\n");
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
		g_TestSocketRecvBuffer[t++] = BL_ETX; // end of transmission;
		g_TestSocketRecvBufferSize = t;
		return length;
	}

	/* prepare response for BlEepromRequest */
	if((frame[0] == 0x02) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlEepromRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashCrc16ResponseMasked, sizeof(dummyBlFlashCrc16ResponseMasked));
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashCrc16ResponseMasked);
		return length;
	}

	/* prepare response for BlFlashCrc16Request */
	if((frame[0] == 0x02) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlFlashCrc16Request\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashCrc16ResponseMasked, sizeof(dummyBlFlashCrc16ResponseMasked));
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashCrc16ResponseMasked);
		return length;
	}

	/* prepare response for BlFlashEraseRequest */
	if((frame[0] == 0x03) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlFlashEraseRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashEraseResponseMasked, sizeof(dummyBlFlashEraseResponseMasked));
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashEraseResponseMasked);
		return length;
	}

	/* prepare response for BlEepromReadRequest or BlFlashReadRequest */
	if((frame[0] == 0x01 || frame[0] == 0x05) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlFlashReadRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashReadResponseMasked, sizeof(dummyBlFlashReadResponseMasked));
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashReadResponseMasked);
		return length;
	}

	/* there is no response for run app, but we set some marker for verification */
	if((frame[0] == 0x08) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlRunAppRequest\n");
		g_TestSocketRecvBuffer[0] = 'x';
		g_TestSocketRecvBuffer[1] = 'x';
		g_TestSocketRecvBuffer[2] = 'x';
		g_TestSocketRecvBufferSize = 0;
		return length;
	}

	Trace_String("Unkown BlRequest\n");
	return 0;
}

/******************************* test functions *******************************/
int ut_BlProxy_MaskControlCharacters(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char sendBuffer[256];
	unsigned char recvBuffer[sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 + 1];

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
	assert(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= bytesWritten);
	assert(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 >= bytesWritten);

	/* and unmask everything again */
	bytesWritten = proxy.UnmaskControlCharacters(recvBuffer, bytesWritten, recvBuffer, sizeof(recvBuffer));
	assert(sizeof(sendBuffer) == bytesWritten);

	for(size_t i = 0; i < 6; i++)
	{
		Trace_Hex(sendBuffer[i]);
		Trace_String("-");
		Trace_Hex(recvBuffer[i]);
		Trace_String("\n");
	}

	assert(0 == memcmp(sendBuffer, recvBuffer, 256));
	
	return 0;
}

int ut_BlProxy_BlEepromReadRequest(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char response[512];

	BlEepromReadRequest request(0xDA7A, sizeof(dummyBlFlashReadResponsePure));
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	assert(sizeof(dummyBlFlashReadResponsePure) == bytesReceived);
	assert(0 == memcmp(dummyBlFlashReadResponsePure, response, bytesReceived));
	return 0;
}

int ut_BlProxy_BlEepromWriteRequest(void)
{
	Trace_String(__FUNCTION__);
	Trace_String(" not implemented\n");
	return 1;
}

int ut_BlProxy_BlFlashCrc16Request(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char response[512];

	BlFlashCrc16Request request(0xDA7ADA7A, 2);
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	assert(sizeof(dummyBlFlashCrc16ResponsePure) == bytesReceived);
	assert(0 == memcmp(dummyBlFlashCrc16ResponsePure, response, bytesReceived));
	return 0;
}

int ut_BlProxy_BlFlashEraseRequest(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char response[512];

	BlFlashEraseRequest request(0xDA7ADA7A, 2);
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	assert(sizeof(dummyBlFlashEraseResponsePure) == bytesReceived);
	assert(0 == memcmp(dummyBlFlashEraseResponsePure, response, bytesReceived));
	return 0;
}

int ut_BlProxy_BlFlashReadRequest(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char response[512];

	BlFlashReadRequest request(0xDA7ADA7A, sizeof(dummyBlFlashReadResponsePure));
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	assert(sizeof(dummyBlFlashReadResponsePure) == bytesReceived);
	assert(0 == memcmp(dummyBlFlashReadResponsePure, response, bytesReceived));
	return 0;
}

int ut_BlProxy_BlFlashWriteRequest(void)
{
	Trace_String(__FUNCTION__);
	Trace_String(" not implemented\n");
	return 1;
}

int ut_BlProxy_BlFuseWriteRequest(void)
{
	Trace_String(__FUNCTION__);
	Trace_String(" not implemented\n");
	return 1;
}

int ut_BlProxy_BlInfoRequest(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);
	unsigned char response[512];

	BlInfoRequest infoRequest;
	size_t bytesReceived = proxy.Send(infoRequest, response, sizeof(response));
	assert(sizeof(BlInfo) == bytesReceived);
	assert(0 == memcmp(&dummyBlInfo, response, bytesReceived));
	return 0;
}

int ut_BlProxy_BlRunAppRequest(void)
{
	TestSocket dummySocket(0, 0);
	BlProxy proxy(&dummySocket);

	BlRunAppRequest request;
	size_t bytesReceived = proxy.Send(request, 0, 0);
	assert(0 == bytesReceived);
	assert('x' == g_TestSocketRecvBuffer[0]);
	assert('x' == g_TestSocketRecvBuffer[1]);
	assert('x' == g_TestSocketRecvBuffer[2]);
	return 0;
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(false, ut_BlProxy_MaskControlCharacters);
	RunTest(false, ut_BlProxy_BlEepromReadRequest);
	RunTest(false, ut_BlProxy_BlEepromWriteRequest);
	RunTest(false, ut_BlProxy_BlFlashCrc16Request);
	RunTest(false, ut_BlProxy_BlFlashEraseRequest);
	RunTest(false, ut_BlProxy_BlFlashReadRequest);
	RunTest(false, ut_BlProxy_BlFlashWriteRequest);
	RunTest(false, ut_BlProxy_BlFuseWriteRequest);

	RunTest(false, ut_BlProxy_BlInfoRequest);
	RunTest(true, ut_BlProxy_BlRunAppRequest);
	UnitTestMainEnd();
}

