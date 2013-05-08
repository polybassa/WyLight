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


/******************************************************************************/
/*! \file WiflyControl.h
 * \author Nils Weiss, Patrick Bruenn
 *
 *! \cond
 * class - WiflyControl
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Hardware.
 *
 * 
 * The WiflyControl class allows the user to control the Wifly_Light Hardware.
 * There are three target's at the Wifly_Light Hardware.
 * - Bootloader<br>
 *           All methodes with Bl* relate to the bootloader part.
 * - Firmware<br>
 *           All methodes with Fw* relate to the firmware part.
 * - RN-171 Wifi Interface<br>
 *           All methodes witch Conf* relate to the communication module.
 *******************************************************************************/

#ifndef _WIFLYCONTROL_H_
#define _WIFLYCONTROL_H_

#include <string>
#include <time.h>
#include "ComProxy.h"
#include "wifly_cmd.h"
#include "BlRequest.h"
#include "TelnetProxy.h"
#include "WiflyControlException.h"
#include "FwCommand.h"
#include "FwRequest.h"
#include "FwResponse.h"

namespace WyLight {

class Control
{
	public:
		/**
		 * string constant to address all LEDs. String representation of 0xffffffff
		 */
		static const std::string LEDS_ALL;

		/**
		 * Connect to a wifly device
		 * @param addr ipv4 address as 32 bit value in host byte order
		 * @param port number of the wifly device server in host byte order
		 */
		Control(uint32_t addr, uint16_t port);
		
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
		std::string BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError);

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
		 * @param name 1 - 32 characters, unique name which apperas in the broadcast message
		 * @return false, in case of an error
		 */
		bool ConfModuleForWlan(const std::string& phrase, const std::string& ssid, const std::string& name = "Wifly_Light") const;

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
		
/* -------------------------- FIRMWARE METHODES -------------------------- */
		/**
		 * Wipe all commands from the WyLight script controller
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwClearScript(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
		
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
		std::string FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Injects a LoopOff command into the wifly script controller
		 * @param numLoops number of rounds before termination of the loop, use 0 for infinite loops. To terminate an infinite loop you have to call \<FwClearScript\>
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwLoopOff(uint8_t numLoops) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Injects a LoopOn command into the wifly script controller
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwLoopOn(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Sets all leds with different colors directly. This doesn't affect the WyLight script controller
		 * Example: to set the first led to yellow and the second to blue and all others to off use a \<pBuffer\> like this:
		 * pBuffer[] = {0xff, 0xff, 0x00, 0x00, 0x00, 0xff}; bufferLength = 6;
		 * @param pBuffer containing continouse rgb values r1g1b1r2g2b2...r32g32b32
		 * @param bufferLength number of bytes in \<pBuffer\> usally 32 * 3 bytes
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
		
		/**
		 * Injects a fade command into the wifly script controller
		 * @param argb is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff) f.e.
		 *        black(  0,  0,  0) as argb is 0xff000000
		 *        green(  0,255,  0) as argb is 0xff00ff00
		 *        white(255,255,255) as argb is 0xffffffff
		 * @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		 * @param addr bitmask of leds which should be effected by this command, set bit to 1 to affect the led, default 0xffffffff
		 * @param parallelFade if true other fades are allowed in parallel with this fade
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwSetFade(uint32_t argb, uint16_t fadeTime = 0, uint32_t addr = 0xffffffff, bool parallelFade = false) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Injects a fade command into the wifly script controller
		 * @param rgb is a hex string representation of a rgb value without leading '0x'
		 *        black "0"
		 *        green "ff00"
		 *        white "ffffff"
		 * @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		 * @param addr is a hex string representation of a 32 bit mask without leading '0x' of leds which should be effected by this comman.
		 *        all leds          "ffffffff" (default)
		 *        first three leds  "7"
		 *        only the last led "80000000"
		 * @param parallelFade if true other fades are allowed in parallel with this fade
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwSetFade(const std::string& rgb, uint16_t fadeTime = 0, const std::string& addr = LEDS_ALL, bool parallelFade = false) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
	
	   /**
		* Injects a gradient command into the wifly script controller
		* @param argb_1 is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff). This is the start color for the gradient.
		* @param argb_2 is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff). This is the end color for the gradient.
		* @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		* @param parallelFade if true other fades are allowed in parallel with this fade
		* @param length is the number of led's from startposition to endposition
		* @param offset can be used to move the startposition of the gradient on the ledstrip
		* @throw ConnectionTimeout if response timed out
		* @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		* @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		*/
		void FwSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime = 0, bool parallelFade = false, uint8_t length = NUM_OF_LED, uint8_t offset = 0);
	
		/**
		* Injects a gradient command into the wifly script controller
		* @param rgb_1 is a hex string representation of a rgb value without leading '0x'
		* @param rgb_2 is a hex string representation of a rgb value without leading '0x'
		* @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		* @param parallelFade if true other fades are allowed in parallel with this fade
		* @param length is the number of led's from startposition to endposition
		* @param offset can be used to move the startposition of the gradient on the ledstrip
		* @throw ConnectionTimeout if response timed out
		* @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		* @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		*/
		void FwSetGradient(const std::string& rgb_1, const std::string& rgb_2, uint16_t fadeTime = 0, bool parallelFade = false, uint8_t length = NUM_OF_LED, uint8_t offset = 0);

		/**
		 * Sets the rtc clock of the wifly device to the specified time.
		 * The wifly device has to be in firmware mode for this command.
		 * @param timeValue pointer to a posix tm struct containing the new time
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwSetRtc(const tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
		
		/**
		 * Injects a wait command into the wifly script controller.
		 * This causes the script processing to wait before executing the next command for the specified duration
		 * @param waitTime in hundreths of a second
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwSetWait(uint16_t waitTime) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

		/**
		 * Stops firmware and script controller execution and start the bootloader of the wifly device
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */
		void FwStartBl(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
		

		//TODO move this test functions to the integration test 
		void FwTest(void);
		void FwStressTest(void);
	
		Control& operator<<(const FwCommand& cmd);

/* ------------------------- VERSION EXTRACT METHODE ------------------------- */
		/**
		 * Methode to extract the firmware version from a hex file
		 * @return the version string from a given hex file
		 */
		std::string ExtractFwVersion(const std::string& pFilename) const;
	

/* ------------------------- PRIVATE DECLARATIONS ------------------------- */
	private:
		/**
		 * Socket used for communication with wifly device.
		 * A reference to this socket is provided to the aggregated subobjects.
		 */
		const TcpSocket mSock;

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
		size_t BlRead(const BlRequest& request, uint8_t* pResponse, const size_t responseSize, bool doSync = true) const throw(ConnectionTimeout, FatalError);

		/**
		 * Instructs the bootloader of the wifly device to write data to the eeprom.
		 * @param address in eeprom
		 * @param pBuffer containing the new data for eeprom
		 * @param bufferLength number of bytes to write to eeprom
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlWriteEeprom(uint32_t address, const uint8_t* pBuffer, size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Instructs the bootloader of the wifly device to write data to the flash.
		 * @param address in flash
		 * @param pBuffer containing the new data for flash
		 * @param bufferLength number of bytes to write to flash
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw InvalidParameter a parameter is out of bound
		 */
		void BlWriteFlash(uint32_t address, uint8_t* pBuffer, size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter);

		/**
		 * Sends a wifly command frame to the wifly device
		 * @param request FwRequest object with the frame, which should be send
		 * @param response will be modified according to the success of this operation
		 * @return response
		 * @throw ConnectionTimeout if response timed out
		 * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
		 * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
		 */		
		FwResponse& FwSend(const FwRequest& request, FwResponse& response) const throw (ConnectionTimeout, FatalError, ScriptBufferFull);
			
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
		size_t BlReadCrcFlash(uint8_t* pBuffer, uint32_t address, uint16_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);
		
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
		size_t BlReadEeprom(uint8_t* pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);
	
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
		size_t BlReadFlash(uint8_t* pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout, FatalError, InvalidParameter);
	
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
	   /**
		* friendships for unittesting only
		*/
		friend bool ut_WiflyControl_ConfSetDefaults(Control& ref);
	
		/**
		 * friendships for unittesting only
		 */
		friend bool ut_WiflyControl_ConfSetWlan(Control& ref, const std::string& phrase, const std::string& ssid);
	
		/**
		 * friendships for unittesting only
		 */
		friend size_t ut_WiflyControl_BlEepromWrite(void);

		/**
		* friendships for unittesting only
		*/
		friend size_t ut_WiflyControl_BlFlashWrite(void);
};
}
#endif /* #ifndef _WIFLYCONTROL_H_ */
