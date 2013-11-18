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

Script::Script()
{
}

Script::Script(const std::string& filename)
{
	// std::string::npos++ will overflow to 0 just like we want!
	const size_t indexOfLastSeperator = filename.find_last_of('/') + 1;
	mName = filename.substr(indexOfLastSeperator);
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

void Script::deserialize(const std::string& filename, Script& newScript) throw (FatalError)
{
	std::ifstream inFile(filename);
	if(!inFile.is_open()) {
		throw FatalError("Open '" + filename + "' to read script failed");
	}

	std::string command;
	while(inFile >> command) {
		if (0 == command.compare(FwCmdLoopOn::TOKEN)) {
			newScript.push_back(new FwCmdLoopOn());
		} else if (0 == command.compare(FwCmdLoopOff::TOKEN)) {
			newScript.push_back(new FwCmdLoopOff(inFile));
		} else if (0 == command.compare(FwCmdWait::TOKEN)) {
			newScript.push_back(new FwCmdWait(inFile));
		} else if (0 == command.compare(FwCmdSetFade::TOKEN)) {
			newScript.push_back(new FwCmdSetFade(inFile));
		} else if (0 == command.compare(FwCmdSetGradient::TOKEN)) {
			newScript.push_back(new FwCmdSetGradient(inFile));
		} else {
			throw FatalError("Unknown command: " + command);
		}
	}
	inFile.close();
}

const std::string& Script::getName() const
{
	return mName;
}

void Script::serialize(const std::string& filename, const Script& newScript) throw (FatalError)
{
	std::ofstream outFile(filename);
	if(!outFile.is_open()) {
		throw FatalError("Open '" + filename + "' to save script failed");
	}

	std::string command;
	size_t identation = 0;
	for(const auto& cmd : newScript) {
		cmd->Write(outFile, identation) << '\n';
	}
	outFile.close();
}
} /* namespace WyLight */
