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

#ifndef __WyLight__Script__
#define __WyLight__Script__

#include "FwCommand.h"

#include <list>
#include <string>

namespace WyLight {

class Script
{
	std::list<const FwCmdScript*> mList;

public:
	static void Read(const std::string& filename, Script& newScript);
	static void Write(const std::string& filename, const Script& newScript);

	Script(const std::string& filename);
	~Script(void);
	void Add(const FwCmdScript* pCmd);
	std::list<const FwCmdScript*>::const_iterator Begin() const;
	std::list<const FwCmdScript*>::const_iterator End() const;
	bool Equals(const Script& ref) const;
};
} /* namespace WyLight */
#endif /* #ifndef __WyLight__Script__ */

