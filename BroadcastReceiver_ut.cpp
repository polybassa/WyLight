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
#include "BroadcastReceiver.h"
#include "ClientSocket.h"
#include <thread>
#include <sstream>
#include <vector>
#include <iostream>
#include <unistd.h>

using std::vector;


static const timespec NANOSLEEP_TIME = {0, 5000000};

unsigned char capturedBroadcastMessage[110] = {
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

unsigned char capturedBroadcastMessage_2[110] = {
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

int ut_BroadcastReceiver_TestEmpty(void)
{
	TestCaseBegin();
	BroadcastReceiver dummyReceiver;
	CHECK(0 == dummyReceiver.NumRemotes());
	TestCaseEnd();
}

int ut_BroadcastReceiver_TestSimple(void)
{
	TestCaseBegin();
	std::ostringstream out;
	UdpSocket udpSock(0x7f000001, BroadcastReceiver::BROADCAST_PORT, false);
	BroadcastReceiver dummyReceiver;
	timeval timeout = {2, 0};
	std::thread myThread(std::ref(dummyReceiver), std::ref(out), &timeout);
	nanosleep(&NANOSLEEP_TIME, NULL);
	udpSock.Send(capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	dummyReceiver.Stop();
	myThread.join();

	CHECK(0 == out.str().compare("0:7f000001\n"));
	CHECK(1 == dummyReceiver.NumRemotes());
	CHECK(0x7F000001 == dummyReceiver.GetIp(0));
	TestCaseEnd();
}

int ut_BroadcastReceiver_TestTwo(void)
{
	TestCaseBegin();
	std::ostringstream out;
	UdpSocket udpSock(0x7f000001, BroadcastReceiver::BROADCAST_PORT, false);
	BroadcastReceiver dummyReceiver;
	timeval timeout = {3, 0};
	std::thread myThread(std::ref(dummyReceiver), std::ref(out), &timeout);
	nanosleep(&NANOSLEEP_TIME, NULL);
	udpSock.Send(capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	nanosleep(&NANOSLEEP_TIME, NULL);
	udpSock.Send(capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	dummyReceiver.Stop();
	myThread.join();

	CHECK(0 == out.str().compare("0:7f000001\n1:7f000001\n"));
	CHECK(2 == dummyReceiver.NumRemotes());
	CHECK(0x7F000001 == dummyReceiver.GetIp(0));
	CHECK(0x7F000001 == dummyReceiver.GetIp(1));
	TestCaseEnd();
}

int ut_BroadcastReceiver_TestNoTimeout(void)
{
	TestCaseBegin();
	std::ostringstream out;
	UdpSocket udpSock(0x7f000001, BroadcastReceiver::BROADCAST_PORT, false);
	BroadcastReceiver dummyReceiver;
	std::thread myThread(std::ref(dummyReceiver), std::ref(out));
	nanosleep(&NANOSLEEP_TIME, NULL);
	udpSock.Send(capturedBroadcastMessage, sizeof(capturedBroadcastMessage));
	nanosleep(&NANOSLEEP_TIME, NULL);
	udpSock.Send(capturedBroadcastMessage_2, sizeof(capturedBroadcastMessage_2));
	dummyReceiver.Stop();
	myThread.join();

	CHECK(0 == out.str().compare("0:7f000001\n1:7f000001\n"));
	CHECK(2 == dummyReceiver.NumRemotes());
	CHECK(0x7F000001 == dummyReceiver.GetIp(0));
	CHECK(0x7F000001 == dummyReceiver.GetIp(1));
	TestCaseEnd();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_BroadcastReceiver_TestEmpty);
	RunTest(true, ut_BroadcastReceiver_TestSimple);
	RunTest(true, ut_BroadcastReceiver_TestTwo);
	RunTest(true, ut_BroadcastReceiver_TestNoTimeout);
	UnitTestMainEnd();
}

