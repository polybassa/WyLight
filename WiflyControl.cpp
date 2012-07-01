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

#include "WiflyControl.h"
#include <iostream>
#include <sstream>

using namespace std;

WiflyControl::WiflyControl()
: mSock(INADDR_ANY, 12345)
{
	mCmdFrame.stx = STX;
	mCmdFrame.length = (uns8)sizeof(struct cmd_set_color) + 2;
	mCmdFrame.led.cmd = SET_COLOR;
	mCmdFrame.led.data.set_color.addr[0] = 0xff;
	mCmdFrame.led.data.set_color.addr[1] = 0xff;
	mCmdFrame.led.data.set_color.addr[2] = 0xff;
	mCmdFrame.led.data.set_color.addr[3] = 0xff;
	mCmdFrame.led.data.set_color.red = 128;
	mCmdFrame.led.data.set_color.green = 0;
	mCmdFrame.led.data.set_color.blue = 0;
	mCmdFrame.led.data.set_color.reserved[0] = 0;
	mCmdFrame.led.data.set_color.reserved[1] = 0;
	mCmdFrame.crcHigh = 0xDE;
	mCmdFrame.crcLow = 0xAD;
}

void WiflyControl::SetColor(char r, char g, char b)
{
	mCmdFrame.led.data.set_color.red = r;
	mCmdFrame.led.data.set_color.green = g;
	mCmdFrame.led.data.set_color.blue = b;
	int bytesWritten = mSock.Send((char*)&mCmdFrame, sizeof(mCmdFrame));
	std::cout << "Send " << bytesWritten << " bytes "
		<< (int)mCmdFrame.led.data.set_color.red << " "
		<< (int)mCmdFrame.led.data.set_color.green << " "
		<< (int)mCmdFrame.led.data.set_color.blue
		<< std::endl;
}

void WiflyControl::SetColor(unsigned long rgba)
{
	char r = (rgba & 0xff000000) >> 24;
	char g = (rgba & 0x00ff0000) >> 16;
	char b = (rgba & 0x0000ff00) >> 8;
	SetColor(r, g, b);
}

void WiflyControl::SetColor(string& s)
{
	SetColor(ToRGBA(s));
}

unsigned long WiflyControl::ToRGBA(string& s) const
{
	if(s.length() > 6) return 0;

	long rgba;
	stringstream converter;
	converter << std::hex << s;
	converter >> rgba;
	cout << rgba << "<" << endl;
	return rgba << 8;
}

