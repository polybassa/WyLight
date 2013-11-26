/*
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
#include "ComProxy.h"
#include "MaskBuffer.h"
#include <algorithm>
#include <limits>
#include <time.h>
#include <unistd.h>

/**************** includes, classes and functions for wrapping ****************/
#include "ClientSocket.h"
#include "string.h"

using namespace WyLight;

#define CRC_SIZE 2
static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) : mSock(0), mSockAddr(addr, port) {}
ClientSocket::~ClientSocket(void) {}

const BlInfo dummyBlInfo = {0xDE, 0xAD, 0xAF, 0xFE, 0xFF, 0x4, 0x0, 0xB0, 0xB1, 0xE5, 0x00};
const uint8_t dummyBlInfoMasked[] = {BL_STX, BL_STX, 0xDE, 0xAD, 0xAF, 0xFE, 0xFF, BL_DLE, 0x04, 0xB0, 0xB1, 0xE5, 0x00, 0xEA, 0x35, BL_ETX};
const uint8_t exampleBlFlashRead[] = {BL_STX, 0x01, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0xAD, 0xB5, 0x04};
const uint8_t dummyBlFlashReadResponseMasked[] = {BL_STX, 0xDE, 0xAD, BL_DLE, BL_DLE, 0xEF, 0xa0, 0x06, BL_ETX};
const uint8_t dummyBlFlashReadResponsePure[] = {0xDE, 0xAD, BL_DLE, 0xEF};
const uint8_t dummyBlFlashCrc16ResponseMasked[] = {BL_STX, 0xDE, 0xAD, BL_DLE, BL_DLE, 0xEF, BL_ETX};
const uint8_t dummyBlFlashCrc16ResponsePure[] = {0xDE, 0xAD, BL_DLE, 0xEF};
const uint8_t dummyBlFlashEraseResponseMasked[] = {BL_STX, 0x03, 0x63, 0x30, BL_ETX};
const uint8_t dummyBlFlashEraseResponsePure[] = {0x03};

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
	TraceBuffer(ZONE_INFO, frame, length, "%02x ", "%s: ", __FUNCTION__);

	/* Sync */
	if((sizeof(BL_SYNC) == length) && (0 == memcmp(BL_SYNC, frame, sizeof(BL_SYNC)))) {
		Trace(ZONE_INFO, "Reply to SYNC\n");
		g_TestSocketRecvBuffer[0] = BL_STX;
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = 1;
		return length;
	}

	/* prepare response for BlInfoRequest */
	if((frame[1] == 0x00) && (frame[2] == 0) && (frame[3] == 0) && (frame[4] == BL_ETX)) {
		Trace(ZONE_INFO,"BlInfoRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlInfoMasked, sizeof(dummyBlInfoMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlInfoMasked);
		return length;
	}

	/* prepare response for BlEepromRequest */
	if((frame[1] == 0x02) && (frame[length - 1] == BL_ETX)) {
		Trace(ZONE_INFO,"BlEepromRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashCrc16ResponseMasked, sizeof(dummyBlFlashCrc16ResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashCrc16ResponseMasked);
		return length;
	}

	/* prepare response for BlFlashCrc16Request */
	if((frame[1] == 0x02) && (frame[length - 1] == BL_ETX)) {
		Trace(ZONE_INFO,"BlFlashCrc16Request\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashCrc16ResponseMasked, sizeof(dummyBlFlashCrc16ResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashCrc16ResponseMasked);
		return length;
	}

	/* prepare response for BlFlashEraseRequest */
	if((frame[1] == 0x03) && (frame[length - 1] == BL_ETX)) {
		Trace(ZONE_INFO,"BlFlashEraseRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashEraseResponseMasked, sizeof(dummyBlFlashEraseResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashEraseResponseMasked);
		return length;
	}

	/* prepare response for BlFlashReadRequest */
	if(0 == memcmp(frame, exampleBlFlashRead, sizeof(exampleBlFlashRead))) {
		Trace(ZONE_INFO,"BlFlashReadRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashReadResponseMasked, sizeof(dummyBlFlashReadResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashReadResponseMasked);
		return length;
	}

	/* prepare response for BlEepromReadRequest*/
	if((frame[1] == 0x05) && (frame[length - 1] == BL_ETX)) {
		Trace(ZONE_INFO,"BlEepromReadRequest\n");
		memcpy(g_TestSocketRecvBuffer, dummyBlFlashReadResponseMasked, sizeof(dummyBlFlashReadResponseMasked));
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = sizeof(dummyBlFlashReadResponseMasked);
		return length;
	}

	/* there is no response for run app, but we set some marker for verification */
	if((frame[1] == 0x08) && (frame[length - 1] == BL_ETX)) {
		Trace(ZONE_INFO,"BlRunAppRequest\n");
		g_TestSocketRecvBuffer[0] = 'x';
		g_TestSocketRecvBuffer[1] = 'x';
		g_TestSocketRecvBuffer[2] = 'x';
		g_TestSocketRecvBufferPos = 0;
		g_TestSocketRecvBufferSize = 0;
		return length;
	}

		Trace(ZONE_INFO, "Unkown BlRequest\n");
	memcpy(g_TestSocketSendBuffer + g_TestSocketSendBufferPos, frame, length);
	g_TestSocketSendBufferPos += length;
	return length;
}

/******************************* test functions *******************************/
size_t ut_ComProxy_MaskControlCharacters(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t sendBuffer[256];
	static const uint8_t *const sendBufferEnd = sendBuffer + sizeof(sendBuffer);
	uint8_t recvBuffer[sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE * 2 + 1];

	/* prepare send buffer with test data */
	for(size_t i = 0; i < sizeof(sendBuffer); i++) {
		sendBuffer[i] = i;
	}

	/* test MaskControlCharacters() with to small target buffer */
	MaskBuffer tooSmall {sizeof(recvBuffer) / 2};
	try {
		tooSmall.Mask(sendBuffer, sendBufferEnd);
		CHECK(false);
	} catch(FatalError& e) {
		CHECK(true);
	}

	/* mask control characters for crc in little endian order(bootloader) */
	MaskBuffer littleEndian {sizeof(recvBuffer)};
	littleEndian.Mask(sendBuffer, sendBufferEnd);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= littleEndian.Size() - 1);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE * 2 >= littleEndian.Size() - 1);

	/* and unmask everything again */
	UnmaskBuffer response {sizeof(recvBuffer)};
		CHECK(response.Unmask(littleEndian.Data() + 1, littleEndian.Size() - 1, true, true));
		CHECK(response.Size() == sizeof(sendBuffer));
		CHECK(0 == memcmp(sendBuffer, response.Data(), response.Size()));

	/* mask control characters for crc in big endian order(firmware) */
	MaskBuffer bigEndian {sizeof(recvBuffer)};
	bigEndian.Mask(sendBuffer, sendBufferEnd, false);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= bigEndian.Size() - 1);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE * 2 >= bigEndian.Size() - 1);

	/* and unmask everything again */
	response.Clear();
		CHECK(0 == response.Size());
		CHECK(response.Unmask(bigEndian.Data() + 1, bigEndian.Size() - 1, true, false));
		CHECK(response.Size() == sizeof(sendBuffer));
		CHECK(0 == memcmp(sendBuffer, response.Data(), response.Size()));

	/* mask control characters for crc in big endian order(firmware) */
	MaskBuffer wrongOrder {sizeof(recvBuffer)};
	wrongOrder.Mask(sendBuffer, sendBufferEnd, false);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= wrongOrder.Size() - 1);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE * 2 >= wrongOrder.Size() - 1);

	/* and unmask everything again in wrong byte order */
	response.Clear();
		CHECK(response.Unmask(wrongOrder.Data() + 1, wrongOrder.Size() - 1, true, true));
		CHECK(response.Size() == 0);

	/* mask control characters for noCrc test */
	MaskBuffer noCrc {sizeof(recvBuffer)};
	noCrc.Mask(sendBuffer, sendBufferEnd);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE <= noCrc.Size() - 1);
		CHECK(sizeof(sendBuffer) + BL_CRTL_CHAR_NUM + CRC_SIZE * 2 >= noCrc.Size() - 1);

	/* and unmask everything again */
	response.Clear();
		CHECK(response.Unmask(noCrc.Data() + 1, noCrc.Size() - 1, false, false));
		CHECK(response.Size() == sizeof(sendBuffer) + 2);
		CHECK(0 == memcmp(sendBuffer, response.Data(), 256));
	TestCaseEnd();
}

size_t ut_ComProxy_BlEepromReadRequest(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t response[512];
	memset(response, 0, sizeof(response));

	BlEepromReadRequest request;
	request.SetAddressNumBytes(0xDA7A, sizeof(dummyBlFlashReadResponsePure));
	size_t bytesReceived = testee.Send(request, response, sizeof(response), false);
	CHECK(sizeof(dummyBlFlashReadResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashReadResponsePure, response, sizeof(dummyBlFlashReadResponsePure)));
	TestCaseEnd();
}

size_t ut_ComProxy_BlEepromReadRequestTimeout(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t response[512];
	memset(response, 0, sizeof(response));
	timeval delay = {1, 0};
	SetDelay(delay);

	BlEepromReadRequest request;
	request.SetAddressNumBytes(0xDA7A, sizeof(dummyBlFlashReadResponsePure));
	try {
		testee.Send(request, response, sizeof(response));
		CHECK(false);
	} catch(std::exception& e) {
		CHECK(true);
	}
	TestCaseEnd();
}

size_t ut_ComProxy_BlEepromWriteRequest(void)
{
	NOT_IMPLEMENTED();
}

size_t ut_ComProxy_BlFlashCrc16Request(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t response[512];
	memset(response, 0, sizeof(response));

	BlFlashCrc16Request request(0xDA7ADA7A, 2);
	size_t bytesReceived = testee.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashCrc16ResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashCrc16ResponsePure, response, sizeof(dummyBlFlashCrc16ResponsePure)));
	TestCaseEnd();
}

size_t ut_ComProxy_BlFlashEraseRequest(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t response[512];
	memset(response, 0, sizeof(response));

	BlFlashEraseRequest request(0xDA7ADA7A, 2);
	size_t bytesReceived = testee.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashEraseResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashEraseResponsePure, response, sizeof(dummyBlFlashEraseResponsePure)));
	TestCaseEnd();
}

size_t ut_ComProxy_BlFlashReadRequest(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t response[512];
	memset(response, 0, sizeof(response));

	BlFlashReadRequest request;
	request.SetAddressNumBytes(0, 0x40);
	size_t bytesReceived = testee.Send(request, response, sizeof(response));
	CHECK(sizeof(dummyBlFlashReadResponsePure) == bytesReceived);
	CHECK(0 == memcmp(dummyBlFlashReadResponsePure, response, sizeof(dummyBlFlashReadResponsePure)));


	TestCaseEnd();
}

size_t ut_ComProxy_SyncWithTarget(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);

	size_t bytesReceived = testee.SyncWithTarget();
	CHECK(bytesReceived == BL_IDENT);

	TestCaseEnd();
}


size_t ut_ComProxy_BlFlashWriteRequest(void)
{
	NOT_IMPLEMENTED();
}

size_t ut_ComProxy_BlFuseWriteRequest(void)
{
	NOT_IMPLEMENTED();
}

size_t ut_ComProxy_BlInfoRequest(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);
	uint8_t response[512];
	memset(response, 0, sizeof(response));

	BlInfoRequest infoRequest;
	size_t bytesReceived = testee.Send(infoRequest, response, sizeof(response));
	Trace(ZONE_INFO, "--->: %u:%u\n", sizeof(BlInfo), bytesReceived);
	CHECK(sizeof(BlInfo) == bytesReceived);

	CHECK(0 == memcmp(&dummyBlInfo, response, sizeof(BlInfo)));
	TestCaseEnd();
}

size_t ut_ComProxy_BlRunAppRequest(void)
{
	TestCaseBegin();
	TcpSocket dummySock(0, 0);
	ComProxy testee(dummySock);

	BlRunAppRequest request;
	size_t bytesReceived = testee.Send(request, 0, 0);
	CHECK(0 == bytesReceived);
	CHECK('x' == g_TestSocketRecvBuffer[0]);
	CHECK('x' == g_TestSocketRecvBuffer[1]);
	CHECK('x' == g_TestSocketRecvBuffer[2]);
	TestCaseEnd();
}

int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true,  ut_ComProxy_MaskControlCharacters);
	RunTest(true,  ut_ComProxy_BlEepromReadRequest);
	RunTest(true,  ut_ComProxy_BlEepromReadRequestTimeout);
	RunTest(false, ut_ComProxy_BlEepromWriteRequest);
	RunTest(true,  ut_ComProxy_BlFlashCrc16Request);
	RunTest(true,  ut_ComProxy_BlFlashEraseRequest);
	RunTest(true,  ut_ComProxy_BlFlashReadRequest);
	RunTest(false, ut_ComProxy_BlFlashWriteRequest);
	RunTest(false, ut_ComProxy_BlFuseWriteRequest);
	RunTest(true,  ut_ComProxy_BlInfoRequest);
	RunTest(true,  ut_ComProxy_BlRunAppRequest);
	RunTest(true,  ut_ComProxy_SyncWithTarget);
	UnitTestMainEnd();
}

