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
#include "MessageQueue.h"
#include "trace.h"
#include <thread>
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <functional>

using namespace WyLight;

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

/******************************* test functions *******************************/
int32_t ut_MessageQueue_simple(void)
{
	TestCaseBegin();

	std::mutex mMut;

	std::atomic_int mInt;
	mInt = 0;
	std::atomic_char running;
	running = 1;

	MessageQueue < std::function < int(void) >> messages;

	std::thread t1([&]
		{
			while(running)
			{
				const auto f = messages.receive();
				if(f) mInt = f();
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		}
		);

	CHECK(0 == mInt);

	messages.push_back([ = ] {return 1; }
			   );
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	CHECK(1 == mInt);

	messages.push_back([ = ] {return 2; }
			   );
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	CHECK(2 == mInt);

	messages.push_back([ = ] {return 4; }
			   );
	messages.push_front([ = ] {return 3; }
			    );
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	CHECK(3 == mInt);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	CHECK(4 == mInt);

	{
		std::lock_guard<std::mutex> lg(mMut);
		messages.push_back([ = ] {return 5; }
				   );
		messages.push_back([ = ] {return 5; }
				   );
		messages.push_back([ = ] {return 5; }
				   );
		messages.clear_and_push_front([ = ] {return 6; }
					      );
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	CHECK(6 == mInt);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	CHECK(6 == mInt);

	messages.push_back([ = ] {return 5; }
			   );
	running = 0;
	t1.join();

	TestCaseEnd();
}


int32_t ut_MessageQueue_complex(void)
{
	TestCaseBegin();

	std::mutex mMut;

	std::atomic_int mInt;
	mInt = 0;

	typedef std::function<int (void)> mFuncType;

	MessageQueue < std::pair < std::function<int(void)>, bool >> messages;

	std::thread t1([&]
		{
			while(true)
			{
				const auto f = messages.receive();
				if(f.second) break;
				if(f.first) mInt = f.first();
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}
		);

	CHECK(0 == mInt);

	messages.push_back(std::make_pair([ = ] {return 1; }, false));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	CHECK(1 == mInt);

	messages.push_back(std::make_pair([ = ] {return 2; }, false));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	CHECK(2 == mInt);

	messages.push_back(std::make_pair([ = ] {return 4; }, false));
	messages.push_back(std::make_pair([ = ] {return 5; }, false));
	messages.push_front(std::make_pair([ = ] {return 3; }, false));
	std::this_thread::sleep_for(std::chrono::milliseconds(40));
	CHECK(3 == mInt);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CHECK(4 == mInt);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CHECK(5 == mInt);


	{
		std::lock_guard<std::mutex> lg(mMut);
		messages.push_back(std::make_pair([ = ] {return 4; }, false));
		messages.push_back(std::make_pair([ = ] {return 4; }, false));
		messages.push_back(std::make_pair([ = ] {return 4; }, false));
		messages.clear_and_push_front(std::make_pair([ = ] {return 6; }, false));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CHECK(6 == mInt);
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	CHECK(6 == mInt);

	messages.push_front(std::make_pair([ = ] {return 0xff; }, true));
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	t1.join();

	TestCaseEnd();
}



int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_MessageQueue_simple);
	RunTest(true, ut_MessageQueue_complex);
	UnitTestMainEnd();
}
