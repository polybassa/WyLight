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
#include <functional>
#include <vector>

namespace WyLight {

/******************************************************************************/
/*!\cond
 * class - ControlNoThrow
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Hardware.
 *
 *
 * The ControlNoThrow class allows the user to control the Wifly_Light hardware.
 * This is a wrapper class to ::WyLight::Control to catch all exceptions from the
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

	class ControlNoThrow
	{
	public:

		/**
		 * Connect to a wifly device
		 * @param addr ipv4 address as 32 bit value in host byte order
		 * @param port number of the wifly device server in host byte order
		 */
		ControlNoThrow(uint32_t addr, uint16_t port);

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
		uint32_t BlReadFwVersion(uint16_t& version) const;

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
		 * Read the currently configured ssid from WyLight module
		 * @param ssid is the outputstring for the current ssid set in the RN-171 WLAN modul
		 * @return Indexed by ::WiflyError
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfGetSsid(std::string& ssid) const;

		/**
		 * Configurates the WyLight module as stand alone accesspoint. With accesspoint name you can change the ssid for this accesspoint.
		 * @param accesspointName 1 - 32 characters
		 * @return Indexed by ::WiflyError
		    <BR><B>FATAL_ERROR</B> in case of an error
		    <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfModuleAsSoftAP(const std::string& accesspointName = "Wifly_Light") const;

		/**
		 * Configurates the WyLight module as client for an existing wlan network with WPA2 protection
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @param name 1 - 32 characters, unique name which apperas in the broadcast message
		 * @return Indexed by ::WiflyError
		    <BR><B>FATAL_ERROR</B> in case of an error
		    <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfModuleForWlan(const std::string& phrase, const std::string& ssid, const std::string& name = "Wifly_Light") const;

		/**
		 * Reboot the modul. ATTENTION: You have to reconnect after a reboot
		 * @return Indexed by ::WiflyError
		        <BR><B>FATAL_ERROR</B> in case of an error
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfRebootWlanModule(void) const;

		/**
		 * Allows you to give every Wifly_Light device an unique name
		 * @param name 1 - 32 characters
		 * @return Indexed by ::WiflyError
		        <BR><B>FATAL_ERROR</B> in case of an error
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfSetDeviceId(const std::string &name) const;

		/**
		 * Wlan module performs a wifi scan and changes to the next free channel. This function can take some time.
		 * @return Indexed by ::WiflyError
		        <BR><B>FATAL_ERROR</B> in case of an error
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t ConfChangeWlanChannel(void) const;


		/**
		 *
		 */
		uint32_t ConfSetParameters(std::list<std::string> commands) const;


/* -------------------------- FIRMWARE METHODES -------------------------- */
		/**
		 * Wipe all commands from the WyLight script controller
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
		uint32_t FwGetVersion(uint16_t& output);

		/**
		 * Injects a LoopOff command into the wifly script controller
		 * @param numLoops number of rounds before termination of the loop, use 0 for infinite loops. To terminate an infinite loop you have to call \<FwClearScript\>
		 * @return Indexed by ::WiflyError
		        <BR><B>CONNECTION_TIMEOUT</B> if response timed out
		        <BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
		        <BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwLoopOff(const uint8_t numLoops);

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
		 * Sets all leds with different colors directly. This doesn't affect the WyLight script controller
		 * Example: to set the first led to yellow and the second to blue and all others to off use a \<buffer\> like this:
		 * buffer[] = {0xff, 0xff, 0x00, 0x00, 0x00, 0xff}; bufferLength = 6;
		 * @param buffer containing continouse rgb values r1g1b1r2g2b2...r32g32b32
		 * @return Indexed by ::WiflyError
		        <BR><B>CONNECTION_TIMEOUT</B> if response timed out
		        <BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
		        <BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwSetColorDirect(const std::vector<uint8_t> buffer);

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
		uint32_t FwSetFade(const uint32_t argb, const uint16_t fadeTime = 0, const uint32_t addr = 0xffffffff, const bool parallelFade = false);

		/**
		* Injects a gradient command into the wifly script controller
		* @param argb_1 is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff). This is the start color for the gradient.
		* @param argb_2 is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff). This is the end color for the gradient.
		* @param fadeTime in hundreths of a second. Use 0 to set color immediately, default = 0
		* @param parallelFade if true other fades are allowed in parallel with this fade
		* @param length is the number of led's from startposition to endposition
		* @param offset can be used to move the startposition of the gradient on the ledstrip
		* @return Indexed by ::WiflyError
		   <BR><B>CONNECTION_TIMEOUT</B> if response timed out
		   <BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed, or a invalid value for offset
		   <BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
		   <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwSetGradient(const uint32_t argb_1, const uint32_t argb_2, const uint16_t fadeTime = 0, const bool parallelFade = false, const uint8_t length = NUM_OF_LED, const uint8_t offset = 0);

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
		uint32_t FwSetWait(const uint16_t waitTime);

		/**
		 * Stops firmware and script controller execution and start the bootloader of the wifly device
		 * @return Indexed by ::WiflyError
		        <BR><B>CONNECTION_TIMEOUT</B> if response timed out
		        <BR><B>FATAL_ERROR</B> if command code of the response doesn't match the code of the request, or too many retries failed
		        <BR><B>SCRIPT_FULL</B> if script buffer in PIC firmware is full and request couldn't be executed
		        <BR><B>NO_ERROR</B> is returned if no error occurred
		 */
		uint32_t FwStartBl(void);

		/* ------------------------- VERSION EXTRACT METHODE ------------------------- */
		/**
		 * Methode to extract the firmware version from a hex file
		 * @return the version string from a given hex file
		 */
		uint32_t ExtractFwVersion(const std::string& pFilename, uint16_t& extractedFwVersion) const;

		friend class StartupManager;
	private:
		/**
		 * Converts all exceptions from ::WiflyControl to the relating ::WiflyError
		 */
		Control mControl;
		uint32_t Try(FwCommand&& cmd);
		uint32_t Try(const std::function<void(void)> call) const;
		uint32_t Try(const std::function<std::string(void)> call, std::string& returnString) const;
		uint32_t Try(const std::function<uint16_t(void)> call, uint16_t& returnValue) const;

	};
}
#endif /* #ifndef _WIFLYCONTROL_NOTHROW_H_ */
