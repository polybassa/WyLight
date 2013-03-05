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

#include "trace.h"
#include "unittest.h"
#include "WiflyControl.h"
#include <string>
#include <stdlib.h>
#include <time.h>

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

// global variables
uint8_t g_FlashRndDataPool[FLASH_SIZE];
uint8_t g_EepromRndDataPool[EEPROM_SIZE];
// empty wrappers to satisfy the linker
ClientSocket::ClientSocket(uint32_t, uint16_t, int) : mSock(0) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const { return 0;}
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const {return 0; }
ComProxy::ComProxy(const TcpSocket& sock) : mSock (sock) {}

#define return_resp {\
	for(unsigned int i = 0; i < sizeof(resp); i++)\
	{ \
		if(responseSize >= i) *pResponse++ = resp[i]; \
	}\
	return sizeof(resp);}


int32_t ComProxy::Send(BlRequest& req, uint8_t* pResponse, size_t responseSize, bool doSync) const
{
	if(typeid(req) == typeid(BlInfoRequest))
	{
		uint8_t resp[] = {0x00, 0x03, 0x01, 0x05, 0xff, 0x84, 0x00, 0xfd, 0x00, 0x00};
		return_resp;
	}
	if(typeid(req) == typeid(BlFlashEraseRequest))
	{
		uint8_t resp[] = {0x03};
		return_resp;
	}
	if(typeid(req) == typeid(BlFlashWriteRequest))
	{
		uint8_t resp[] = {0x04};
		return_resp;
	}
	if(typeid(req) == typeid(BlFlashReadRequest))
	{
		BlFlashReadRequest* mReq = dynamic_cast<BlFlashReadRequest*>(&req);
		
		uint32_t address = ((uint32_t)mReq->addressU) << 16;
		address += ((uint32_t)mReq->addressHigh) << 8;
		address += (uint32_t)mReq->addressLow;
		
		uint16_t bytes = ((uint16_t)mReq->numBytesHigh) << 8;
		bytes += ((uint16_t)mReq->numBytesLow);
		
		unsigned int i;
		for(i = 0; i < bytes; i++)
		{
			*pResponse++ = g_FlashRndDataPool[address + i];
		}
		return i;
	}
	
	return -1;
}


int32_t ComProxy::Send(cmd_frame const* pFrame, uint8_t* pResponse, size_t responseSize, bool doSync) const {	return -1; }

// wrapper to test WiflyControl
static std::list<std::string> g_TestBuffer;
static bool g_ProxySaved = false;
static bool g_ProxyConnected = false;


TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock (sock) {}
bool TelnetProxy::Close(bool doSave) const
{
	g_ProxySaved = doSave;
	g_ProxyConnected = false;
	return true;
}

bool TelnetProxy::Open(void) const
{
	return g_ProxyConnected = true;
}

bool TelnetProxy::Send(const std::string& message, const std::string& expectedResponse) const
{
	g_TestBuffer.push_back(message);
	return g_ProxyConnected;
}

bool TelnetProxy::SendString(const std::string& command, std::string value) const
{
	g_TestBuffer.push_back(command);
	g_TestBuffer.push_back(value);
	return g_ProxyConnected;
}


// Testcases
size_t ut_WiflyControl_BlReadInfo(void)
{
	TestCaseBegin();
	WiflyControl testctrl(0,0);
	BlInfo mInfo;
	testctrl.BlReadInfo(mInfo);
	CHECK(mInfo.familyId == 4);
	CHECK(mInfo.versionMajor == 1);
	CHECK(mInfo.versionMinor == 5);
	CHECK(mInfo.zero == 0);
	CHECK(mInfo.sizeHigh == 3);
	CHECK(mInfo.sizeLow == 0);
	CHECK(mInfo.startU == 0);
	CHECK(mInfo.startHigh == 253);
	CHECK(mInfo.startLow == 0);
	CHECK(mInfo.cmdmaskHigh == 255);
	//CHECK(mInfo.cmdmaskLow == 0x84);
	TestCaseEnd();
}

size_t ut_WiflyControl_BlFlashErase(void)
{
	TestCaseBegin();
	WiflyControl testctrl(0,0);
	WiflyControlException *pEx;
	pEx = NULL;
	try
	{
		testctrl.BlFlashErase();
	} catch (WiflyControlException &e) {
		pEx = &e;
	}
	CHECK(pEx == NULL);
	TestCaseEnd();
}

size_t ut_WiflyControl_BlFlashRead(void)
{
	TestCaseBegin();
	srand(time(NULL));
	
	/* fill gloabal random data pool */
	for(unsigned int i = 0; i < sizeof(g_FlashRndDataPool); i++)
		g_FlashRndDataPool[i] = (uint8_t) rand() % 255;
	
	WiflyControl testctrl(0,0);
	
	uint8_t rcvFlashData[FLASH_SIZE];
	
	size_t rcvBytes = testctrl.BlReadFlash(rcvFlashData, 0, FLASH_SIZE);
	
	CHECK(0 == memcmp(g_FlashRndDataPool, rcvFlashData, rcvBytes));	
	TestCaseEnd();
}

size_t ut_WiflyControl_ConfSetDefaults(void)
{
	TestCaseBegin();
	static const std::string commands[] = {
		"set broadcast interval 1\r\n",   // to support fast broadcast recognition
		"set uart baud 115200\r\n",       // PIC uart parameter
		"set uart flow 0\r\n",            // PIC uart parameter
		"set uart mode 0\r\n",            // PIC uart parameter
		"set wlan rate 0\r\n",            // slowest datarate but highest range
		"set comm remote 0\r\n",		  // Disable *Hello* string
		"set comm open 0\r\n",			  // Disable *OPEN* string
		"set comm close 0\r\n",			  // Disable *CLOS* string
		"set opt deviceid Wifly_Light\r\n",// Set deviceid which appears in broadcastmsg to "Wifly_Light"
		"set wlan tx 12\r\n",			  // Set the Wi-Fi transmit power to maximum
	};
	static const size_t numCommands = sizeof(commands) / sizeof(commands[0]);

	WiflyControl testee(0, 0);

	g_TestBuffer.clear();
	CHECK(testee.ConfSetDefaults());
	CHECK(!g_ProxyConnected);
	CHECK(g_ProxySaved);
	CHECK(numCommands == g_TestBuffer.size());
	for(size_t i = 0; i < numCommands; i++)
	{
		CHECK(0 == commands[i].compare(g_TestBuffer.front()));
		g_TestBuffer.pop_front();
		
	}
	TestCaseEnd();
}

size_t ut_WiflyControl_ConfSetWlan(void)
{
	TestCaseBegin();
	static const std::string phraseBase("12345678911234567892123456789312345678941234567895123456789612");
	static const std::string phraseContainsNonAlNum(phraseBase + "\x1f");
	static const std::string phrase(phraseBase + "3");
	static const std::string phraseToLong(phrase + " ");
	static const std::string ssid      ("12345678911234567892123456789312");
	static const std::string ssidToLong(ssid + " ");
	WiflyControl testee(0, 0);
	
	// passphrase to short
	CHECK(!testee.ConfSetWlan("", ssid));

	// passphrase to long
	CHECK(!testee.ConfSetWlan(phraseToLong, ssid));

	// passphrase contains not only alphanumeric characters
	CHECK(!testee.ConfSetWlan(phraseContainsNonAlNum, ssid));

	// ssid to short
	CHECK(!testee.ConfSetWlan(phrase, ""));

	// ssid to long
	CHECK(!testee.ConfSetWlan(phrase, ssidToLong));

	// valid passphrase and ssid
	CHECK(testee.ConfSetWlan(phrase, ssid));
	
	TestCaseEnd();
}

int main (int argc, const char* argv[])
{
	UnitTestMainBegin();
	RunTest(true, ut_WiflyControl_ConfSetDefaults);
	RunTest(true, ut_WiflyControl_ConfSetWlan);
	RunTest(true, ut_WiflyControl_BlReadInfo);
	RunTest(true, ut_WiflyControl_BlFlashErase);
	RunTest(true, ut_WiflyControl_BlFlashRead);
	UnitTestMainEnd();
}

