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
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include "intelhexclass.h"

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
: m_Proxy(useTcp ? (reinterpret_cast<const ClientSocket*>(new TcpSocket(pAddr, port))) : (reinterpret_cast<const ClientSocket*>(new UdpSocket(pAddr, port))))
{
	//TODO remove length
	mCmdFrame.length = (uns8)sizeof(struct cmd_set_color) + 2;

}

void WiflyControl::AddColor(unsigned long addr, unsigned long rgba, unsigned char hour, unsigned char minute, unsigned char second)
{
	mCmdFrame.led.cmd = ADD_COLOR;
	SetAddrRgb(mCmdFrame.led.data.add_color, addr, rgba);
	mCmdFrame.led.data.add_color.hour = hour;
	mCmdFrame.led.data.add_color.minute = minute;
	mCmdFrame.led.data.add_color.second = second;

	int bytesRead = FwSend(&mCmdFrame, sizeof(struct cmd_add_color));
	cout << __FUNCTION__ << ": We got " << bytesRead << " bytes response" << endl;
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
	while(address > FLASH_ERASE_BLOCKSIZE * FLASH_ERASE_BLOCKS)
	{
		// force SYNC only for erase command
		bytesRead = BlFlashErase(&buffer[0], address, FLASH_ERASE_BLOCKS, true);
#ifdef DEBUG
		cout << __FILE__ << "::" << __FUNCTION__
		 << "(): Erase at " << hex << address << endl;
#endif
		 if((bytesRead < 1) || (0x03 != buffer[0])) 
		 {
		      cout << __FILE__ << "::" << __FUNCTION__
		      << "():Erase flash failed at address: " << hex << address << endl;
		      return FALSE; 
		 }
		 address -= FLASH_ERASE_BLOCKSIZE * FLASH_ERASE_BLOCKS;
	}
	/* now we erased everything until a part of the flash smaller than FLASH_ERASE_BLOCKS * FLASH_ERASE_BLOCKSIZE
	 * so we set our startaddress at the beginning of this block and erase */
	bytesRead = BlFlashErase(&buffer[0], FLASH_ERASE_BLOCKS * FLASH_ERASE_BLOCKSIZE -1, FLASH_ERASE_BLOCKS, true);
	if((bytesRead < 1) || (0x03 != buffer[0])) 
	{		    
		  cout << __FILE__ << "::" << __FUNCTION__
		  << "():Erase flash failed at address: " << hex << 0 << endl;
		  return FALSE; 
	}
	return TRUE;
}

size_t WiflyControl::BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize, bool doSync) const
{
	unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
	size_t bytesReceived = m_Proxy.Send(req, buffer, sizeof(buffer), doSync);

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
	if(numBytes + address > EEPROM_SIZE)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): can not performe read outside the eeprom" << endl;
		return NULL;
	}
	
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
	if(numBytes + address > FLASH_SIZE)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): can not performe read outside the flash" << endl;
		return NULL;
	}
  
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
	if(numBlocks * FLASH_ERASE_BLOCKSIZE + address > FLASH_SIZE)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): can not performe crc outside the flash" << endl;
		return false;
	}
  
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
	if(bufferLength + address > FLASH_SIZE)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): can not performe write outside the flash" << endl;
		return false;
	}
  
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

bool WiflyControl::BlWriteEeprom(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const
{
	if(bufferLength + address > EEPROM_SIZE)
	{
		cout << __FILE__ << "::" << __FUNCTION__
		<< "(): can not performe write outside the eeprom" << endl;
		return false;
	}
      
	BlEepromWriteRequest request;
	unsigned char response;
	
	size_t bytesLeft = bufferLength;
	while(bytesLeft > EEPROM_WRITE_BLOCKSIZE)
	{
		request.SetData(address, pBuffer, EEPROM_WRITE_BLOCKSIZE);

		// we expect only the 0x06 command byte as response
		if(1 != BlRead(request, &response, sizeof(response)))
		{
			return false;
		}
		
		address += EEPROM_WRITE_BLOCKSIZE;
		pBuffer += EEPROM_WRITE_BLOCKSIZE;
		bytesLeft -= EEPROM_WRITE_BLOCKSIZE;
	}

	request.SetData(address, pBuffer, bytesLeft);
	return (1 == BlRead(request, &response, sizeof(response)));
}

bool WiflyControl::BlEepromErase(void) const
{
	unsigned char buffer[EEPROM_SIZE];
	
	for(unsigned int i = 0; i < sizeof(buffer); i++)
	{
	    buffer[i] = 0xff;
	}
	return BlWriteEeprom((unsigned int)0, &buffer[0], sizeof(buffer));
	
}

bool WiflyControl::BlEnableAutostart(void) const
{
	unsigned char value = 0xff;
	
	return BlWriteEeprom((unsigned int)BL_AUTOSTART_ADDRESS, &value, sizeof(value));
}

bool WiflyControl::BlProgramFlash(const std::string& pFilename)
{
	cout << endl << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")" << endl;
	cout << "opening '" << pFilename << "' ...";

	std::ifstream hexFile;
	hexFile.open(const_cast<char*>(pFilename.c_str()), ifstream::in);
	
	if(!hexFile.good())
	{
		cout << "failed!" << endl;
		return false;
	}
	cout << "done." << endl;

	
	intelhex hexConverter;
	hexFile >> hexConverter;
	
	BlInfo info;
	if(sizeof(info) != BlReadInfo(info))
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": can not read bootloader info from traget device" << endl;
		return false;
	}
	
	/*Check if last address of programmcode is not in the bootblock */
	
	unsigned long endAddress;
	
	if(hexConverter.endAddress(&endAddress))
	{
	    if(endAddress >= (unsigned long)(info.GetAddress()))
	    {
		  cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		  << ": endaddress of program code is in bootloader area of the target device flash" << endl;
		  return false;
	    }
	}
	else
	{
	    cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
	    << ": can not read endAddress from hexConverter" << endl;
	    return false;
	}
	
	unsigned short word1, word2;
	unsigned int bootAddress;
	unsigned char resetVector[4];
	unsigned char appVector[4];
	
	/* Calculate the resetVector */
	bootAddress = (info.GetAddress() + 2) / 2;
	
	word1 = 0xEF00 | (bootAddress & 0xff);
	word2 = 0xF000 | ((bootAddress >> 8) & 0x0FFF);
	
	resetVector[0] = (unsigned char)word1;
	resetVector[1] = (unsigned char)(word1 >> 8);
	resetVector[2] = (unsigned char)word2;
	resetVector[3] = (unsigned char)(word2 >> 8);
	
	/*Put AppVektor from the beginning of hexfile at the startaddress of the bootloader */
	
	hexConverter.begin();
	if(hexConverter.currentAddress() != 0)
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": program code does not start at address 0x0000" << endl;
		return false;
	}
	
	for(unsigned int i = 0; i < sizeof(appVector); i++)
	{
		if(!hexConverter.getData(&appVector[i],(unsigned long)i))
		{
		    cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		    << ": can not read data at address 0x" << hexConverter.currentAddress() << endl;
		    return false;
		}
	}
	
	if(!BlEnableAutostart())
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": can not enable bootloader autostart!" << endl;
		return false;
	}
	
	if(!BlFlashErase())
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": can not erase target device flash!" << endl;
		return false;
	}
	
	unsigned int writeAddress = 0;
	unsigned char flashBuffer[FLASH_SIZE];
	unsigned char nextByte;
	
	if(endAddress > FLASH_SIZE)
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": endaddress of program code is outside the target device flash" << endl;
		return false;
	}
	
	/* Write the resetVector to temporary flashBuffer*/
	memcpy(&flashBuffer[0], &resetVector[0], sizeof(resetVector));
	
	writeAddress += sizeof(resetVector);
	
	for(;writeAddress <= (unsigned int)endAddress; writeAddress++)
	{
		if(hexConverter.getData(&nextByte, writeAddress))
		{
		    flashBuffer[writeAddress] = nextByte;
		}
		else
		{
		    flashBuffer[writeAddress] = 0xff;	
		} 
	}
	if(!BlWriteFlash(0, &flashBuffer[0], (size_t)endAddress+1))
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": writing program code to target device flash failed!" << endl;
		return false;
	}
	
	/* we always have to write a FLASH_WRITE Block when we wanna write to device flash,
	 * so we have to pack the appVector at the end of a Block of data */
	
	unsigned char appVecBuf[FLASH_WRITE_BLOCKSIZE];
	
	for(int i = 0; i < FLASH_WRITE_BLOCKSIZE; i++)
	{
		appVecBuf[i] = 0xff;
	}
	
	memcpy(&appVecBuf[sizeof(appVecBuf) - sizeof(appVector)], &appVector[0], sizeof(appVector)); 
	
	if(!BlWriteFlash(info.GetAddress() - FLASH_WRITE_BLOCKSIZE, &appVecBuf[0], FLASH_WRITE_BLOCKSIZE))
	{
		cout << __FILE__ << "::" << __FUNCTION__ << "(" << pFilename << ")"
		<< ": writing application startvector to target device flash failed!" << endl;
		return false;
	}
	
	return true;
}

void WiflyControl::ClearScript(void)
{
	mCmdFrame.led.cmd = CLEAR_SCRIPT;
	FwSend(&mCmdFrame, 0);
}

int WiflyControl::FwSend(struct cmd_frame* pFrame, size_t length) const
{
	pFrame->length = length + 2; //add cmd and length byte
	return m_Proxy.Send(&mCmdFrame, NULL, 0, false);
}

void WiflyControl::FwTest(void)
{
	int doRun = 5;
  
	static const unsigned long RED   = 0xFF000000;
	static const unsigned long GREEN = 0x00FF0000;
	static const unsigned long BLUE  = 0x0000FF00;
	static const unsigned long WHITE = 0xFFFFFF00;
	static const unsigned long BLACK = 0x00000000;

	while(doRun > 0)
	{
		SetColor(0xFFFFFFFFLU, BLACK);
		SetColor(0xFF000000LU, RED);   sleep(1);
		SetColor(0x00FF0000LU, GREEN); sleep(1);
		SetColor(0x0000FF00LU, BLUE);  sleep(1);
		SetColor(0x000000FFLU, WHITE); sleep(1);
		SetFade(0xFFFFFFFFLU, 0x00000000LU, (unsigned short)5000, false);
		sleep(20);
		SetFade(0x000000FFLU, RED,  4000, true);
		SetFade(0x0000FF00LU, GREEN,1000, true);
		SetFade(0x00FF0000LU, BLUE, 2000, true);
		SetFade(0xFF000000LU, WHITE,3000, false);
		sleep(20);
		doRun--;
	}
}

void WiflyControl::StartBl(void)
{
	mCmdFrame.led.cmd = START_BL;
	
	int bytesRead = FwSend(&mCmdFrame, 0);
	cout << __FUNCTION__ << ": We got " << bytesRead << " bytes response" << endl;	
}

void WiflyControl::SetColor(unsigned long addr, unsigned long rgba)
{
	mCmdFrame.led.cmd = SET_COLOR;
	SetAddrRgb(mCmdFrame.led.data.set_color, addr, rgba);

	int bytesRead = FwSend(&mCmdFrame, sizeof(struct cmd_set_color));
	cout << __FUNCTION__ << ": We got " << bytesRead << " bytes response" << endl;
}

void WiflyControl::SetColor(string& addr, string& rgba)
{
	SetColor(ToRGBA(addr), ToRGBA(rgba) << 8);
}

void WiflyControl::SetFade(unsigned long addr, unsigned long rgba, unsigned short fadeTmms, bool parallelFade)
{
	mCmdFrame.led.cmd = SET_FADE;
	SetAddrRgb(mCmdFrame.led.data.set_fade, addr, rgba);
	mCmdFrame.led.data.set_fade.fadeTmms = htons((unsigned short)(fadeTmms / 10));
	mCmdFrame.led.data.set_fade.parallelFade = parallelFade;

	int bytesRead = FwSend(&mCmdFrame, sizeof(cmd_set_fade));
	cout << __FUNCTION__ << ": We got " << bytesRead << " bytes response" << endl;
}

void WiflyControl::SetFade(string& addr, string& rgba, unsigned short fadeTmms, bool parallelFade)
{
	SetFade(ToRGBA(addr), ToRGBA(rgba) << 8, fadeTmms, parallelFade);
}

unsigned long WiflyControl::ToRGBA(string& s) const
{
	if(s.length() > 8) return 0;

	// use a stringstream to convert hex ascii string into machine bits
	unsigned long rgba;
	stringstream converter;
	converter << hex << s;
	converter >> rgba;
	return rgba;
}

