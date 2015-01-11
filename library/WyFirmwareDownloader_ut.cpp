/*
 Copyright (C) 2015 Nils Weiss, Patrick Bruenn.

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

#include "unittest.h"
#include "WyFirmwareDownloader.h"
#include <algorithm>
#include <limits>
#include <time.h>
#include <unistd.h>
#include "bootloader.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <string>


/**************** includes, classes and functions for wrapping ****************/
#include "ClientSocket.h"

using namespace WyLight;

static const uint32_t __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) : mSock(0), mSockAddr(addr, port) {}
ClientSocket::~ClientSocket(void) {}

uint8_t g_TestSocketRecvBuffer[300000];
size_t g_TestSocketRecvBufferPos = 0;
size_t g_TestSocketRecvBufferSize = 0;
uint8_t g_TestSocketSendBuffer[300000];
size_t g_TestSocketSendBufferPos = 0;
timespec g_TestSocketSendDelay;

size_t g_TestFileLength = 0;

void SetDelay(timeval& delay)
{
	g_TestSocketSendDelay.tv_sec = delay.tv_sec;
	g_TestSocketSendDelay.tv_nsec = delay.tv_usec * 1000;
}
TcpSocket::TcpSocket(uint32_t Addr, uint16_t port) throw (ConnectionLost, FatalError) {
    
}

TcpSocket::TcpSocket(int listenSocket, const struct timespec *timeout) throw (ConnectionLost, FatalError)
{
	g_TestSocketSendDelay.tv_sec = 0;
	g_TestSocketSendDelay.tv_nsec = 0;
}

ClientSocket::ClientSocket()
: mSock(-1), mSockAddr(0, 0)
{
	
}

size_t TcpSocket::Recv(uint8_t *pBuffer, size_t length, timeval *timeout) const throw (FatalError)
{
	nanosleep(&g_TestSocketSendDelay, NULL);
	//Trace(ZONE_VERBOSE, "%p %zu of %zu wait for %zu\n", pBuffer, g_TestSocketRecvBufferPos, g_TestSocketRecvBufferSize, length);
	if(g_TestSocketRecvBufferPos < g_TestSocketRecvBufferSize) {
        size_t sendLength = std::min(g_TestSocketRecvBufferSize - g_TestSocketRecvBufferPos, length);
        memcpy(pBuffer, &g_TestSocketRecvBuffer[g_TestSocketRecvBufferPos], sendLength);
        g_TestSocketRecvBufferPos+=sendLength;
		return sendLength;
	}
	return 0;
}

size_t TcpSocket::Send(const uint8_t *frame, size_t length) const
{
    memcpy(&g_TestSocketSendBuffer[g_TestSocketSendBufferPos], frame, length);
	g_TestSocketSendBufferPos += length;
    
    if (g_TestSocketSendBufferPos == g_TestFileLength) {
        g_TestSocketRecvBuffer[g_TestSocketRecvBufferPos] = DONE_RESPONSE;
        g_TestSocketRecvBuffer[g_TestSocketRecvBufferPos+1] = EOF;
        g_TestSocketRecvBufferSize += 2;
    }
    
	return length;
}

/******************************* test functions *******************************/
size_t ut_WyFirmwareDownloader_loadFirmware(void)
{
	TestCaseBegin();
    
    const uint32_t BL_VERSION = htonl(BOOTLOADER_VERSION);
    char welcome[] = WELCOME_RESPONSE;
    
    memcpy(welcome, &BL_VERSION, sizeof(uint32_t));
    memcpy(g_TestSocketRecvBuffer, welcome, sizeof(welcome));
    g_TestSocketRecvBufferSize += sizeof(welcome);
    g_TestSocketRecvBufferPos = 0;
    
    std::fstream src("/Users/nweiss/WyLight/unit_test_data/firmware_out.bin", std::stringstream::in | std::stringstream::binary);
    CHECK((bool)src);
    if (!src) {
        TestCaseEnd();
    }
    src.seekg(0, src.end);
    const size_t length = src.tellg();
    src.seekg(0, src.beg);
    
    std::unique_ptr<char[]> buffer(new char[length]());
    src.read((char *)buffer.get(), length);
    src.close();
    
    g_TestFileLength = length;

    CHECK(0 == FirmwareDownloader(0, 0).loadFirmware("/Users/nweiss/WyLight/unit_test_data/firmware_in.bin"));
    CHECK(0 == memcmp(buffer.get(), g_TestSocketSendBuffer, length-1));
    
    TestCaseEnd();
}

int main (int argc, const char *argv[])
{
	UnitTestMainBegin();
	RunTest(true,  ut_WyFirmwareDownloader_loadFirmware);
    UnitTestMainEnd();
}

