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
		sock.send_to(boost::asio::buffer("WiFly_Light", 12), *it);
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

