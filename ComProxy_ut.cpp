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

#include "ComProxy.h"

/**************** includes, classes and functions for wrapping ****************/
#include "ClientSocket.h"
#include "string.h"

#define CRC_SIZE 2

ClientSocket::ClientSocket(const char* pAddr, short port, int style) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}

const BlInfo dummyBlInfo = {0xDE, 0xAD, 0xAF, 0xFE, 0xFF, 0x4, 0x0, 0xB0, 0xB1, 0xE5, 0x00};
const unsigned char dummyBlInfoMasked[] = {BL_STX, BL_STX, 0xDE, 0xAD, 0xAF, 0xFE, 0xFF, BL_DLE, 0x04, 0xB0, 0xB1, 0xE5, 0x00, 0xEA, 0x35, BL_ETX};
const unsigned char exampleBlFlashRead[] = {BL_STX, 0x01, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xAD, 0xB5, 0x04};
const unsigned char dummyBlFlashReadResponseMasked[] = {BL_STX, 0xDE, 0xAD, BL_DLE, BL_DLE, 0xEF, 0xa0, 0x06, BL_ETX};
const unsigned char dummyBlFlashReadResponsePure[] = {0xDE, 0xAD, BL_DLE, 0xEF};
const unsigned char dummyBlFlashCrc16ResponseMasked[] = {BL_STX, 0xDE, 0xAD, BL_DLE, BL_DLE, 0xEF, BL_ETX};
const unsigned char dummyBlFlashCrc16ResponsePure[] = {0xDE, 0xAD, BL_DLE, 0xEF};
const unsigned char dummyBlFlashEraseResponseMasked[] = {BL_STX, 0x03, 0x63, 0x30, BL_ETX};
const unsigned char dummyBlFlashEraseResponsePure[] = {0x03};

unsigned char g_TestSocketRecvBuffer[10240];
size_t g_TestSocketRecvBufferPos = 0;
size_t g_TestSocketRecvBufferSize = 0;
unsigned char g_TestSocketSendBuffer[10240];
size_t g_TestSocketSendBufferSize;
TestSocket::TestSocket(const char* pAddr, short port) : ClientSocket(pAddr, port, 0) {}

/**
 * For each call to Recv() we only return one byte of data to simulate a very
 * fragmented response from pic.
 */
size_t TestSocket::Recv(unsigned char* pBuffer, size_t length, unsigned long timeoutTmms) const
{
	if(g_TestSocketRecvBufferPos < g_TestSocketRecvBufferSize)
	{
		*pBuffer = g_TestSocketRecvBuffer[g_TestSocketRecvBufferPos];
		g_TestSocketRecvBufferPos++;
		return 1;
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
		Trace_String("Reply to SYNC\n");
		g_TestSocketRecvBuffer[0] = BL_STX;
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = 1;
		return length;
	}

	/* prepare response for BlInfoRequest */
	if((frame[1] == 0x00) && (frame[2] == 0) && (frame[3] == 0) && (frame[4] == BL_ETX))
	{
		Trace_String("BlInfoRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlInfoMasked, sizeof(dummyBlInfoMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlInfoMasked);
		return length;
	}

	/* prepare response for BlEepromRequest */
	if((frame[1] == 0x02) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlEepromRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashCrc16ResponseMasked, sizeof(dummyBlFlashCrc16ResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashCrc16ResponseMasked);
		return length;
	}

	/* prepare response for BlFlashCrc16Request */
	if((frame[1] == 0x02) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlFlashCrc16Request\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashCrc16ResponseMasked, sizeof(dummyBlFlashCrc16ResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashCrc16ResponseMasked);
		return length;
	}

	/* prepare response for BlFlashEraseRequest */
	if((frame[1] == 0x03) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlFlashEraseRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashEraseResponseMasked, sizeof(dummyBlFlashEraseResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashEraseResponseMasked);
		return length;
	}

	/* prepare response for BlFlashReadRequest */
	if(0 == memcmp(frame, exampleBlFlashRead, sizeof(exampleBlFlashRead)))
	{
		Trace_String("BlFlashReadRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashReadResponseMasked, sizeof(dummyBlFlashReadResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashReadResponseMasked);
		return length;
	}

	/* prepare response for BlEepromReadRequest*/
	if((frame[1] == 0x05) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlEepromReadRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashReadResponseMasked, sizeof(dummyBlFlashReadResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashReadResponseMasked);
		return length;
	}

	/* there is no response for run app, but we set some marker for verification */
	if((frame[1] == 0x08) && (frame[length-1] == BL_ETX))
	{
		Trace_String("BlRunAppRequest\n");
		g_TestSocketRecvBuffer[0] = 'x';
		g_TestSocketRecvBuffer[1] = 'x';
		g_TestSocketRecvBuffer[2] = 'x';
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = 0;
		return length;
	}

	Trace_String("Unkown BlRequest\n");
	return 0;
}

/******************************* test functions *******************************/
int ut_ComProxy_MaskControlCharacters(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);
	unsigned char sendBuffer[256];
	unsigned char recvBuffer[sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 + 1];

	/* prepare send buffer with test data */
	for(size_t i = 0; i < sizeof(sendBuffer); i++)
	{
		sendBuffer[i] = i;
	}

	/* test MaskControlCharacters() with to small target buffer */
	size_t bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer) / 2);
	CHECK(0 == bytesWritten);

	/* mask control characters for crc in little endian order(bootloader) */
	bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer));
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= bytesWritten);
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 >= bytesWritten);

	/* and unmask everything again */
	bytesWritten = proxy.UnmaskControlCharacters(recvBuffer, bytesWritten, recvBuffer, sizeof(recvBuffer), true);
	CHECK(sizeof(sendBuffer) == bytesWritten);

	/* mask control characters for crc in big endian order(firmware) */
	bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer), false);
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= bytesWritten);
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 >= bytesWritten);

	/* and unmask everything again */
	bytesWritten = proxy.UnmaskControlCharacters(recvBuffer, bytesWritten, recvBuffer, sizeof(recvBuffer), true, false);
	CHECK(sizeof(sendBuffer) == bytesWritten);

	/* mask control characters for crc in big endian order(firmware) */
	bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer), false);
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= bytesWritten);
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 >= bytesWritten);

	/* and unmask everything again in wrong byte order */
	bytesWritten = proxy.UnmaskControlCharacters(recvBuffer, bytesWritten, recvBuffer, sizeof(recvBuffer), true);
	CHECK(0 == bytesWritten);

	/* mask control characters for noCrc test */
	bytesWritten = proxy.MaskControlCharacters(sendBuffer, sizeof(sendBuffer), recvBuffer, sizeof(recvBuffer));
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= bytesWritten);
	CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE*2 >= bytesWritten);

	/* and unmask everything again */
	bytesWritten = proxy.UnmaskControlCharacters(recvBuffer, bytesWritten, recvBuffer, sizeof(recvBuffer), false);
	CHECK(sizeof(sendBuffer) + 2 == bytesWritten);

	for(size_t i = 0; i < 6; i++)
	{
		Trace_Hex(sendBuffer[i]);
		Trace_String("-");
		Trace_Hex(recvBuffer[i]);
		Trace_String("\n");
	}
	CHECK(0 == memcmp(sendBuffer, recvBuffer, 256));
	TestCaseEnd();
}

int ut_ComProxy_BlEepromReadRequest(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);
	unsigned char response[512];

	BlEepromReadRequest request;
	request.SetAddressNumBytes(0xDA7A, sizeof(dummyBlFlashReadResponsePure));
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashReadResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashReadResponsePure, response, bytesReceived));
	TestCaseEnd();
}

int ut_ComProxy_BlEepromWriteRequest(void)
{
	NOT_IMPLEMENTED();
}

int ut_ComProxy_BlFlashCrc16Request(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);
	unsigned char response[512];

	BlFlashCrc16Request request(0xDA7ADA7A, 2);
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashCrc16ResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashCrc16ResponsePure, response, bytesReceived));
	TestCaseEnd();
}

int ut_ComProxy_BlFlashEraseRequest(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);
	unsigned char response[512];

	BlFlashEraseRequest request(0xDA7ADA7A, 2);
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashEraseResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashEraseResponsePure, response, bytesReceived));
	TestCaseEnd();
}

int ut_ComProxy_BlFlashReadRequest(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);
	unsigned char response[512];

	BlFlashReadRequest request;
//	request.SetAddressNumBytes(0xDA7ADA7A, sizeof(dummyBlFlashReadResponsePure));
	request.SetAddressNumBytes(0, 0x40);
	size_t bytesReceived = proxy.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashReadResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashReadResponsePure, response, bytesReceived));
	TestCaseEnd();
}

int ut_ComProxy_BlFlashWriteRequest(void)
{
	NOT_IMPLEMENTED();
}

int ut_ComProxy_BlFuseWriteRequest(void)
{
	NOT_IMPLEMENTED();
}

int ut_ComProxy_BlInfoRequest(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);
	unsigned char response[512];

	BlInfoRequest infoRequest;
	size_t bytesReceived = proxy.Send(infoRequest, response, sizeof(response));
	Trace_String("--->: ");
	Trace_Number(sizeof(BlInfo));
	Trace_String(":");
	Trace_Number(bytesReceived);
	CHECK(sizeof(BlInfo) == bytesReceived);

	CHECK(0 == memcmp(&dummyBlInfo, response, bytesReceived));
	TestCaseEnd();
}

int ut_ComProxy_BlRunAppRequest(void)
{
	TestCaseBegin();
	TestSocket dummySocket(0, 0);
	ComProxy proxy(&dummySocket);

	BlRunAppRequest request;
	size_t bytesReceived = proxy.Send(request, 0, 0);
	CHECK(0 == bytesReceived);
	CHECK('x' == g_TestSocketRecvBuffer[0]);
	CHECK('x' == g_TestSocketRecvBuffer[1]);
	CHECK('x' == g_TestSocketRecvBuffer[2]);
	TestCaseEnd();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_ComProxy_MaskControlCharacters);
	RunTest(true, ut_ComProxy_BlEepromReadRequest);
	RunTest(false, ut_ComProxy_BlEepromWriteRequest);
	RunTest(true, ut_ComProxy_BlFlashCrc16Request);
	RunTest(true, ut_ComProxy_BlFlashEraseRequest);
	RunTest(true, ut_ComProxy_BlFlashReadRequest);
	RunTest(false, ut_ComProxy_BlFlashWriteRequest);
	RunTest(false, ut_ComProxy_BlFuseWriteRequest);

	RunTest(true, ut_ComProxy_BlInfoRequest);
	RunTest(true, ut_ComProxy_BlRunAppRequest);
	UnitTestMainEnd();
}

