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
#include <unistd.h>
#include "FtpServer.h"

#include <iostream>
#include <fstream>

//using namespace WyLight;

size_t ut_FtpServer_Test(void)
{
	TestCaseBegin();
	WyLight::FtpServer testee;
	char test;
	std::cin >> test;

	CHECK('x' == 1);
	TestCaseEnd();
}

int main (int argc, const char *argv[])
{
	
	WyLight::FtpServer testee;
	while(1){
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
	/*
	UnitTestMainBegin();
	RunTest(true,  ut_FtpServer_Test);
	UnitTestMainEnd();
	 
	 */
}

