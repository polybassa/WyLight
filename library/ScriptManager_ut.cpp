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
#include "ScriptManager.h"

using namespace WyLight;

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;


const size_t FwCmdScript::INDENTATION_MAX;
const char FwCmdScript::INDENTATION_CHARACTER;

const std::string FwCmdSetFade::TOKEN("fade");
const std::string FwCmdSetGradient::TOKEN("gradient");
const std::string FwCmdLoopOn::TOKEN("loop");
const std::string FwCmdLoopOff::TOKEN("loop_off");
const std::string FwCmdWait::TOKEN("wait");

size_t ut_ScriptManager_Empty(void)
{
	TestCaseBegin();
	ScriptManager testee {"./binary"};

	// there shouldn't be any script
		CHECK(0 == testee.numScripts());

	// we expect an out of bounds exception when no script is available
	try {
		testee.getScript(0);
		CHECK(false);
	} catch(const FatalError& e) {
		CHECK(    0 == strcmp(e.what(), "ScriptManager: Index out of bounds"));
	}
	TestCaseEnd();
}

size_t ut_ScriptManager_Good(void)
{
	TestCaseBegin();
	// preparing a temporary scriptfile
	const Script refScript("TestInput.txt");
	const std::string tempScript {std::string("./binary/TestOutput") + ScriptManager::EXTENSION};
	Script::serialize(tempScript, refScript);

	ScriptManager testee {"./binary"};
	CHECK(        1 == testee.numScripts());
	CHECK(refScript == testee.getScript(0));

	// clean up the temp file
	std::remove(tempScript.c_str());
	TestCaseEnd();
}

int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_ScriptManager_Empty);
	RunTest(true, ut_ScriptManager_Good);
	UnitTestMainEnd();
}

