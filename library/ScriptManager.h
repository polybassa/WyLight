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

#ifndef __WyLight__ScriptManager__
#define __WyLight__ScriptManager__

#include "Script.h"
#include "WiflyControlException.h"
#include <string>
#include <vector>

namespace WyLight
{
class ScriptManager {
    const std::string m_Path;
    std::vector<std::string> m_ScriptFiles;
    static bool hasScriptFileExtension(const std::string& filename);

public:
    static const std::string EXTENSION;

    ScriptManager(const std::string& path);
    ~ScriptManager(void);

    Script getScript(size_t index) const;
    const std::string& getScriptName(size_t index) const;
    size_t numScripts() const;
};
} /* namespace WyLight */
#endif /* #ifndef __WyLight__ScriptManager__ */
