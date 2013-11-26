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
#include "BroadcastReceiver.h"
#include "trace.h"
#include <thread>
#include <sstream>
#include <vector>
#include <iostream>
#include <unistd.h>

using std::vector;
using namespace WyLight;

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static const timespec NANOSLEEP_TIME = {0, 5000000};

uint8_t capturedBroadcastMessage[110] = {
	0x00, 0x0f, 0xb5, 0xb2, 0x57, 0xfa, //MAC
	0x07, //channel
	0x3f, //rssi
	0x07, 0xd0, //port
	0x00, 0x00, 0x24, 0xb1, //rtc
	0x0b, 0xff, //battery
	0x0d, 0x11, //gpio
	0x54, 0x69, 0x6d, 0x65, 0x20, 0x4e, 0x4f, 0x54, 0x20, 0x53, 0x45, 0x54, 0x00, 0x00, //time
	0x57, 0x69, 0x46, 0x6c, 0x79, 0x20, 0x56, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x33, 0x36,
	0x2c, 0x20, 0x30, 0x38, 0x2d, 0x32, 0x32, 0x2d, 0x32, 0x30, 0x31, 0x32, 0x00, 0x00, //version
	'W', 'i', 'F', 'l', 'y', '-', 'E', 'Z', 'X',
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', //deviceid
	0x4e, 0x00, //boottime
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
};

uint8_t capturedBroadcastMessage_2[110] = {
	0xbc, 0x14, 0x01, 0x97, 0xf2, 0xa8, //MAC
	0x07, //channel
	0x3b, //rssi
	0x07, 0xd0, //port
	0x00, 0x00, 0x09, 0x20, //rtc
	0x0b, 0xe7, //battery
	0x0d, 0x11, //gpio
	0x54, 0x69, 0x6d, 0x65, 0x20, 0x4e,   0x4f, 0x54, 0x20, 0x53, 0x45, 0x54, 0x00, 0x00, //time
	0x57, 0x69, 0x46, 0x6c, 0x79, 0x20, 0x56, 0x65, 0x72, 0x20, 0x32, 0x2e, 0x33, 0x36,
	0x2c, 0x20, 0x30, 0x39, 0x2d, 0x32, 0x37, 0x2d, 0x32, 0x30, 0x31, 0x32, 0x00, 0x00, //version
	'W', 'i', 'F', 'l', 'y', '_', 'L', 'i', 'g',
	'h', 't', 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x02, //deviceid
	0x18, 0x00, //boottime
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //sensors
};

static const Ipv4Addr g_FirstRemote(0x7F000001, 0xffff);
static const Ipv4Addr g_SecondRemote(0x7F000002, 0xffff);
static const Ipv4Addr g_ThirdRemote(0x7F000003, 0xffff);

/**************** includes, classes and functions for wrapping ****************/
uint8_t g_TestSocketRecvBuffer[10240];
uint8_t *g_TestSocketRecvBufferPos = g_TestSocketRecvBuffer;
size_t g_TestSocketRecvBufferSize = 0;
const sockaddr_in *g_TestSocketRecvAddr;

void SetTestSocket(const sockaddr_in *addr, size_t offset, void *pData, size_t dataLength)
{
	g_TestSocketRecvAddr = addr;
		memcpy(g_TestSocketRecvBuffer + offset, pData, dataLength);
	g_TestSocketRecvBufferPos = g_TestSocketRecvBuffer;
	g_TestSocketRecvBufferSize = offset + dataLength;
}

ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) : mSock(0), mSockAddr(addr, port) {}
ClientSocket::~ClientSocket(void) {}

UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast) throw (FatalError)
	: ClientSocket(addr, port, SOCK_DGRAM) {}

size_t UdpSocket::RecvFrom(uint8_t *pBuffer, size_t length, timeval *timeout, struct sockaddr *remoteAddr, socklen_t *remoteAddrLength) const throw (FatalError)
{
	const size_t bytesToSend = std::min(g_TestSocketRecvBufferSize, length);
	if(bytesToSend > 0) {
		memcpy(pBuffer,    g_TestSocketRecvBufferPos, bytesToSend);
		memcpy(remoteAddr, g_TestSocketRecvAddr,      sizeof(sockaddr_in));
		*remoteAddrLength = sizeof(sockaddr_in);
		g_TestSocketRecvBufferPos += bytesToSend;
		g_TestSocketRecvBufferSize -= bytesToSend;
	}
	return bytesToSend;
}

size_t UdpSocket::Send(const uint8_t *frame, size_t length) const
{
	return 0;
}

std::ostringstream g_TestOut;
size_t g_LastIndex;
void TestCallback(size_t index, const Endpoint& newEndpoint)
{
	g_LastIndex = index;
	g_TestOut << index << ':' << newEndpoint << '\n';
}


/******************************* test functions *******************************/
int32_t ut_BroadcastReceiver_TestEmpty(void)
{
	TestCaseBegin();
	BroadcastReceiver dummyReceiver;
	CHECK(0 == dummyReceiver.NumRemotes());

	TestCaseEnd();
}

int32_t ut_BroadcastReceiver_TestGetEndpoint(void)
{
	TestCaseBegin();
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	g_TestOut.str("");
	BroadcastReceiver dummyReceiver(BroadcastReceiver::BROADCAST_PORT, "", TestCallback);
	timeval timeout = {2, 0};
	std::thread myThread(std::ref(dummyReceiver), &timeout);
	nanosleep(&NANOSLEEP_TIME, NULL);
	dummyReceiver.Stop();
	myThread.join();

	const Endpoint& byIndex = dummyReceiver.GetEndpoint(0);
	const Endpoint& byFingerprint = dummyReceiver.GetEndpointByFingerprint(byIndex.AsUint64());
	CHECK(byIndex.GetIp() == byFingerprint.GetIp());
	CHECK(byIndex.GetPort() == byFingerprint.GetPort());
	CHECK(byIndex.GetDeviceId() == byFingerprint.GetDeviceId());
	CHECK(byIndex.GetScore() == byFingerprint.GetScore());
	CHECK(byIndex.GetIp() == 0x7F000001);
	CHECK(byIndex.GetPort() == 2000);
	CHECK(0 == byIndex.GetDeviceId().compare("WiFly-EZX12345678901234567890123N"));
	CHECK(byIndex.GetScore() == 0);

	const Endpoint& outOfBound = dummyReceiver.GetEndpoint(1);
	const Endpoint& empty = dummyReceiver.GetEndpointByFingerprint(0);
	CHECK(outOfBound.GetIp() == empty.GetIp());
	CHECK(outOfBound.GetPort() == empty.GetPort());
	CHECK(outOfBound.GetDeviceId() == empty.GetDeviceId());
	CHECK(outOfBound.GetScore() == empty.GetScore());
	CHECK(outOfBound.GetIp() == 0);
	CHECK(outOfBound.GetPort() == 0);
	CHECK(0 == outOfBound.GetDeviceId().compare(""));
	CHECK(outOfBound.GetScore() == 0);

	CHECK(0 == g_TestOut.str().compare("0:0 127.0.0.1:2000  :  WiFly-EZX12345678901234567890123N\n"));
	CHECK(1 == dummyReceiver.NumRemotes());
	TestCaseEnd();
}

int32_t ut_BroadcastReceiver_TestSimple(void)
{
	TestCaseBegin();
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	g_TestOut.str("");
	BroadcastReceiver dummyReceiver(BroadcastReceiver::BROADCAST_PORT, "", TestCallback);
	timeval timeout = {2, 0};
	std::thread myThread(std::ref(dummyReceiver), &timeout);
	dummyReceiver.Stop();
	myThread.join();

	CHECK(0 == g_TestOut.str().compare("0:0 127.0.0.1:2000  :  WiFly-EZX12345678901234567890123N\n"));
	CHECK(1 == dummyReceiver.NumRemotes());
	CHECK(0x7F000001 == dummyReceiver.GetEndpoint(0).GetIp());
	CHECK(2000 == dummyReceiver.GetEndpoint(0).GetPort());
	TestCaseEnd();
}

int32_t ut_BroadcastReceiver_TestTwoSame(void)
{
	TestCaseBegin();
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	g_TestOut.str("");
	BroadcastReceiver dummyReceiver(BroadcastReceiver::BROADCAST_PORT, "", TestCallback);
	timeval timeout = {3, 0};
	std::thread myThread(std::ref(dummyReceiver), &timeout);
	nanosleep(&NANOSLEEP_TIME, NULL);
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	nanosleep(&NANOSLEEP_TIME, NULL);
	dummyReceiver.Stop();
	myThread.join();

	CHECK(0 == g_TestOut.str().compare("0:0 127.0.0.1:2000  :  WiFly-EZX12345678901234567890123N\n"));
	CHECK(1 == dummyReceiver.NumRemotes());
	CHECK(0x7F000001 == dummyReceiver.GetEndpoint(0).GetIp());
	CHECK(2000 == dummyReceiver.GetEndpoint(0).GetPort());
	TestCaseEnd();
}

int32_t ut_BroadcastReceiver_TestNoTimeout(void)
{
	TestCaseBegin();
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	g_TestOut.str("");
	BroadcastReceiver dummyReceiver(BroadcastReceiver::BROADCAST_PORT, "", TestCallback);
	std::thread myThread(std::ref(dummyReceiver));
	nanosleep(&NANOSLEEP_TIME, NULL);
	SetTestSocket(&g_SecondRemote, 0, capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	nanosleep(&NANOSLEEP_TIME, NULL);
	SetTestSocket(&g_ThirdRemote, 0, capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	nanosleep(&NANOSLEEP_TIME, NULL);

	dummyReceiver.Stop();
	myThread.join();

	CHECK(0 == g_TestOut.str().compare("0:0 127.0.0.1:2000  :  WiFly-EZX12345678901234567890123N\n1:0 127.0.0.2:2000  :  WiFly_Light\n2:0 127.0.0.3:2000  :  WiFly_Light\n"));
	CHECK(3 == dummyReceiver.NumRemotes());
	CHECK(0x7F000001 == dummyReceiver.GetEndpoint(0).GetIp());
	CHECK(2000 == dummyReceiver.GetEndpoint(0).GetPort());
	CHECK(0x7F000002 == dummyReceiver.GetEndpoint(1).GetIp());
	CHECK(2000 == dummyReceiver.GetEndpoint(1).GetPort());
	CHECK(0x7F000003 == dummyReceiver.GetEndpoint(2).GetIp());
	CHECK(2000 == dummyReceiver.GetEndpoint(2).GetPort());
	TestCaseEnd();
}

int32_t ut_BroadcastReceiver_TestRecentEndpoints(void)
{
	static const std::string TEST_FILENAME = "TestRecentEndpoints.txt";
	TestCaseBegin();
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	g_TestOut.str("");
	BroadcastReceiver dummyReceiver(BroadcastReceiver::BROADCAST_PORT, "", TestCallback);
	timeval timeout = {2, 0};
	std::thread myThread(std::ref(dummyReceiver), &timeout);


	nanosleep(&NANOSLEEP_TIME, NULL);
	SetTestSocket(&g_SecondRemote, 0, capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	nanosleep(&NANOSLEEP_TIME, NULL);
	SetTestSocket(&g_ThirdRemote, 0, capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	nanosleep(&NANOSLEEP_TIME, NULL);

	dummyReceiver.Stop();
	myThread.join();

	Endpoint& firstScored = dummyReceiver.GetEndpoint(0);
	++(++firstScored);
	Endpoint& secondScored = dummyReceiver.GetEndpoint(2);
	++(++secondScored);

	dummyReceiver.WriteRecentEndpoints(TEST_FILENAME, 2);
	BroadcastReceiver reread;
	reread.ReadRecentEndpoints(TEST_FILENAME);

	CHECK(2 == reread.NumRemotes());
	CHECK(0x7F000001 == reread.GetEndpoint(0).GetIp());
	CHECK(2000 == reread.GetEndpoint(0).GetPort());
	CHECK(0x7F000003 == reread.GetEndpoint(1).GetIp());
	CHECK(2000 == reread.GetEndpoint(1).GetPort());
	TestCaseEnd();
}

int32_t ut_BroadcastReceiver_TestRecentEndpoints2(void)
{
	g_LastIndex = 0xff;
	static const std::string TEST_FILENAME = "TestRecentEndpoints2.txt";
	TestCaseBegin();
	SetTestSocket(&g_FirstRemote, 0, capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	g_TestOut.str("");
	{
		BroadcastReceiver dummyReceiver(BroadcastReceiver::BROADCAST_PORT, "", TestCallback);
		timeval timeout = {2, 0};
		std::thread myThread(std::ref(dummyReceiver), &timeout);

		nanosleep(&NANOSLEEP_TIME, NULL);
		CHECK(g_LastIndex == 0);
		SetTestSocket(&g_SecondRemote, 0, capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
		nanosleep(&NANOSLEEP_TIME, NULL);
		CHECK(g_LastIndex == 1);

		dummyReceiver.Stop();
		myThread.join();

		Endpoint& firstScored = dummyReceiver.GetEndpoint(0);
		++(++firstScored);
		Endpoint& secondScored = dummyReceiver.GetEndpoint(1);
		++(++secondScored);

		CHECK(2 == dummyReceiver.NumRemotes());
		CHECK(0x7F000001 == dummyReceiver.GetEndpoint(0).GetIp());
		CHECK(2000 == dummyReceiver.GetEndpoint(0).GetPort());

		dummyReceiver.WriteRecentEndpoints(TEST_FILENAME, 2);
	}
	BroadcastReceiver reread(BroadcastReceiver::BROADCAST_PORT, TEST_FILENAME, TestCallback);
	timeval timeout = {2, 0};
	std::thread myThread(std::ref(reread), &timeout);

		CHECK(2 == reread.NumRemotes());
		CHECK(0x7F000001 == reread.GetEndpoint(0).GetIp());
		CHECK(2000 == reread.GetEndpoint(0).GetPort());

	nanosleep(&NANOSLEEP_TIME, NULL);
	SetTestSocket(&g_ThirdRemote, 0, capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	nanosleep(&NANOSLEEP_TIME, NULL);
	CHECK(2 == g_LastIndex);


	reread.Stop();
	myThread.join();

	CHECK(3 == reread.NumRemotes());
	CHECK(0x7F000001 == reread.GetEndpoint(0).GetIp());
	CHECK(2000 == reread.GetEndpoint(0).GetPort());
	CHECK(0x7F000002 == reread.GetEndpoint(1).GetIp());
	CHECK(2000 == reread.GetEndpoint(1).GetPort());
	TestCaseEnd();
}


int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_BroadcastReceiver_TestEmpty);
	RunTest(true, ut_BroadcastReceiver_TestGetEndpoint);
	RunTest(true, ut_BroadcastReceiver_TestSimple);
	RunTest(true, ut_BroadcastReceiver_TestTwoSame);
	RunTest(true, ut_BroadcastReceiver_TestNoTimeout);
	RunTest(true, ut_BroadcastReceiver_TestRecentEndpoints);
	RunTest(true, ut_BroadcastReceiver_TestRecentEndpoints2);
	UnitTestMainEnd();
}

