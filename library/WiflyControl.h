/*
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


#ifndef _WIFLYCONTROL_H_
#define _WIFLYCONTROL_H_

#include <string>
#include "ComProxy.h"
#include "wifly_cmd.h"
#include "BlRequest.h"
#include "TelnetProxy.h"
#include "WiflyControlException.h"
#include "FwCommand.h"
#include "Script.h"


namespace WyLight {

/******************************************************************************/
/*! \file WiflyControl.h
 * \author Nils Weiss, Patrick Bruenn
 *
 *! \cond
 * class - Control
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Hardware.
 *
 *
 * The Control class allows the user to control the Wifly_Light Hardware.
 * There are three target's at the Wifly_Light Hardware.
 * - Bootloader<br>
 *           All methodes with Bl* relate to the bootloader part.
 * - Firmware<br>
 *           All methodes with Fw* relate to the firmware part.
 * - RN-171 Wifi Interface<br>
 *           All methodes witch Conf* relate to the communication module.
 *******************************************************************************/
	class Control
	{
	public:
		/**
		 * string constant to address all LEDs. String representation of 0xffffffff
		 */
		static const std::string LEDS_ALL;
		static const std::list<std::string> RN171_DEFAULT_PARAMETERS;
		static const std::list<std::string> RN171_BASIC_PARAMETERS;
		static const std::list<std::string> RN171_SOFT_AP_DEFAULT_PARAMETERS;

		/**
		 * Connect to a wifly device
		 * @param addr ipv4 address as 32 bit value in host byte order
		 * @param port number of the wifly device server in host byte order
		 */
		Control(uint32_t addr, uint16_t port);

		/*
		 * Send a byte sequence to ident the current software running on PIC
		 * @return mode of target: BL_IDENT for Bootloader mode, FW_IDENT for Firmware mode
		 * @throw FatalError if synchronisation fails
		 */
		size_t GetTargetMode(void) const throw(FatalError);

/* ------------------------- BOOTLOADER METHODES ------------------------- */
		/**
		 * Instructs the bootloader to set the autostart flag to true. This ensures
		 * the bootloader will be started on the next reboot automatically.
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlEnableAutostart(void) const throw(ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader to erase the whole eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader to erase the whole flash which is not occupied
		 * by the bootloader itself.
		 * The wifly device has to be in bootloader mode for this command.
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		void BlEraseFlash(void) const throw(ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader to update the wifly device with new firmware.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param filename path to the *.hex file containing the new firmware
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		void BlProgramFlash(const std::string& filename) const throw (ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader to create crc-16 checksums for the content of
		 * the specified flash area. TODO crc values are in little endian byte order
		 * The wifly device has to be in bootloader mode for this command.
		 * @param out ostream for the resulting 16bit crc values
		 * @param address crc generation starts from this flash address
		 * @param numBytes size of the flash area for which the crc are calculated
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader to read the specified memory area of the eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param out ostream where the eeprom content is written
		 * @param address start of the eeprom region to read
		 * @param numBytes size of the eeprom region to read
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader to read the specified memory area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param out ostream where the flash content is written
		 * @param address start of the flash region to read
		 * @param numBytes size of the flash region to read
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader to read the version string from the firmware memory.
		 * The wifly device has to be in bootloader mode for this command.
		 * @return the version string from pic flash memory
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		uint16_t BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader to return a struct of bootloader informations
		 * like bootloader version, flash and eeprom size. see \<BlInfo\> for details.
		 * The wifly device has to be in bootloader mode for this command.
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		void BlReadInfo(BlInfo& info) const throw (ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader to start the wifly device firmware.
		 * The wifly device has to be in bootloader mode for this command.
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		void BlRunApp(void) const throw (ConnectionTimeout, FatalError);

/* --------------------- WLAN CONFIGURATION METHODES --------------------- */

		/**
		 * Read the device id from WyLight module
		 * @return an empty string or the device id
		 */
		std::string ConfGetDeviceId(void) const;

		/**
		 * Read the currently configured wlan passphrase from WyLight module
		 * @return an empty string or the wlan passphrase
		 */
		std::string ConfGetPassphrase(void) const;

		/**
		 * Read the currently configured mode(client/SoftAP) from WyLight module
		 * @return true if WyLight module is configured for SoftAP mode
		 */
		bool ConfGetSoftAp(void) const;

		/**
		 * Read the currently configured ssid from WyLight module
		 * @return an empty string or the ssid
		 */
		std::string ConfGetSsid(void) const;

		/**
		 * Configurates the WyLight module as stand alone accesspoint. With accesspoint name you can change the ssid for this accesspoint.
		 * @param accesspointName 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool ConfModuleAsSoftAP(const std::string& accesspointName = "Wifly_Light") const;

		/**
		 * Configurates the WyLight module as client for an existing wlan network with WPA2 protection
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @param deviceId 1 - 32 characters, unique name which apperas in the broadcast message
		 * @return false, in case of an error
		 */
		bool ConfModuleForWlan(const std::string& phrase, const std::string& ssid, const std::string& deviceId = "Wifly_Light") const;

		/**
		 * Reboot the modul. ATTENTION: You have to reconnect after a reboot
		 * @return false, in case of an error
		 */
		bool ConfRebootWlanModule(void) const;

		/**
		 * Allows you to give every Wifly_Light device an unique name
		 * @param name 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool ConfSetDeviceId(const std::string& name) const;

		/**
		 * Wlan module performs a wifi scan and changes to the next free channel. This function can take some time.
		 * @return false, in case of an error
		 */
		bool ConfChangeWlanChannel(void) const;

		/**
		 *
		 */
		bool ConfSetParameters(std::list<std::string> commands) const;



/* -------------------------- FIRMWARE METHODES -------------------------- */
		/**
		 * Reads the cycletimes from wifly device and stores them into the response object
		 * @return a string with all recorded cycletimes from PIC firmware
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		std::string FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Reads the current rtc time from the wifly device
		 * @param timeValue reference to a tm object, where to store the rtc time from PIC firmware
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Reads the tracebuffer from wifly device and stores the data into the response object
		 * @return a string with all recorded trace messages from PIC firmware
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		std::string FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Reads the firmware version currently running on the wifly device.
		 * @return a string representing the version number of the PIC firmware
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		uint16_t FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		//TODO move this test functions to the integration test
		void FwTest(void);
		void FwStressTest(void);

		Control& operator<<(FwCommand&& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
		Control& operator<<(FwCommand& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
		Control& operator<<(const Script& script) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

/* ------------------------- VERSION EXTRACT METHODE ------------------------- */
		/**
		 * Methode to extract the firmware version from a hex file
		 * @return the version string from a given hex file
		 */
		uint16_t ExtractFwVersion(const std::string& pFilename) const;


/* ------------------------- PRIVATE DECLARATIONS ------------------------- */
	private:
		/**
		 * Sockets used for communication with wifly device.
		 * A reference to the TcpSocket is provided to the aggregated subobjects.
		 */
		const TcpSocket mTcpSock;

		/**
		 * The UdpSocket is used directly in WiflyControl, to send fast connectionless packets.
		 */
		const UdpSocket mUdpSock;

		/**
		 * Proxy object handling the low level communication with bootloader and firmware.
		 */
		const ComProxy mProxy;

		/**
		 * Proxy object handling the communication with the wlan module for its configuration
		 */
		const TelnetProxy mTelnet;

		/**
		 * Instructs the bootloader to erase the specified area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param endAddress address of the block to delete
		 * @param numPages number of pages in a block
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		void BlEraseFlashArea(const uint32_t endAddress, const uint8_t numPages) const throw (ConnectionTimeout, FatalError);

		/**
		 * Send a request to the bootloader and read his response into pResponse
		 * @param request reference to a bootloader requested
		 * @param pResponse a buffer for the response of the bootloader
		 * @param responseSize sizeof of the <pResponse> buffer in bytes
		 * @param doSync if set to 'true' the uart sync is issued before data transfer default = true
		 * @return the number of bytes the bootloader send back in his response
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 */
		size_t BlRead(const BlRequest& request, uint8_t *pResponse, const size_t responseSize, bool doSync = true) const throw(ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader of the wifly device to write data to the eeprom.
		 * @param address in eeprom
		 * @param pBuffer containing the new data for eeprom
		 * @param bufferLength number of bytes to write to eeprom
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlWriteEeprom(uint32_t address, const uint8_t *pBuffer, size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader of the wifly device to write data to the flash.
		 * @param address in flash
		 * @param pBuffer containing the new data for flash
		 * @param bufferLength number of bytes to write to flash
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlWriteFlash(uint32_t address, uint8_t *pBuffer, size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Sends a wifly command frame to the wifly device
		 * @param command FwCommand object with the frame, which should be send
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwSend(FwCommand& cmd) const throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Instructs the bootloader to create crc-16 checksums for the content of
		 * the specified flash area. TODO crc values are in little endian byte order
		 * The wifly device has to be in bootloader mode for this command.
		 * @param pBuffer pointer to a buffer for the resulting 16bit crc values
		 * @param address crc generation starts from this flash address
		 * @param numBytes size of the flash area for which the crc are calculated
		 * @return the number of bytes read (result / 2 = number of crc values)
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		size_t BlReadCrcFlash(uint8_t *pBuffer, uint32_t address, uint16_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader to read the specified memory area of the eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param pBuffer destination for the copy of the eeprom content, should be at least \<numBytes\> wide
		 * @param address start of the eeprom region to read
		 * @param numBytes size of the eeprom region to read
		 * @return the number of bytes read
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		size_t BlReadEeprom(uint8_t *pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader to read the specified memory area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param pBuffer destination for the copy of the flash content, should be at least \<numBytes\> wide
		 * @param address start of the flash region to read
		 * @param numBytes size of the flash region to read
		 * @return the number of bytes read
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		size_t BlReadFlash(uint8_t *pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Read the currently configured wlan passphrase from WyLight module
		 * @param searchKey to search the wlan settings for
		 * @return an empty string or the value of the specified wlan setting
		 */
		std::string ConfGet(const std::string& searchKey, const std::string& getCmd = "get wlan\r\n") const;

		/**
		 * Set the WyLight module communication parameters to defaults
		 * @return false, in case of an error
		 */
		bool ConfSetDefaults(void) const;

		/**
		 * Set the WyLight module wlan connection parameters
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool ConfSetWlan(const std::string& phrase, const std::string& ssid) const;

/* ------------------ friendships for unittesting only ------------------- */
		friend size_t ut_WiflyControl_BlEepromWrite(void);
		friend size_t ut_WiflyControl_BlFlashWrite(void);
		friend size_t ut_WiflyControl_ConfSetDefaults(void);
		friend size_t ut_WiflyControl_ConfSetWlan(void);
	};
}
#endif /* #ifndef _WIFLYCONTROL_H_ */
