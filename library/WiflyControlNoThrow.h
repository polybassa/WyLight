/*
		Copyright (C) 2013 Nils Weiss, Patrick Bruenn.

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

#ifndef _WIFLYCONTROL_NOTHROW_H_
#define _WIFLYCONTROL_NOTHROW_H_
#include "WiflyControl.h"

/******************************************************************************/
/*!\file WiflyControlNoThrow.h
 * \author Nils Weiss, Patrick Bruenn
 *
 * \cond
 * enum - WiflyError
 * \endcond
 *
 * \brief Returnvalues of WiflyControlNoThrow
 *
 *
 *******************************************************************************/

enum WiflyError {
	NO_ERROR = 0,			/**< is returned if no error occurred */ 
	FATAL_ERROR,			/**< if command code of the response doesn't match the code of the request, or too many retries failed */ 
	CONNECTION_LOST,
	CONNECTION_TIMEOUT,		/**< if response timed out */
	INVALID_PARAMETER,		/**< if a parameter is out of bound */
	SCRIPT_FULL,
};

/******************************************************************************/
/*!\cond
 * class - WiflyControlNoThrow
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Hardware.
 *
 * 
 * The WiflyControlNoThrow class allows the user to control the Wifly_Light hardware.
 * This is a wrapper class to ::WiflyControl to catch all exceptions from the
 * lower software layers and convert them into error codes, which is required for
 * example for jni or iOS clients
 * There are three target's at the Wifly_Light Hardware.
 * - Bootloader<br>
 *           All methodes with Bl* relate to the bootloader part.
 * - Firmware<br>
 *           All methodes with Fw* relate to the firmware part.
 * - RN-171 Wifi Interface<br>
 *           All methodes witch Conf* relate to the communication module.
 *******************************************************************************/

class WiflyControlNoThrow : private WiflyControl
{	
	public:

		/**
		 * Connect to a wifly device
		 * @param addr ipv4 address as 32 bit value in host byte order
		 * @param port number of the wifly device server in host byte order
		 */
		WiflyControlNoThrow(uint32_t addr, uint16_t port);
		
/* ------------------------- BOOTLOADER METHODES ------------------------- */
		/**
		 * Instructs the bootloader to set the autostart flag to true. This ensures
		 * the bootloader will be started on the next reboot automatically.
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlEnableAutostart(void) const;
		
		/**
		 * Instructs the bootloader to erase the whole eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlEraseEeprom(void) const;

		/**
		 * Instructs the bootloader to erase the whole flash which is not occupied
		 * by the bootloader itself.
		 * The wifly device has to be in bootloader mode for this command.
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlEraseFlash(void) const;
		
		/**
		 * Instructs the bootloader to update the wifly device with new firmware.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param filename path to the *.hex file containing the new firmware
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlProgramFlash(const std::string& filename) const;

		/**
		 * Instructs the bootloader to create crc-16 checksums for the content of
		 * the specified flash area. TODO crc values are in little endian byte order
		 * The wifly device has to be in bootloader mode for this command.
		 * @param out ostream for the resulting 16bit crc values
		 * @param address crc generation starts from this flash address
		 * @param numBlocks size of the flash area for which the crc are calculated. One block contains 64 bytes.
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>INVALID_PARAMETER</B> if a parameter is out of bound
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBlocks) const;

		/**
		 * Instructs the bootloader to read the specified memory area of the eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param out ostream where the eeprom content is written
		 * @param address start of the eeprom region to read
		 * @param numBytes size of the eeprom region to read
		 * @return Indexed by ::WiflyError 
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>INVALID_PARAMETER</B> if a parameter is out of bound
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const;

		/**
		 * Instructs the bootloader to read the specified memory area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param out ostream where the eeprom content is written
		 * @param address start of the flash region to readRequest
		 * @param numBytes size of the flash region to read
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>INVALID_PARAMETER</B> if a parameter is out of bound
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const;

		/**
		 * Instructs the bootloader to read the version string from the firmware memory.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param versionString is the returnvalue for the firmware version from pic memory
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlReadFwVersion(std::string& versionString) const;

		/**
		 * Instructs the bootloader to return a struct of bootloader informations
		 * like bootloader version, flash and eeprom size. see ::BlInfo for details.
		 * The wifly device has to be in bootloader mode for this command.
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlReadInfo(BlInfo& info) const;

		/**
		 * Instructs the bootloader to start the wifly device firmware.
		 * The wifly device has to be in bootloader mode for this command.
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t BlRunApp(void) const;

/* --------------------- WLAN CONFIGURATION METHODES --------------------- */
		/**
		 * Read the currently configured ssid from Wifly module
		 * @param ssid is the outputstring for the current ssid set in the RN-171 WLAN modul
		 * @return Indexed by ::WiflyError
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfGetSsid(std::string& ssid) const;

		/**
		 * Set the Wifly module communication parameters to defaults
		 * @return Indexed by ::WiflyError
			<BR><B>FATAL_ERROR</B> in case of an error
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfSetDefaults(void) const;

		/**
		 * Set the Wifly module wlan connection parameters
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @return Indexed by ::WiflyError
			<BR><B>FATAL_ERROR</B> in case of an error
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfSetWlan(const std::string& phrase, const std::string& ssid) const;
		
/* -------------------------- FIRMWARE METHODES -------------------------- */
		/**
		 * Wipe all commands from the Wifly script controller
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwClearScript(void);
		
		/**
		 * Reads the cycletimes from wifly device and stores them into the response object
		 * @param output string with all recorded cycletimes from PIC firmware
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwGetCycletime(std::string& output);

		/**
		 * Reads the current rtc time from the wifly device
		 * @param timeValue reference to a tm object, where to store the rtc time from PIC firmware
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwGetRtc(tm& timeValue);

		/**
		 * Reads the tracebuffer from wifly device and stores the data into the response object
		 * @param output a string with all recorded trace messages from PIC firmware
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwGetTracebuffer(std::string& output);


		/**
		 * Reads the firmware version currently running on the wifly device.
		 * @param output a string representing the version number of the PIC firmware
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwGetVersion(std::string& output);

		/**
		 * Injects a LoopOff command into the wifly script controller
		 * @param numLoops number of rounds before termination of the loop, use 0 for infinite loops. To terminate an infinite loop you have to call \<FwClearScript\>
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwLoopOff(uint8_t numLoops);

		/**
		 * Injects a LoopOn command into the wifly script controller
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwLoopOn(void);

		/**
		 * Sets all leds with different colors directly. This doesn't affect the Wifly script controller
		 * Example: to set the first led to yellow and the second to blue and all others to off use a \<pBuffer\> like this:
		 * pBuffer[] = {0xff, 0xff, 0x00, 0x00, 0x00, 0xff}; bufferLength = 6;
		 * @param pBuffer containing continouse rgb values r1g1b1r2g2b2...r32g32b32
		 * @param bufferLength number of bytes in \<pBuffer\> usally 32 * 3 bytes
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength);
		
		/**
		 * Injects a fade command into the wifly script controller
		 * @param argb is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff) f.e.
		 *        black(  0,  0,  0) as argb is 0xff000000
		 *        green(  0,255,  0) as argb is 0xff00ff00
		 *        white(255,255,255) as argb is 0xffffffff
		 * @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		 * @param addr bitmask of leds which should be effected by this command, set bit to 1 to affect the led, default 0xffffffff
		 * @param parallelFade if true other fades are allowed in parallel with this fade
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwSetFade(uint32_t argb, uint16_t fadeTime = 0, uint32_t addr = 0xffffffff, bool parallelFade = false);

		/**
		 * Sets the rtc clock of the wifly device to the specified time.
		 * The wifly device has to be in firmware mode for this command.
		 * @param timeValue pointer to a posix tm struct containing the new time
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwSetRtc(const tm& timeValue);
		
		/**
		 * Injects a wait command into the wifly script controller.
		 * This causes the script processing to wait before executing the next command for the specified duration
		 * @param waitTime in hundreths of a second
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
		 <	BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwSetWait(uint16_t waitTime);

		/**
		 * Stops firmware and script controller execution and start the bootloader of the wifly device
		 * @return Indexed by ::WiflyError
			<BR><B>CONNECTION_TIMEOUT</B> if response timed out
			<BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
			<BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
			<BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwStartBl(void);
	
	private:
		/**
		 * Converts all exceptions from ::WiflyControl to the relating ::WiflyError
		 */
		uint32_t SolveException(void) const;
	
};
#endif /* #ifndef _WIFLYCONTROL_NOTHROW_H_ */
