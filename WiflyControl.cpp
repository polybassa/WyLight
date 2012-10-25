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

#include "WiflyControl.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <stddef.h>

using namespace std;

void* RunReceiving(void* pObj)
{
	WiflyControl* pMe = reinterpret_cast<WiflyControl*>(pObj);
	pMe->Receiving();
	return NULL;
}

WiflyControl::WiflyControl(const char* pAddr, short port, bool useTcp)
{
	mCmdFrame.stx = STX;
	mCmdFrame.length = (uns8)sizeof(struct cmd_set_color) + 2;
	mCmdFrame.crcHigh = 0xDE;
	mCmdFrame.crcLow = 0xAD;

	if(useTcp)
	{
		mSock = new TcpSocket(pAddr, port);
		assert(mSock);

		//pthread_create(&mRecvThread, 0, RunReceiving, this);
	}
	else
	{
		mSock = new UdpSocket(pAddr, port);
		assert(mSock);
	}
}

void WiflyControl::Receiving() const
{
	unsigned char buffer[2048];
	int bytesReceived;
	cout << "Receiving..." << endl;
	for(;;)
	{
		bytesReceived = mSock->Recv(buffer, sizeof(buffer) - 1);
		buffer[sizeof(buffer) - 1] = '\0';
		cout << "Trace " << bytesReceived << " bytes: >>" << buffer << "<<" << endl;
	}
}

void WiflyControl::AddColor(unsigned long addr, unsigned long rgba, unsigned char hour, unsigned char minute, unsigned char second)
{
	mCmdFrame.led.cmd = ADD_COLOR;
	// next line would be better, but mCmdFrame...addr is unaligned!
	//*(unsigned long*)mCmdFrame.led.data.set_color.addr = htonl(addr);
/**TODO mCmdFrame.led.data.add_color.addr[0] = (addr & 0xff000000) >> 24;
	mCmdFrame.led.data.add_color.addr[1] = (addr & 0x00ff0000) >> 16;
	mCmdFrame.led.data.add_color.addr[2] = (addr & 0x0000ff00) >> 8;
	mCmdFrame.led.data.add_color.addr[3] = (addr & 0x000000ff);
*/
	mCmdFrame.led.data.add_color.red = (rgba & 0xff000000) >> 24;
	mCmdFrame.led.data.add_color.green = (rgba & 0x00ff0000) >> 16;
	mCmdFrame.led.data.add_color.blue = (rgba & 0x0000ff00) >> 8;
//TODO	mCmdFrame.led.data.add_color.hour = hour;
	mCmdFrame.led.data.add_color.minute = minute;
	mCmdFrame.led.data.add_color.second = second;
	int bytesWritten = mSock->Send(reinterpret_cast<unsigned char*>(&mCmdFrame), sizeof(mCmdFrame));
	assert(sizeof(mCmdFrame) == bytesWritten);
}

void WiflyControl::AddColor(string& addr, string& rgba, unsigned char hour, unsigned char minute, unsigned char second)
{
	AddColor(ToRGBA(addr), ToRGBA(rgba) << 8, hour, minute, second);
}

size_t WiflyControl::BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize) const
{
	BlProxy proxy(mSock);
	unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
	size_t bytesReceived = proxy.Send(req, buffer, sizeof(buffer));

	cout << __FILE__ << "::" << __FUNCTION__ << "(): " << bytesReceived << ":" << sizeof(BlInfo) << endl;
	if(responseSize == bytesReceived)
	{
		memcpy(pResponse, buffer, responseSize);
		return responseSize;
	}
	return 0;
}

size_t WiflyControl::BlReadEeprom(unsigned char* pBuffer, unsigned int address, const size_t numBytes) const
{
	BlEepromReadRequest readRequest(address, numBytes);
	return BlRead(readRequest, pBuffer, numBytes);
}

size_t WiflyControl::BlReadFlash(unsigned char* pBuffer, unsigned int address, const size_t numBytes) const
{
	BlFlashReadRequest readRequest(address, numBytes);
	return BlRead(readRequest, pBuffer, numBytes);
}

size_t WiflyControl::BlReadInfo(BlInfo& blInfo)
{
	BlInfoRequest request;
	return BlRead(request, reinterpret_cast<unsigned char*>(&blInfo), sizeof(BlInfo));
}

void WiflyControl::SetColor(unsigned long addr, unsigned long rgba)
{
	mCmdFrame.led.cmd = SET_COLOR;
	// next line would be better, but mCmdFrame...addr is unaligned!
	//*(unsigned long*)mCmdFrame.led.data.set_color.addr = htonl(addr);
	mCmdFrame.led.data.set_color.addr[0] = (addr & 0xff000000) >> 24;
	mCmdFrame.led.data.set_color.addr[1] = (addr & 0x00ff0000) >> 16;
	mCmdFrame.led.data.set_color.addr[2] = (addr & 0x0000ff00) >> 8;
	mCmdFrame.led.data.set_color.addr[3] = (addr & 0x000000ff);
	mCmdFrame.led.data.set_color.red = (rgba & 0xff000000) >> 24;
	mCmdFrame.led.data.set_color.green = (rgba & 0x00ff0000) >> 16;
	mCmdFrame.led.data.set_color.blue = (rgba & 0x0000ff00) >> 8;
	int bytesWritten = mSock->Send(reinterpret_cast<unsigned char*>(&mCmdFrame), sizeof(mCmdFrame));
#ifdef DEBUG
	cout << "Send " << bytesWritten << " bytes "
		<< addr << " | "
		<< (int)mCmdFrame.led.data.set_color.addr[0] << " "
		<< (int)mCmdFrame.led.data.set_color.addr[1] << " "
		<< (int)mCmdFrame.led.data.set_color.addr[2] << " "
		<< (int)mCmdFrame.led.data.set_color.addr[3] << " : "
		<< (int)mCmdFrame.led.data.set_color.red << " "
		<< (int)mCmdFrame.led.data.set_color.green << " "
		<< (int)mCmdFrame.led.data.set_color.blue
		<< endl;
#else
	assert(sizeof(mCmdFrame) == bytesWritten);
#endif
}

void WiflyControl::SetColor(string& addr, string& rgba)
{
	SetColor(ToRGBA(addr), ToRGBA(rgba) << 8);
}

void WiflyControl::SetFade(unsigned long addr, unsigned long rgba, unsigned short fadeTmms)
{
	mCmdFrame.led.cmd = SET_FADE;
	// next line would be better, but mCmdFrame...addr is unaligned!
	//*(unsigned long*)mCmdFrame.led.data.set_color.addr = htonl(addr);
	mCmdFrame.led.data.set_fade.addr[0] = (addr & 0xff000000) >> 24;
	mCmdFrame.led.data.set_fade.addr[1] = (addr & 0x00ff0000) >> 16;
	mCmdFrame.led.data.set_fade.addr[2] = (addr & 0x0000ff00) >> 8;
	mCmdFrame.led.data.set_fade.addr[3] = (addr & 0x000000ff);
	mCmdFrame.led.data.set_fade.red = (rgba & 0xff000000) >> 24;
	mCmdFrame.led.data.set_fade.green = (rgba & 0x00ff0000) >> 16;
	mCmdFrame.led.data.set_fade.blue = (rgba & 0x0000ff00) >> 8;
	mCmdFrame.led.data.set_fade.fadeTmms = htons(fadeTmms);
	int bytesWritten = mSock->Send(reinterpret_cast<unsigned char*>(&mCmdFrame), sizeof(mCmdFrame));
#ifdef DEBUG
	cout << "Send " << bytesWritten << " bytes "
		<< addr << " | "
		<< (int)mCmdFrame.led.data.set_fade.addr[0] << " "
		<< (int)mCmdFrame.led.data.set_fade.addr[1] << " "
		<< (int)mCmdFrame.led.data.set_fade.addr[2] << " "
		<< (int)mCmdFrame.led.data.set_fade.addr[3] << " : "
		<< (int)mCmdFrame.led.data.set_fade.red << " "
		<< (int)mCmdFrame.led.data.set_fade.green << " "
		<< (int)mCmdFrame.led.data.set_fade.blue << " : "
		<< (int)mCmdFrame.led.data.set_fade.fadeTmms << endl;

		unsigned int tempTmms = 0;
		do
		{
			tempTmms += 1000;
			cout << (int)tempTmms/1000;
			cout.flush();
			sleep(1); 
		}while(tempTmms < fadeTmms);
		cout << endl;
#else
	assert(sizeof(mCmdFrame) == bytesWritten);
#endif
}

void WiflyControl::SetFade(string& addr, string& rgba, unsigned short fadeTmms)
{
	SetFade(ToRGBA(addr), ToRGBA(rgba) << 8, fadeTmms);
}

unsigned long WiflyControl::ToRGBA(string& s) const
{
	if(s.length() > 8) return 0;

	// use a stringstream to convert hex ascii string into machine bits
	unsigned long rgba;
	stringstream converter;
	converter << hex << s;
	converter >> rgba;
	cout << rgba << "<" << endl;
	return rgba;
}

