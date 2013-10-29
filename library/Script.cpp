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
}

Script::Script(const std::string& filename)
{
	Script::deserialize(filename, *this);
}

Script::~Script(void)
{
}

bool Script::operator == (const Script& ref) const
{
	if(this->list::size() != ref.list::size()) {
		return false;
	}

	auto refIt = ref.begin();
	for(const auto& cmd : *this) {
		if(*cmd != **refIt++) {
			return false;
		}
	}
	return true;
}

void Script::deserialize(const std::string& filename, Script& newScript)
{
	std::ifstream inFile(filename);
	if(!inFile.is_open()) {
		Trace(ZONE_ERROR, "Open file to read script failed\n");
		return;	
	}

	std::string command;
	while(inFile >> command) {
		if (0 == command.compare(FwCmdLoopOn::TOKEN)) {
			newScript.emplace_back(FwCmdLoopOn());
		} else if (0 == command.compare(FwCmdLoopOff::TOKEN)) {
			newScript.emplace_back(FwCmdLoopOff(inFile));
		} else if (0 == command.compare(FwCmdWait::TOKEN)) {
			newScript.emplace_back(FwCmdWait(inFile));
		} else if (0 == command.compare(FwCmdSetFade::TOKEN)) {
			newScript.emplace_back(FwCmdSetFade(inFile));
		} else if (0 == command.compare(FwCmdSetGradient::TOKEN)) {
			newScript.emplace_back(FwCmdSetGradient(inFile));
		} else {
			Trace(ZONE_ERROR, "Unknown command '%s'\n", command.c_str());
			assert(false);
		}
	}
	inFile.close();
}

void Script::serialize(const std::string& filename, const Script& newScript)
{
	std::ofstream outFile(filename);
	if(!outFile.is_open()) {
		Trace(ZONE_ERROR, "Open file to save script failed\n");
		return;	
	}

	std::string command;
	size_t identation = 0;
	for(const auto& cmd : newScript) {
		cmd->Write(outFile, identation) << '\n';
	}
	outFile.close();
}
} /* namespace WyLight */
