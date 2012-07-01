/**
		Copyright (C) 2012 Nils Weiss, Patrick Brünn.

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

#define SET_COLOR_RED 128, 0, 0
#define SET_COLOR_GREEN 0, 128, 0
#define SET_COLOR_BLUE 0, 0, 128


class WiflyControlCli
{
	private:
		WiflyControl mControl;
		bool mRunning;

	public:
		WiflyControlCli(void);
		void run(void);	
};
#endif /* #ifndef _WIFLYCONTROLCLI_H_ */
