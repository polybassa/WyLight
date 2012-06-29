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

#include "ClientSocket.h"

#include <cstring>
#include <iostream>
#include <unistd.h>

#ifdef ANDROID
#include <jni.h>
#endif

ClientSocket::ClientSocket(long addr, short port)
	: mSock(socket(AF_INET, SOCK_DGRAM, 0))
{
	memset(&mSockAddr, 0, sizeof(mSockAddr));
	mSockAddr.sin_family = AF_INET;
	mSockAddr.sin_port = htons(port);
	mSockAddr.sin_addr.s_addr = htonl(addr);
}

ClientSocket::~ClientSocket()
{
	close(mSock);
}

int ClientSocket::Send(struct cmd_frame* frame, size_t length) const
{
	return sendto(mSock, (char*)frame, length, 0, (struct sockaddr*)&mSockAddr, sizeof(mSockAddr));
}

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
	int bytesWritten = mSock.Send(&mCmdFrame, sizeof(mCmdFrame));
	std::cout << "Send " << bytesWritten << " bytes "
		<< (int)mCmdFrame.led.data.set_color.red << " "
		<< (int)mCmdFrame.led.data.set_color.green << " "
		<< (int)mCmdFrame.led.data.set_color.blue
		<< std::endl;
}

void colorLoop(WiflyControl& control)
{
	for(;;)
	{
		control.SetColor(128, 0, 0);
		sleep(1);
		control.SetColor(0, 128, 0);
		sleep(1);
		control.SetColor(0, 0, 128);
		sleep(1);
	}
}

#ifdef ANDROID
extern "C" {
void Java_biz_bruenn_WiflyLight_WiflyLightActivity_runClient(JNIEnv* env, jobject ref)
{
	WiflyControl control;
	colorLoop(control);
}
}
#endif

int main(int argc, const char* argv[])
{
	WiflyControl control;
	colorLoop(control);
	return 0;
}
