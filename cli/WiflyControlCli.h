/**
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

#ifndef _WIFLYCONTROLCLI_H_
#define _WIFLYCONTROLCLI_H_
#include <string>
#include "WiflyControl.h"
#include <stdint.h>

class WiflyControlCli
{
private:
	WyLight::Control mControl;
	bool mRunning;
	void ShowHelp(void) const;

public:
	WiflyControlCli(uint32_t addr, uint16_t port);
	void Run(void);
	WyLight::Control& getControl(void);
};
#endif /* #ifndef _WIFLYCONTROLCLI_H_ */
