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

#include "Script.h"
#include "trace.h"
#include <fstream>

namespace WyLight {

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

Script::Script(const std::string& filename)
{
	Script::Read(filename, *this);
}

Script::~Script(void)
{
	while(!mList.empty()) {
		const FwCmdScript* pNext = mList.back();
		mList.pop_back();
		delete pNext;
	}
}

void Script::Add(const FwCmdScript* pCmd)
{
	mList.push_back(pCmd);
}

std::list<const FwCmdScript*>::const_iterator Script::Begin(void) const
{
	return mList.begin();
}

std::list<const FwCmdScript*>::const_iterator Script::End(void) const
{
	return mList.end();
}

bool Script::Equals(const Script& ref) const
{
	if(mList.size() != ref.mList.size()) {
		return false;
	}

	auto refIt = ref.Begin();
	for(auto it = mList.begin(); it != mList.end(); ++it, ++refIt) {
		if(!(*it)->Equals(**refIt)) {
			return false;
		}
	}
	return true;
}

void Script::Read(const std::string& filename, Script& newScript)
{
	std::ifstream inFile(filename);
	if(!inFile.is_open()) {
		Trace(ZONE_ERROR, "Open file to read script failed\n");
		return;	
	}

	std::string command;
	while(inFile >> command) {

		if (0 == command.compare(FwCmdLoopOn::TOKEN)) {
			newScript.Add(new FwCmdLoopOn());
		} else if (0 == command.compare(FwCmdLoopOff::TOKEN)) {
			newScript.Add(new FwCmdLoopOff(inFile));
		} else if (0 == command.compare(FwCmdWait::TOKEN)) {
			newScript.Add(new FwCmdWait(inFile));
		} else if (0 == command.compare(FwCmdSetFade::TOKEN)) {
			newScript.Add(new FwCmdSetFade(inFile));
		} else if (0 == command.compare(FwCmdSetGradient::TOKEN)) {
			newScript.Add(new FwCmdSetGradient(inFile));
		} else {
			Trace(ZONE_ERROR, "Unknown command '%s'\n", command.c_str());
			assert(false);
		}
	}
	inFile.close();
}

void Script::Write(const std::string& filename, const Script& newScript)
{
	std::ofstream outFile(filename);
	if(!outFile.is_open()) {
		Trace(ZONE_ERROR, "Open file to save script failed\n");
		return;	
	}

	std::string command;
	size_t identation = 0;
	for(auto it = newScript.Begin(); it != newScript.End(); ++it) {
		(*it)->Write(outFile, identation) << '\n';
	}
	outFile.close();
}
} /* namespace WyLight */
