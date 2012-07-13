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

#ifndef _WIFLYCONTROL_H_
#define _WIFLYCONTROL_H_

#include <string>
#include "ClientSocket.h"
#include "wifly_cmd.h"

class WiflyControl
{
	private:
		struct cmd_frame mCmdFrame;
		const ClientSocket mSock;
		unsigned long ToRGBA(std::string& s) const;
		
	public:
		WiflyControl();
		/**
			rgba is a 32 Bit rgb value with alpha channel. Alpha is unused, but easier to handle
			f.e. red(255, 0, 0) is in rgba as: 0xff000000
				 white(255, 255, 255) is in rgba as: 0xffffff00
		**/
		void SetColor(unsigned long addr, unsigned long rgba);
		void SetColor(std::string& addr, std::string& color);
		void SetFade(unsigned long addr, unsigned long rgba, unsigned short fadeTmms);
		void SetFade(std::string& addr, std::string& color, unsigned short fadeTmms);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
