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

#include "WiflyControl.h"
#include "crc.h"
#include "trace.h"
#include "MaskBuffer.h"
#include "Version.h"

#include <algorithm>
#include <sstream>
#include <cctype>
#include <stddef.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#include "intelhexclass.h"
#include "WiflyColor.h"
#include <thread>
#include <chrono>

using std::cout;
using std::ifstream;
using std::hex;

namespace WyLight {

	static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

	const std::string Control::LEDS_ALL {"ffffffff"};
	const std::list<std::string> Control::RN171_DEFAULT_PARAMETERS = {
		"set broadcast interval 0x1\r\n",    // to support fast broadcast recognition
		"set comm close 0\r\n",            // Disable *CLOS* string
		"set comm open 0\r\n",             // Disable *OPEN* string
		"set comm remote 0\r\n",           // Disable *Hello* string
		"set dns name rn.microchip.com\r\n",    // set dns of updateserver
		"set ip flags 0x6\r\n",                    // if the module loses the accesspoint connection, the connection is closed
		"set ip dhcp 1\r\n",               // enable DHCP client
		//		"set ftp address 169.254.7.57\r\n",// configure localhost as ftp server in ad-hoc connection
		"set ftp pass Pass123\r\n",        // configure ftp password
		"set ftp user roving\r\n",         // configure ftp username
		"set opt deviceid Wifly_Light\r\n", // Set deviceid which appears in broadcastmsg to "Wifly_Light"
		"set uart baud 115200\r\n",        // PIC uart parameter
		"set uart flow 0\r\n",             // PIC uart parameter
		"set uart mode 0\r\n",             // PIC uart parameter
		"set wlan channel 0\r\n",                  // Set the wlan channel to 0 to perform an automatic scan for a free channel
		"set wlan auth 4\r\n",             // use WPA2 protection
		"set wlan join 1\r\n",             // scan for ap and auto join
		"set wlan rate 0\r\n",             // slowest datarate but highest range
		"set wlan tx 12\r\n",              // Set the Wi-Fi transmit power to maximum
		"set ip p 11\r\n",                                 // Enable UDP, TCP_CLIENT and TCP Protocol
		//"set sys launch_string wps_app"	   // Choose Wps mode
	};
	const std::list<std::string> Control::RN171_BASIC_PARAMETERS = {
		"set broadcast interval 0x1\r\n",    // to support fast broadcast recognition
		"set comm close 0\r\n",            // Disable *CLOS* string
		"set comm open 0\r\n",             // Disable *OPEN* string
		"set comm remote 0\r\n",           // Disable *Hello* string
		"set dns name rn.microchip.com\r\n",    // set dns of updateserver
		"set ip flags 0x6\r\n",                    // if the module loses the accesspoint connection, the connection is closed
		"set ip dhcp 1\r\n",               // enable DHCP client
		"set ftp pass Pass123\r\n",        // configure ftp password
		"set ftp user roving\r\n",         // configure ftp username
		"set uart baud 115200\r\n",        // PIC uart parameter
		"set uart flow 0\r\n",             // PIC uart parameter
		"set uart mode 0\r\n",             // PIC uart parameter
		"set wlan channel 0\r\n",                  // Set the wlan channel to 0 to perform an automatic scan for a free channel
		"set wlan auth 4\r\n",             // use WPA2 protection
		"set wlan join 1\r\n",             // scan for ap and auto join
		"set wlan rate 0\r\n",             // slowest datarate but highest range
		"set wlan tx 12\r\n",              // Set the Wi-Fi transmit power to maximum
		"set ip p 11\r\n",                                 // Enable UDP, TCP_CLIENT and TCP Protocol
		//"set sys launch_string wps_app"	   // Choose Wps mode
	};
	const std::list<std::string> Control::RN171_SOFT_AP_DEFAULT_PARAMETERS = {
		"set broadcast interval 1\r\n",    // to support fast broadcast recognition
		"set comm close 0\r\n",            // Disable *CLOS* string
		"set comm open 0\r\n",             // Disable *OPEN* string
		"set comm remote 0\r\n",           // Disable *Hello* string
		//		"set dns name rn.microchip.com\r\n",	// set dns of updateserver
		"set ip dhcp 4\r\n",               // enable DHCP server
		"set ftp address 169.254.7.57\r\n", // configure localhost as ftp server in ad-hoc connection
		"set ftp pass Pass123\r\n",        // configure ftp password
		"set ftp user roving\r\n",         // configure ftp username
		"set uart baud 115200\r\n",        // PIC uart parameter
		"set uart flow 0\r\n",             // PIC uart parameter
		"set uart mode 0\r\n",             // PIC uart parameter
		"set wlan join 7\r\n",             // enable AP mode
		"set wlan rate 0\r\n",             // slowest datarate but highest range
		"set wlan tx 12\r\n",              // Set the Wi-Fi transmit power to maximum
		"set wlan channel 1\r\n",                  // Set the wlan channel to 0 to perform an automatic scan for a free channel
		"set ip a 1.2.3.4\r\n",            // Set ip address for accespoint
		"set ip g 0.0.0.0\r\n",                    // Set gateway address to zero
		"set ip n 255.255.255.0\r\n",      // Set netmask for accespoint
		"set ip p 11\r\n",                                 // Enable UDP, TCP_CLIENT and TCP Protocol
	};

	const size_t FwCmdScript::INDENTATION_MAX;
	const char FwCmdScript::INDENTATION_CHARACTER;

	const std::string FwCmdSetFade::TOKEN("fade");
	const std::string FwCmdSetGradient::TOKEN("gradient");
	const std::string FwCmdLoopOn::TOKEN("loop");
	const std::string FwCmdLoopOff::TOKEN("loop_off");
	const std::string FwCmdWait::TOKEN("wait");

	Control::Control(uint32_t addr, uint16_t port) : mTcpSock(addr, port), mUdpSock(addr, port, false, 0), mProxy(mTcpSock), mTelnet(mTcpSock) {}

	size_t Control::GetTargetMode(void) const throw(FatalError)
	{
		return mProxy.SyncWithTarget();
	}

	/** ------------------------- BOOTLOADER METHODES ------------------------- **/
	void Control::BlEnableAutostart(void) const throw(ConnectionTimeout, FatalError)
	{
		static const uint8_t value = 0xff;
		try {
			BlWriteEeprom((uint32_t)BL_AUTOSTART_ADDRESS, &value, sizeof(value));
		} catch(InvalidParameter &e) {
			throw new FatalError(std::string(e.what()) + "\n Internal failure in BlWriteEeprom(BL_AUTOSTART_ADDRESS....)");
		} catch(std::exception &e) {
			throw e;
		}
	}

	void Control::BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError)
	{
		uint8_t buffer[EEPROM_SIZE];
		std::fill_n(buffer, EEPROM_SIZE, 0xff);
			BlWriteEeprom((uint32_t)0, buffer, sizeof(buffer));
	}

	void Control::BlEraseFlash(void) const throw(ConnectionTimeout, FatalError)
	{
		BlInfo info;
		BlReadInfo(info);

		const unsigned int firstAddress = info.GetAddress() - 1;
		unsigned int address = firstAddress;

		while(address > FLASH_ERASE_BLOCKSIZE * FLASH_ERASE_BLOCKS)
		{
			BlEraseFlashArea(address, FLASH_ERASE_BLOCKS);
			address -= FLASH_ERASE_BLOCKSIZE * FLASH_ERASE_BLOCKS;
		}
		/* now we erased everything until a part of the flash smaller than FLASH_ERASE_BLOCKS * FLASH_ERASE_BLOCKSIZE
		 * so we set our startaddress at the beginning of this block and erase */
			BlEraseFlashArea(FLASH_ERASE_BLOCKS * FLASH_ERASE_BLOCKSIZE - 1, FLASH_ERASE_BLOCKS);
	}

	void Control::BlEraseFlashArea(const uint32_t endAddress, const uint8_t numPages) const throw(ConnectionTimeout, FatalError)
	{
		unsigned char response;
		BlFlashEraseRequest request(endAddress, numPages);

		// always sync for flash erase
		BlRead(request, &response, sizeof(response), true);

		// we expect only one byte as response, the command code 0x03
		if(0x03 != response) {
			throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
		}
	}

	size_t Control::BlRead(const BlRequest& req, unsigned char *pResponse, const size_t responseSize, bool doSync) const throw(ConnectionTimeout, FatalError)
	{
		unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
		size_t bytesReceived = mProxy.Send(req, buffer, sizeof(buffer), doSync);
		Trace(ZONE_INFO, " %zd:%ld \n", bytesReceived, sizeof(BlInfo));
		TraceBuffer(ZONE_VERBOSE, (uint8_t *)&buffer[0], bytesReceived, "0x%02x, ", "Message: ");
		if(responseSize != bytesReceived) {
			throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": Too many retries");
		}
		memcpy(pResponse, buffer, responseSize);
		return responseSize;
	}

	void Control::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		out.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
		uint8_t buffer[5096];
		size_t bytesRead = BlReadCrcFlash(buffer, address, numBytes);
		for(size_t i = 0; i < bytesRead; i++) {
			out << buffer[i];
		}
	}

	size_t Control::BlReadCrcFlash(unsigned char *pBuffer, unsigned int address, uint16_t numBlocks) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		if(numBlocks * FLASH_ERASE_BLOCKSIZE + address > FLASH_SIZE) {
			throw InvalidParameter(std::string(__FILE__) + ':' + __FUNCTION__ + ": address or numBlocks out of range");
		}

		size_t bytesRead;
		size_t sumBytesRead = 0;
		while(numBlocks > FLASH_CRC_BLOCKSIZE)
		{
			BlFlashCrc16Request readRequest(address, FLASH_CRC_BLOCKSIZE);
			bytesRead = BlRead(readRequest, pBuffer, FLASH_CRC_BLOCKSIZE * 2);
			sumBytesRead += bytesRead;
			address += (FLASH_CRC_BLOCKSIZE * FLASH_ERASE_BLOCKSIZE);
			numBlocks -= FLASH_CRC_BLOCKSIZE;
			pBuffer += FLASH_CRC_BLOCKSIZE * 2;
		}

		BlFlashCrc16Request readRequest(address, numBlocks);
		bytesRead = BlRead(readRequest, pBuffer, numBlocks * 2);
		sumBytesRead += bytesRead;
		return sumBytesRead;
	}

	void Control::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		uint8_t buffer[EEPROM_SIZE];
		size_t bytesRead = BlReadEeprom(buffer, address, numBytes);
		out.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
		for(size_t i = 0; i < bytesRead; i++) {
			out.put(buffer[i]);
		}
	}

	size_t Control::BlReadEeprom(uint8_t *pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		if(numBytes + address > EEPROM_SIZE) {
			throw InvalidParameter(std::string(__FILE__) + ':' + __FUNCTION__ + ": address or numBytes out of range");
		}

		size_t bytesRead;
		size_t sumBytesRead = 0;
		BlEepromReadRequest readRequest;
		while(numBytes > EEPROM_READ_BLOCKSIZE)
		{
			readRequest.SetAddressNumBytes(address, EEPROM_READ_BLOCKSIZE);
			bytesRead = BlRead(readRequest, pBuffer, EEPROM_READ_BLOCKSIZE);
			sumBytesRead += bytesRead;
			address += EEPROM_READ_BLOCKSIZE;
			numBytes -= EEPROM_READ_BLOCKSIZE;
			pBuffer += EEPROM_READ_BLOCKSIZE;
		}
		readRequest.SetAddressNumBytes(address, numBytes);
		bytesRead = BlRead(readRequest, pBuffer, numBytes);
		sumBytesRead += bytesRead;
		return sumBytesRead;
	}

	void Control::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		out.flags ( std::ios::right | std::ios::hex | std::ios::showbase );
		uint8_t buffer[FLASH_SIZE];
		size_t bytesRead = BlReadFlash(buffer, address, numBytes);
		for(size_t i = 0; i < bytesRead; i++) {
			out << buffer[i];
		}
	}

	size_t Control::BlReadFlash(uint8_t *pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		if(numBytes + address > FLASH_SIZE) {
			throw InvalidParameter(std::string(__FILE__) + ':' + __FUNCTION__ + ": address or numBytes out of range");
		}

		size_t bytesRead;
		size_t sumBytesRead = 0;
		BlFlashReadRequest readRequest;
		while(numBytes > FLASH_READ_BLOCKSIZE)
		{
			readRequest.SetAddressNumBytes(address, FLASH_READ_BLOCKSIZE);
			bytesRead = BlRead(readRequest, pBuffer, FLASH_READ_BLOCKSIZE);
			sumBytesRead += bytesRead;
			address += FLASH_READ_BLOCKSIZE;
			numBytes -= FLASH_READ_BLOCKSIZE;
			pBuffer += FLASH_READ_BLOCKSIZE;
		}

		readRequest.SetAddressNumBytes(address, numBytes);
		bytesRead = BlRead(readRequest, pBuffer, numBytes);
		sumBytesRead += bytesRead;
		return sumBytesRead;
	}

	uint16_t Control::BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError)
	{
		BlInfo info;
		BlReadInfo(info);


		union versionUnion {
			uint16_t version;
			uint8_t bytes[2];
		}
		versUnion;

		BlReadFlash(versUnion.bytes, VERSION_STRING_ORIGIN, sizeof(versUnion.bytes));

		return ntohs(versUnion.version) > 300 ? 0 : ntohs(versUnion.version);
	}

	void Control::BlReadInfo(BlInfo& blInfo) const throw (ConnectionTimeout, FatalError)
	{
		BlInfoRequest request;
			BlRead(request, reinterpret_cast<unsigned char *>(&blInfo), sizeof(BlInfo));
	}

	void Control::BlWriteFlash(unsigned int address, unsigned char *pBuffer, size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		if(bufferLength + address > FLASH_SIZE) {
			throw InvalidParameter("Address + bufferLength out of flash range\n");
		}

		BlFlashWriteRequest request;
		unsigned char response;

		size_t bytesLeft = bufferLength;
		while(bytesLeft > FLASH_WRITE_BLOCKSIZE)
		{
			request.SetData(address, pBuffer, FLASH_WRITE_BLOCKSIZE);

			// we expect only the 0x04 command byte as response
			BlRead(request, &response, sizeof(response));
			if(response != 0x04) {
				throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
			}
			address += FLASH_WRITE_BLOCKSIZE;
			pBuffer += FLASH_WRITE_BLOCKSIZE;
			bytesLeft -= FLASH_WRITE_BLOCKSIZE;
		}

		request.SetData(address, pBuffer, bytesLeft);
			BlRead(request, &response, sizeof(response));
		if(response != 0x04) {
			throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
		}
	}

	void Control::BlWriteEeprom(unsigned int address, const uint8_t *pBuffer, size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter)
	{
		if(bufferLength + address > EEPROM_SIZE) {
			throw InvalidParameter("Address + bufferLength out of eeprom range\n");
		}

		BlEepromWriteRequest request;
		unsigned char response;

		size_t bytesLeft = bufferLength;
		while(bytesLeft > EEPROM_WRITE_BLOCKSIZE)
		{
			request.SetData(address, pBuffer, EEPROM_WRITE_BLOCKSIZE);

			// we expect only the 0x06 command byte as response
			BlRead(request, &response, sizeof(response));
			if(response != 0x06) {
				throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
			}

			address += EEPROM_WRITE_BLOCKSIZE;
			pBuffer += EEPROM_WRITE_BLOCKSIZE;
			bytesLeft -= EEPROM_WRITE_BLOCKSIZE;
		}

		request.SetData(address, pBuffer, bytesLeft);
			BlRead(request, &response, sizeof(response));
		if(response != 0x06) {
			throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
		}
	}

	void Control::BlProgramFlash(const std::string& pFilename) const throw (ConnectionTimeout, FatalError)
	{
		std::ifstream hexFile;
		hexFile.open(const_cast<char *>(pFilename.c_str()), ifstream::in);

		if(!hexFile.good()) {
			throw FatalError("opening '" + pFilename + "' failed");
		}

		intelhex hexConverter;
		hexFile >> hexConverter;

		BlInfo info;
		BlReadInfo(info);

		/*Check if last address of programmcode is not in the bootblock */
		unsigned long endAddress;
		if(!hexConverter.endAddress(&endAddress)) {
			throw FatalError("can't read endAddress from hexConverter \n");
		}

		if(endAddress >= (unsigned long)(info.GetAddress())) {
			throw FatalError("endaddress of program code is in bootloader area of the target device flash \n");
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
		if(hexConverter.currentAddress() != 0) {
			throw FatalError("program code does not start at address 0x0000 \n");
		}

		for(unsigned int i = 0; i < sizeof(appVector); i++) {
			if(!hexConverter.getData(&appVector[i],(unsigned long)i)) {

				throw FatalError("can not read data at address " + std::to_string(hexConverter.currentAddress()));
			}
		}

		BlEnableAutostart();
		BlEraseFlash();

		unsigned int writeAddress = 0;
		unsigned char flashBuffer[FLASH_SIZE];
		unsigned char nextByte;

		if(endAddress > FLASH_SIZE) {
			throw FatalError("endaddress of program code is outside the target device flash\n");
		}

		/* Write the resetVector to temporary flashBuffer*/
		memcpy(&flashBuffer[0], &resetVector[0], sizeof(resetVector));

		writeAddress += sizeof(resetVector);

		for(; writeAddress <= (unsigned int)endAddress; writeAddress++) {
			if(hexConverter.getData(&nextByte, writeAddress)) {
				flashBuffer[writeAddress] = nextByte;
			} else {
				flashBuffer[writeAddress] = 0xff;
			}
		}
		BlWriteFlash(0, &flashBuffer[0], (size_t)endAddress + 1);

		/* we always have to write a FLASH_WRITE Block when we wanna write to device flash,
		 * so we have to pack the appVector at the end of a Block of data */

		unsigned char appVecBuf[FLASH_WRITE_BLOCKSIZE];

		for(int i = 0; i < FLASH_WRITE_BLOCKSIZE; i++) {
			appVecBuf[i] = 0xff;
		}

		memcpy(&appVecBuf[sizeof(appVecBuf) - sizeof(appVector)], &appVector[0], sizeof(appVector));
		BlWriteFlash(info.GetAddress() - FLASH_WRITE_BLOCKSIZE, &appVecBuf[0], FLASH_WRITE_BLOCKSIZE);
	}

	void Control::BlRunApp(void) const throw (ConnectionTimeout, FatalError)
	{
		BlRunAppRequest request;
		unsigned char buffer[32];
		size_t bytesRead = BlRead(request, &buffer[0], 6);

				Trace(ZONE_VERBOSE, "We got %zd bytes response.\n", bytesRead);
		if(4 <= bytesRead) {
			struct response_frame *pResponse = (response_frame *)&buffer[0];
			if(pResponse->cmd == FW_STARTED) return;
			throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
		}
		throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong length");
	}

	bool Control::ConfGetSoftAp(void) const
	{
		std::string result {};
		if(mTelnet.Open()) {
			mTelnet.RecvString("get wlan\r\n", "Join=", result);
			mTelnet.Close(false);
		}
		return (0 == result.compare("7"));
	}

	std::string Control::ConfGet(const std::string& searchKey, const std::string& getCmd) const
	{
		std::string result {};
		if(mTelnet.Open()) {
			mTelnet.RecvString(getCmd, searchKey, result);
			mTelnet.Close(false);
		}
		return result;
	}

	std::string Control::ConfGetDeviceId(void) const
	{
		return ConfGet("DeviceId=", "get opt\r\n");
	}

	std::string Control::ConfGetPassphrase(void) const
	{
		return ConfGet("Passphrase=");
	}

	std::string Control::ConfGetSsid(void) const
	{
		return ConfGet("SSID=");
	}

	bool Control::ConfModuleAsSoftAP(const std::string& accesspointName) const
	{
		if(!ConfSetDeviceId(accesspointName)) {
				Trace(ZONE_ERROR, "set DeviceId failed\n");
			return false;
		}

		static const size_t SSID_MAX = 32;

		if((accesspointName.size() < 1) || (accesspointName.size() > SSID_MAX)) {
				Trace(ZONE_WARNING, "Invalid wlan ssid '%s'\n", accesspointName.data());
			return false;
		}

		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.SendString("set wlan ssid ", accesspointName)) {
				Trace(ZONE_ERROR, "set wlan ssid to '%s' failed\n", accesspointName.data());
			mTelnet.Close(false);
			return false;
		}

		if(!mTelnet.Close(true)) {
				Trace(ZONE_ERROR, "save changes failed\n");
			return false;
		}

		if(!ConfSetParameters(RN171_SOFT_AP_DEFAULT_PARAMETERS)) {
				Trace(ZONE_ERROR, "set defaults failed\n");
			return false;
		}

		return ConfRebootWlanModule();
	}

	bool Control::ConfModuleForWlan(const std::string& phrase, const std::string& ssid, const std::string& deviceId) const
	{
		if(!ConfSetDefaults()) {
			Trace(ZONE_ERROR, "set defaults failed\n");
			return false;
		}

		if(!ConfSetWlan(phrase, ssid)) {
			Trace(ZONE_ERROR, "set wlan phrase and ssid failed\n");
			return false;
		}

		if(!ConfSetDeviceId(deviceId)) {
			Trace(ZONE_ERROR, "set device name failed\n");
			return false;
		}

		return ConfRebootWlanModule();
	}

	bool Control::ConfSetParameters(std::list<std::string> commands) const
	{
		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		for(auto command : commands) {
			if(!mTelnet.Send(command)) {
				Trace(ZONE_ERROR, "command: '%s' failed -> exit without saving\n", command.c_str());
				return mTelnet.Close(false);
			}
		}
		return mTelnet.Close(true);
	}

	bool Control::ConfSetDefaults(void) const
	{
		return this->ConfSetParameters(RN171_DEFAULT_PARAMETERS);
	}

	bool Control::ConfSetWlan(const std::string& phrase, const std::string& ssid) const
	{
		static const size_t PHRASE_MAX = 63;
		static const size_t SSID_MAX = 32;

		if((phrase.size() < 1) || (phrase.size() > PHRASE_MAX) || 0 != std::count_if(phrase.begin(), phrase.end(), iscntrl)) {
			Trace(ZONE_WARNING, "Invalid wlan passphrase '%s'\n", phrase.data());
			return false;
		}

		if((ssid.size() < 1) || (ssid.size() > SSID_MAX)) {
			Trace(ZONE_WARNING, "Invalid wlan ssid '%s'\n", ssid.data());
			return false;
		}

		if(!mTelnet.Open()) {
			Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.SendString("set wlan phrase ", phrase)) {
			Trace(ZONE_ERROR, "set wlan phrase to '%s' failed\n", phrase.data());
			mTelnet.Close(false);
			return false;
		}

		if(!mTelnet.SendString("set wlan ssid ", ssid)) {
				Trace(ZONE_ERROR, "set wlan ssid to '%s' failed\n", ssid.data());
			mTelnet.Close(false);
			return false;
		}
		return mTelnet.Close(true);
	}

	bool Control::ConfSetDeviceId(const std::string& name) const
	{
		static const size_t NAME_MAX_LEN = 32;

		if((name.size() < 1) || (name.size() > NAME_MAX_LEN)) {
				Trace(ZONE_WARNING, "Invalid device name '%s'\n", name.data());
			return false;
		}

		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		if(!mTelnet.SendString("set opt deviceid ", name)) {
				Trace(ZONE_ERROR, "set device name to '%s' failed\n", name.data());
			mTelnet.Close(false);
			return false;
		}

		return mTelnet.Close(true);
	}

	bool Control::ConfRebootWlanModule(void) const
	{
		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}

		static const std::string command = "reboot";

		if(!mTelnet.SendRebootCommand()) {
				Trace(ZONE_ERROR, "send reboot command failed\n");
			mTelnet.Close(false);
			return false;
		}
		return true;
	}

	bool Control::ConfChangeWlanChannel(void) const
	{
		if(!mTelnet.Open()) {
				Trace(ZONE_ERROR, "open telnet connection failed\n");
			return false;
		}
		std::string result;
		if(!mTelnet.PerformWifiScan(result)) {
				Trace(ZONE_ERROR, "wifi scan failed\n");
			return mTelnet.Close(false);
		}
		std::cout << result << std::endl;
		unsigned int newChannel = mTelnet.ComputeFreeChannel(result);
		std::cout << newChannel;
		if(!newChannel) {
				Trace(ZONE_ERROR, "compute new channel failed\n");
			return mTelnet.Close(false);
		}
		mTelnet.ChangeWifiChannel(newChannel);
		return mTelnet.Close(false);
	}

	std::string Control::FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		FwCmdGetCycletime cmd;
		*this << cmd;
		return cmd.mResponse.ToString();
	}

	void Control::FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		FwCmdGetRtc cmd;
		*this << cmd;
		timeValue = cmd.mResponse.GetRealTime();
	}

	std::string Control::FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		FwCmdGetTracebuffer cmd;
		*this << cmd;
		return cmd.mResponse.ToString();
	}

	uint16_t Control::FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		FwCmdGetVersion cmd;
		*this << cmd;
		return cmd.mResponse.getVersion();
	}

	void Control::FwSend(FwCommand& cmd) const throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		if(cmd.IsResponseRequired()) {

			response_frame buffer;
			size_t numCrcRetries = 8;
			do
			{
				const size_t bytesRead = mProxy.Send(cmd, &buffer, sizeof(buffer));
				TraceBuffer(ZONE_VERBOSE, (uint8_t *)&buffer, bytesRead, "%02x ", "We got %zd bytes response.\nMessage: ", bytesRead);

				if(cmd.GetResponse().Init(buffer, bytesRead)) {
					return;
				}
			}
			while(0 < --numCrcRetries);
			throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": Too many retries");
		} else {
			MaskBuffer maskBuffer {BL_MAX_MESSAGE_LENGTH};
			maskBuffer.Mask(cmd.GetData(), cmd.GetData() + cmd.GetSize(), false);
			if(maskBuffer.Size() != mUdpSock.Send(maskBuffer.Data(), maskBuffer.Size())) {
				throw FatalError("mUdpSock.Send() failed");
			}
		}
	}

	void Control::FwStressTest(void)
	{
		*this << FwCmdClearScript {};

		uns8 ledArr[NUM_OF_LED * 3];
		uns8 color = 0;
		while(true) {
			color++;
			std::fill_n(ledArr, sizeof(ledArr), color);
			*this << FwCmdSetColorDirect {ledArr, sizeof(ledArr)};
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	uint16_t Control::ExtractFwVersion(const std::string& pFilename) const
	{
		std::ifstream hexFile;
		hexFile.open(const_cast<char *>(pFilename.c_str()), ifstream::in);

		if(!hexFile.good()) {
			throw FatalError("opening '" + pFilename + "' failed");
		}

		intelhex hexConverter;
		hexFile >> hexConverter;
		union versionUnion {
			uint16_t version;
			uint8_t bytes[2];
		}
		versUnion;

		hexConverter.getData(&versUnion.bytes[1], VERSION_STRING_ORIGIN);
		hexConverter.getData(&versUnion.bytes[0], VERSION_STRING_ORIGIN + 1);

		return ntohs(versUnion.version) > 300 ? 0 : ntohs(versUnion.version);
	}

	Control& Control::operator<<(FwCommand&& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		this->FwSend(cmd);
		return *this;
	}

	Control& Control::operator<<(FwCommand& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		this->FwSend(cmd);
		return *this;;
	}

	Control& Control::operator<<(const Script& script) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
	{
		for(auto it = script.begin(); it != script.end(); ++it) {
			*this << **it;
		}
		return *this;
	}

	void Control::FwTest(void)
	{
	#if 0
		static const timespec sleepTime {0, 50000000};
		uint32_t color = 0xff;
		for(size_t i = 0; i < 100; ++i) {
			color = ((color & 0xff) << 24) | (color >> 8);
			*this << FwCmdSetFade {color};
			nanosleep(&sleepTime, NULL);
		}
	#else


		*this << FwCmdClearScript() << FwCmdSetFade(WiflyColor::BLACK, 2);

/*		WyLight::Script testScript("test.script");

                testScript.emplace_front(FwCmdLoopOn());
                testScript.emplace_back(FwCmdSetFade(WiflyColor::GREEN, 2000));
                testScript.emplace_back(FwCmdSetFade(WiflyColor::RED, 2000));
                testScript.emplace_back(FwCmdSetFade(WiflyColor::BLUE, 2000));
                testScript.emplace_back(FwCmdLoopOff(5));

                *this << testScript;
*/		/*
                uint32_t bitMask = 0x01;
                for(unsigned int i = 0; i < NUM_OF_LED; i++)
                {
                        ledColor.red((uint8_t)((0xff / NUM_OF_LED) * i));
                        ledColor.green((uint8_t)((0xff / NUM_OF_LED) * i));
                        ledColor.blue(0xff);
                        FwSetFade(setFadeResp, bitMask, ledColor.argb(), 20000 + i * 200, true);
                        bitMask = bitMask << 1;
                }

                testScript.push_back((setWaitResp, 30000);
                FwSetFade(setFadeResp, WiflyColor::GREEN,2000, 0xFFFFFFFF, false);
                FwSetFade(setFadeResp, WiflyColor::RED,  2000, 0x000000FF, true);
                FwSetFade(setFadeResp, WiflyColor::GREEN,2000, 0x0000FF00, true);
                FwSetFade(setFadeResp, WiflyColor::BLUE, 2000, 0x00FF0000, true);
                FwSetFade(setFadeResp, WiflyColor::WHITE,2000, 0xFF000000, false);
                FwSetWait(setWaitResp, 2000);
                FwSetFade(setFadeResp, WiflyColor::BLACK,2000, 0xFFFFFFFF, false);
                FwLoopOff(loopOffResp, 0);*/
	#endif
	}
} /* namespace WyLight */
