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

#include "ScriptManager.h"
#include "trace.h"
#include <sys/types.h>
#include <dirent.h>

namespace WyLight {

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;
static const std::string EMPTY_STRING{};
const std::string ScriptManager::EXTENSION{".txt"};

ScriptManager::ScriptManager()
{
	DIR* const searchDir = opendir(".");
	if(NULL == searchDir) {
		Trace(ZONE_ERROR, "Open script directory failed\n");
		return;
	}

	for(struct dirent* file = readdir(searchDir); NULL != file; file = readdir(searchDir)) {
		std::string filename(file->d_name);
		if(hasScriptFileExtension(filename)) {
			m_ScriptFiles.push_back(filename);
		}
	}
	closedir(searchDir);
}

ScriptManager::~ScriptManager(void)
{
}

const std::string& ScriptManager::getScript(size_t index) const
{
	if(index < m_ScriptFiles.size()) {
		return m_ScriptFiles[index];
	}

	Trace(ZONE_WARNING, "Index %u out of bounds\n", index);
	return EMPTY_STRING;
}

bool ScriptManager::hasScriptFileExtension(const std::string& filename)
{
	if(EXTENSION.length() > filename.length()) {
		return false;
	}
	return (0 == filename.compare(filename.length() - EXTENSION.length(), EXTENSION.length(), EXTENSION));
}

size_t ScriptManager::numScripts() const
{
	return m_ScriptFiles.size();
}
} /* namespace WyLight */
