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
#include "Script.h"

/**************** includes, classes and functions for wrapping ****************/

using namespace WyLight;

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

static const FwCmdSetFade refFade(0x1234, 100, 0x112233, true);
static const FwCmdSetGradient refGradient(0x112233, 0x445566, false, 1, 2, 100);
static const FwCmdLoopOff refLoopOff(0);
static const FwCmdLoopOn refLoop;
static const FwCmdWait refWait(5000);

const std::string FwCmdSetFade::TOKEN("fade");
const std::string FwCmdSetGradient::TOKEN("gradient");
const std::string FwCmdLoopOn::TOKEN("loop");
const std::string FwCmdLoopOff::TOKEN("loop_off");
const std::string FwCmdWait::TOKEN("wait");

const size_t FwCmdScript::INDENTATION_MAX;
const char FwCmdScript::INDENTATION_CHARACTER;




#if 0
void badFade(void)
{
	Script newScript("TestInput.txt");
	auto nextCmd = newScript.Begin();
	
	assert(refLoop.Equals(*(*(nextCmd++))));
	assert(refGradient.Equals(*(*(nextCmd++))));

	FadeCmd badFade(1234, 0x0B1621, true, 101);
	assert(!badFade.Equals(*(*(nextCmd++))));
}

void badGradient(void)
{
	Script newScript("TestInput.txt");
	auto nextCmd = newScript.Begin();
	
	assert(refLoop.Equals(*(*(nextCmd++))));
	
	GradientCmd badGradient(0x0B1621, 0x2C3742, 0, 1, 2, 101);
	assert(!badGradient.Equals(*(*(nextCmd++))));
}

void badLoopEnd(void)
{
	Script newScript("TestInput.txt");
	auto nextCmd = newScript.Begin();
	
	assert(refLoop.Equals(*(*(nextCmd++))));
	assert(refGradient.Equals(*(*(nextCmd++))));
	assert(refFade.Equals(*(*(nextCmd++))));
	assert(refWait.Equals(*(*(nextCmd++))));

	LoopEndCmd refLoopOff(1);
	assert(!refLoopOff.Equals(*(*(nextCmd++))));
}

void badWait(void)
{
	Script newScript("TestInput.txt");
	auto nextCmd = newScript.Begin();
	
	assert(refLoop.Equals(*(*(nextCmd++))));
	assert(refGradient.Equals(*(*(nextCmd++))));
	assert(refFade.Equals(*(*(nextCmd++))));

	WaitCmd refWait(5001);
	assert(!refWait.Equals(*(*(nextCmd++))));
}
#endif

size_t ut_Script_ReadGood(void)
{
	TestCaseBegin();
	Script newScript("TestInput.txt");
	auto nextCmd = newScript.Begin();
	
	CHECK(refLoop.Equals(*(*(nextCmd++))));
	CHECK(refGradient.Equals(*(*(nextCmd++))));
	CHECK(refFade.Equals(*(*(nextCmd++))));
	CHECK(refWait.Equals(*(*(nextCmd++))));
	CHECK(refLoopOff.Equals(*(*(nextCmd++))));
	CHECK(nextCmd == newScript.End());
	TestCaseEnd();
}

size_t ut_Script_WriteGood(void)
{
	TestCaseBegin();
	Script refScript("TestInput.txt");
	Script::Write("TestOutput.txt", refScript);
	Script newScript("TestOutput.txt");
	CHECK(newScript.Equals(refScript));
	TestCaseEnd();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_Script_ReadGood);
#if 0
	good();
	badFade();
	badGradient();
	badLoopEnd();
	badWait();
#endif
	RunTest(true, ut_Script_WriteGood);
	UnitTestMainEnd();
}

