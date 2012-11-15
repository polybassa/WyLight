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
#include "crc.h"

#include <iostream>
#include <sstream>
#include <cassert>
#include <stddef.h>

using namespace std;

/**
 * Macro to reduce code redundancy, while converting two 32 bit values into
 * an address array and red, green, blue values. 
 */
#define SetAddrRgb(REF, ADDRESS, RGBA) { \
	REF.addr[0] = (ADDRESS & 0xff000000) >> 24; \
	REF.addr[1] = (ADDRESS & 0x00ff0000) >> 16; \
	REF.addr[2] = (ADDRESS & 0x0000ff00) >> 8; \
	REF.addr[3] = (ADDRESS & 0x000000ff); \
	REF.red = (RGBA & 0xff000000) >> 24; \
	REF.green = (RGBA & 0x00ff0000) >> 16; \
	REF.blue = (RGBA & 0x0000ff00) >> 8; \
}

WiflyControl::WiflyControl(const char* pAddr, short port, bool useTcp)
{
	mCmdFrame.stx = STX;
	mCmdFrame.length = (uns8)sizeof(struct cmd_set_color) + 2;

	if(useTcp)
	{
		mSock = new TcpSocket(pAddr, port);
	}
	else
	{
		mSock = new UdpSocket(pAddr, port);
	}
	assert(mSock);
}

void WiflyControl::AddColor(unsigned long addr, unsigned long rgba, unsigned char hour, unsigned char minute, unsigned char second)
{
	mCmdFrame.length = sizeof(struct cmd_add_color) + 3;
	mCmdFrame.led.cmd = ADD_COLOR;
	SetAddrRgb(mCmdFrame.led.data.add_color, addr, rgba);
	mCmdFrame.led.data.add_color.hour = hour;
	mCmdFrame.led.data.add_color.minute = minute;
	mCmdFrame.led.data.add_color.second = second;
	FwSend(&mCmdFrame);
}

void WiflyControl::AddColor(string& addr, string& rgba, unsigned char hour, unsigned char minute, unsigned char second)
{
	AddColor(ToRGBA(addr), ToRGBA(rgba) << 8, hour, minute, second);
}

size_t WiflyControl::BlFlashErase(unsigned char* pBuffer, unsigned int endAddress, const size_t numPages, bool doSync) const
{
	BlFlashEraseRequest request(endAddress, numPages);
	// we expect only one byte as response, the command code 0x03
	return BlRead(request, pBuffer, 1, doSync);
}

bool WiflyControl::BlFlashErase(void) const
{
	BlInfo info;
	if(sizeof(info) != BlReadInfo(info))
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): Erase flash failed, couldn't determine bootloader location" << endl;
		return FALSE;
	}
	const unsigned int firstAddress = info.GetAddress() - 1;
	unsigned int address = firstAddress;
	
	unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
	size_t bytesRead;
	while(address > FLASH_ERASE_BLOCKSIZE)
	{
		// force SYNC only for erase command
		 bytesRead = BlFlashErase(buffer, address, FLASH_ERASE_BLOCKSIZE, (firstAddress == address));
		 if((bytesRead != 1) || (0x03 != buffer[0])) 
		 {
			cout << __FILE__ << "::" << __FUNCTION__
			<< "():Erase flash failed at address: " << hex << address << endl;
			return FALSE; 
		 }
		 address -= FLASH_ERASE_BLOCKSIZE;
	}
	bytesRead = BlFlashErase(buffer, 0, address, false);
	if((bytesRead != 1) || (0x03 != buffer[0])) 
	{		    
		  cout << __FILE__ << "::" << __FUNCTION__
		  << "():Erase flash failed at address: " << hex << 0 << endl;
		  return FALSE; 
	}
	return TRUE;
}

size_t WiflyControl::BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize, bool doSync) const
{
	BlProxy proxy(mSock);
	unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
	size_t bytesReceived = proxy.Send(req, buffer, sizeof(buffer), doSync);

	cout << __FILE__ << "::" << __FUNCTION__ << "(): " << bytesReceived << ":" << sizeof(BlInfo) << endl;
	if(responseSize == bytesReceived)
	{
		memcpy(pResponse, buffer, responseSize);
		return responseSize;
	}
	return 0;
}

size_t WiflyControl::BlReadEeprom(unsigned char* pBuffer, unsigned int address, size_t numBytes) const
{
	size_t bytesRead;
	size_t sumBytesRead = 0;
	BlEepromReadRequest readRequest;
	while(numBytes > EEPROM_READ_BLOCKSIZE)
	{
		readRequest.SetAddressNumBytes(address, EEPROM_READ_BLOCKSIZE);
		bytesRead = BlRead(readRequest, pBuffer, EEPROM_READ_BLOCKSIZE);
		sumBytesRead += bytesRead;
		if(EEPROM_READ_BLOCKSIZE != bytesRead)
		{
			cout << __FILE__ << "::" << __FUNCTION__
			<< "(): only " << bytesRead << " bytes read not " << EEPROM_READ_BLOCKSIZE << endl;
			return sumBytesRead;
		}
		address += EEPROM_READ_BLOCKSIZE;
		numBytes -= EEPROM_READ_BLOCKSIZE;
		pBuffer += EEPROM_READ_BLOCKSIZE;
	}
	readRequest.SetAddressNumBytes(address, numBytes);
	bytesRead = BlRead(readRequest, pBuffer, numBytes);
	sumBytesRead += bytesRead;
	if(numBytes != bytesRead)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): only " << bytesRead << " bytes read not " << numBytes << endl;
	}
	return sumBytesRead;
}

size_t WiflyControl::BlReadFlash(unsigned char* pBuffer, unsigned int address, size_t numBytes) const
{
	size_t bytesRead;
	size_t sumBytesRead = 0;
	BlFlashReadRequest readRequest;
	while(numBytes > FLASH_READ_BLOCKSIZE)
	{
		readRequest.SetAddressNumBytes(address, FLASH_READ_BLOCKSIZE);
		bytesRead = BlRead(readRequest, pBuffer, FLASH_READ_BLOCKSIZE);
		sumBytesRead += bytesRead;
		if(FLASH_READ_BLOCKSIZE != bytesRead)
		{
			cout << __FILE__ << "::" << __FUNCTION__
			<< "(): only " << bytesRead << " bytes read not " << FLASH_READ_BLOCKSIZE << endl;
			return sumBytesRead;
		}
		address += FLASH_READ_BLOCKSIZE;
		numBytes -= FLASH_READ_BLOCKSIZE;
		pBuffer += FLASH_READ_BLOCKSIZE;
	}

	readRequest.SetAddressNumBytes(address, numBytes);
	bytesRead = BlRead(readRequest, pBuffer, numBytes);
	sumBytesRead += bytesRead;
	if(numBytes != bytesRead)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): only " << bytesRead << " bytes read not " << numBytes << endl;
	}
	return sumBytesRead;
}

size_t WiflyControl::BlReadCrcFlash(unsigned char* pBuffer, unsigned int address, size_t numBlocks) const
{
	size_t bytesRead;
	size_t sumBytesRead = 0;
	while(numBlocks > FLASH_CRC_BLOCKSIZE)
	{
		BlFlashCrc16Request readRequest(address, FLASH_CRC_BLOCKSIZE);
		bytesRead = BlRead(readRequest, pBuffer, FLASH_CRC_BLOCKSIZE * 2);
		sumBytesRead += bytesRead;
		if(FLASH_CRC_BLOCKSIZE * 2 != bytesRead)
		{
			cout << __FILE__ << "::" << __FUNCTION__
			<< "(): only " << bytesRead << " bytes read not " << FLASH_CRC_BLOCKSIZE * 2 << endl;
			return sumBytesRead;
		}
		address += (FLASH_CRC_BLOCKSIZE * FLASH_ERASE_BLOCKSIZE);
		numBlocks -= FLASH_CRC_BLOCKSIZE;
		pBuffer += FLASH_CRC_BLOCKSIZE * 2;
	}

	BlFlashCrc16Request readRequest(address, numBlocks);
	bytesRead = BlRead(readRequest, pBuffer, numBlocks * 2);
	sumBytesRead += bytesRead;
	if(numBlocks * 2 != bytesRead)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): only " << bytesRead << " bytes read not " << numBlocks * 2 << endl;
	}
	return sumBytesRead;
}

size_t WiflyControl::BlReadInfo(BlInfo& blInfo) const
{
	BlInfoRequest request;
	return BlRead(request, reinterpret_cast<unsigned char*>(&blInfo), sizeof(BlInfo));
}

bool WiflyControl::BlRunApp(void) const
{
	BlRunAppRequest request;
	unsigned char buffer[32];
	size_t bytesRead = BlRead(request, buffer, sizeof(buffer));

	/* we expect a "RDY" as lifesign of the application */
	if((3 == bytesRead) && (0 != memcmp("RDY", buffer, bytesRead)))
	{
		return true;
	}
	cout << __FILE__ << "::" << __FUNCTION__
		<< "(): " << bytesRead << " bytes read" << endl;
	return false;
}

bool WiflyControl::BlWriteFlash(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const
{
	BlFlashWriteRequest request;
	unsigned char response;
	
	size_t bytesLeft = bufferLength;
	while(bytesLeft > FLASH_WRITE_BLOCKSIZE)
	{
		request.SetData(address, pBuffer, FLASH_WRITE_BLOCKSIZE);

		// we expect only the 0x04 command byte as response
		if(1 != BlRead(request, &response, sizeof(response)))
		{
			return false;
		}
		
		address += FLASH_WRITE_BLOCKSIZE;
		pBuffer += FLASH_WRITE_BLOCKSIZE;
		bytesLeft -= FLASH_WRITE_BLOCKSIZE;
	}

	request.SetData(address, pBuffer, bytesLeft);
	return (1 == BlRead(request, &response, sizeof(response)));
}

void WiflyControl::ClearScript(void)
{
	mCmdFrame.length = 3;
	mCmdFrame.led.cmd = CLEAR_SCRIPT;
	FwSend(&mCmdFrame);
}

bool WiflyControl::FwSend(const struct cmd_frame* pFrame) const
{
	unsigned char buffer[FW_MAX_MESSAGE_LENGTH];
	unsigned char* pCrcHigh = buffer + pFrame->length;
	unsigned char* pCrcLow = pCrcHigh + 1;
	const size_t numBytes = pFrame->length + 2;
	memcpy(buffer, pFrame, pFrame->length);
	Crc_BuildCrc(reinterpret_cast<const unsigned char*>(pFrame), pFrame->length, pCrcHigh, pCrcLow);
	int bytesWritten = mSock->Send(buffer, numBytes);

#ifdef DEBUG
	cout << "Send " << bytesWritten << " bytes: " << endl;
	for(int i = 0; i < bytesWritten; i++)
	{
		cout << hex << (int)(buffer[i]) << " ";
	}
	cout << endl;
#if 0
	bytesWritten = mSock->Recv(buffer, sizeof(buffer), 2000);
	cout << "Received " << bytesWritten << " bytes: " << endl;
	for(int i = 0; i < bytesWritten; i++)
	{
		cout << (buffer[i]) << " ";
	}
	cout << endl;
#endif
#endif
	return (0 <= bytesWritten) && (numBytes == static_cast<size_t>(bytesWritten));
}

void WiflyControl::StartBl(void)
{
	mCmdFrame.led.cmd = START_BL;
	int bytesWritten = mSock->Send(reinterpret_cast<unsigned char*>(&mCmdFrame), sizeof(mCmdFrame));
	assert(sizeof(mCmdFrame) == bytesWritten);
}

void WiflyControl::SetColor(unsigned long addr, unsigned long rgba)
{
	mCmdFrame.length = sizeof(struct cmd_set_color) + 3;
	mCmdFrame.led.cmd = SET_COLOR;
	SetAddrRgb(mCmdFrame.led.data.set_color, addr, rgba);
	FwSend(&mCmdFrame);
}

void WiflyControl::SetColor(string& addr, string& rgba)
{
	SetColor(ToRGBA(addr), ToRGBA(rgba) << 8);
}

void WiflyControl::SetFade(unsigned long addr, unsigned long rgba, unsigned short fadeTmms)
{
	mCmdFrame.length = sizeof(cmd_set_fade) + 3;
	mCmdFrame.led.cmd = SET_FADE;
	SetAddrRgb(mCmdFrame.led.data.set_fade, addr, rgba);
	mCmdFrame.led.data.set_fade.fadeTmms = htons(fadeTmms);
	FwSend(&mCmdFrame);
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

