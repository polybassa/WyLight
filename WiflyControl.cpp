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

