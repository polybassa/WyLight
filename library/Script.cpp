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
		// std::string::npos++ will overflow to 0 just like we want!
		const size_t indexOfLastSeperator = filename.find_last_of('/') + 1;
		mName = filename.substr(indexOfLastSeperator);
		Script::deserialize(filename, *this);
	}

	Script::~Script(void)
	{
		clear();
	}

	bool Script::operator == (const Script& ref) const
	{
		if(size() != ref.size()) {
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

	Script::ScriptList::const_iterator Script::begin() const noexcept
	{
		return mList.begin();
	}

	void Script::clear()
	{
		for(auto cmd : mList) {
			delete cmd;
		}
		mList.clear();
	}

	void Script::deserialize(const std::string& filename, Script& newScript) throw (FatalError)
	{
		std::ifstream inFile(filename);
		if(!inFile.is_open()) {
			throw FatalError("Open '" + filename + "' to read script failed");
		}

		newScript.deserialize(inFile, newScript);
		inFile.close();
	}
	
	void Script::deserialize(std::istream &inStream, WyLight::Script &newScript) {
		std::string command;
		while(inStream >> command) {
			if(0 == command.compare(FwCmdLoopOn::TOKEN)) {
				newScript.push_back(new FwCmdLoopOn());
			} else if(0 == command.compare(FwCmdLoopOff::TOKEN)) {
				newScript.push_back(new FwCmdLoopOff(inStream));
			} else if(0 == command.compare(FwCmdWait::TOKEN)) {
				newScript.push_back(new FwCmdWait(inStream));
			} else if(0 == command.compare(FwCmdSetFade::TOKEN)) {
				newScript.push_back(new FwCmdSetFade(inStream));
			} else if(0 == command.compare(FwCmdSetGradient::TOKEN)) {
				newScript.push_back(new FwCmdSetGradient(inStream));
			} else {
				throw FatalError("Unknown command: " + command);
			}
		}
	}

	Script::ScriptList::const_iterator Script::end() const noexcept
	{
		return mList.end();
	}

	const std::string& Script::getName() const
	{
		return mName;
	}

	void Script::push_back(FwCmdScript *pNew)
	{
		mList.push_back(pNew);
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

	size_t Script::size() const
	{
		return mList.size();
	}
} /* namespace WyLight */
