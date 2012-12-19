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
#include "BroadcastReceiver.h"
#include <boost/asio.hpp>
#include <vector>
#include <iostream>
using std::vector;
using boost::asio::ip::udp;

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

int ut_BroadcastReceiver_TestEmpty(void)
{
	TestCaseBegin();
	BroadcastReceiver dummyReceiver(55555);
	vector<std::string> ipTable;
	CHECK(0 == dummyReceiver.GetIpTable(ipTable));
	CHECK(0 == ipTable.size());
	TestCaseEnd();
}

int ut_BroadcastReceiver_TestSimple(void)
{
	TestCaseBegin();
	try {
		boost::asio::io_service io_service;
		udp::socket sock(io_service, udp::endpoint(udp::v4(), 0));
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", "55555");
		udp::resolver::iterator it = resolver.resolve(query);
		BroadcastReceiver dummyReceiver(55555);
		sleep(1);
		sock.send_to(boost::asio::buffer(capturedBroadcastMessage, sizeof(capturedBroadcastMessage)), *it);
		dummyReceiver.Stop();

		vector<std::string> ipTable;
		size_t numElements = dummyReceiver.GetIpTable(ipTable);
		CHECK(1 == numElements);
		CHECK(ipTable.size() == numElements);
	} catch (std::exception& e) {
		std::cout << __FUNCTION__ << ':' <<  e.what() << '\n';
		errors++;
	}
	TestCaseEnd();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_BroadcastReceiver_TestEmpty);
	RunTest(true, ut_BroadcastReceiver_TestSimple);
	UnitTestMainEnd();
}

