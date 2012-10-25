/**
		Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

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

#ifndef _WIFLYCONTROLCLI_H_
#define _WIFLYCONTROLCLI_H_
#include <string>
#include "WiflyControl.h"

class WiflyControlCli
{
	private:
		WiflyControl mControl;
		bool mRunning;
		void ShowHelp(void) const;

	public:
		WiflyControlCli(const char* pAddr, short port, bool useTcp);
		void Run(void);	
};
#endif /* #ifndef _WIFLYCONTROLCLI_H_ */
