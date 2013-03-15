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

#ifndef _WIFLYCONTROL_H_
#define _WIFLYCONTROL_H_

#include <string>
#include <time.h>
#include "ComProxy.h"
#include "wifly_cmd.h"
#include "BlRequest.h"
#include "intelhexclass.h"
#include "error.h"
#include "TelnetProxy.h"
#include "WiflyControlException.h"
#include "WiflyControlResponse.h"

class WiflyControl
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
		WiflyControl(uint32_t addr, uint16_t port);
		
/** ------------------------- BOOTLOADER METHODES ------------------------- **/
		/**
		 * Instructs the bootloader to erase the whole eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 */
		void BlEepromErase(void) const;

		/**
		 * TODO
		 */
		void BlEnableAutostart(void) const;
		
		/**
		 * Instructs the bootloader to erase the whole flash which is not occupied
		 * by the bootloader itself.
		 * The wifly device has to be in bootloader mode for this command.
		 */
		void BlFlashErase(void) const;

		/**
		 * Instructs the bootloader to create crc-16 checksums for the content of
		 * the specified flash area. TODO crc values are in little endian byte order
		 * The wifly device has to be in bootloader mode for this command.
		 * @param pBuffer pointer to a buffer for the resulting 16bit crc values
		 * @param address crc generation starts from this flash address
		 * @param numBytes size of the flash area for which the crc are calculated
		 * @return the number of bytes read (result / 2 = number of crc values)
		 */
		size_t BlReadCrcFlash(uint8_t* pBuffer, uint32_t address, uint16_t numBytes) const;

		/**
		 * Instructs the bootloader to read the specified memory area of the eeprom.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param pBuffer destination for the copy of the eeprom content, should be at least <numBytes> wide
		 * @param address start of the eeprom region to read
		 * @param numBytes size of the eeprom region to read
		 * @return the number of bytes read
		 */
		size_t BlReadEeprom(uint8_t* pBuffer, uint32_t address, size_t numBytes) const;

		/**
		 * Instructs the bootloader to read the specified memory area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param pBuffer destination for the copy of the flash content, should be at least <numBytes> wide
		 * @param address start of the flash region to readRequest
		 * @param numbytes size of the flash region to read
		 * @return the number of bytes read
		 */
		size_t BlReadFlash(uint8_t* pBuffer, uint32_t address, size_t numBytes) const;

		/**
		 * Instructs the bootloader to return a struct of bootloader informations
		 * like bootloader version, flash and eeprom size. see <BlInfo> for details.
		 * The wifly device has to be in bootloader mode for this command.
		 */
		void BlReadInfo(BlInfo& info) const;
		
		/**
		 * Instructs the bootloader to update the wifly device with new firmware.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param filename path to the *.hex file containing the new firmware
		 */
		void BlProgramFlash(const std::string& filename);

		/**
		 * Instructs the bootloader to start the wifly device firmware.
		 * The wifly device has to be in bootloader mode for this command.
		 */
		void BlRunApp(void) const;

/** --------------------- WLAN CONFIGURATION METHODES --------------------- **/
		/**
		 * Read the currently configured ssid from Wifly module
		 * @return an empty string or the ssid
		 */
		std::string ConfGetSsid(void) const;

		/**
		 * Set the Wifly module communication parameters to defaults
		 * @return false, in case of an error
		 */
		bool ConfSetDefaults(void) const;

		/**
		 * Set the Wifly module wlan connection parameters
		 * @param phrase WPA2 passphrase 1 - 63 characters
		 * @param ssid 1 - 32 characters
		 * @return false, in case of an error
		 */
		bool ConfSetWlan(const std::string& phrase, const std::string& ssid) const;
		
/** -------------------------- FIRMWARE METHODES -------------------------- **/
		/**
		 * Wipe all commands from the Wifly script controller
		 * @param response will be modified according to the success of this operation
		 */
		void FwClearScript(WiflyResponse& response);
		
		/**
		 * Reads the cycletimes from wifly device and stores them into the response object
		 * @param response reference to an object to store the read cyletimes
		 * @return cycletimes from the wifly device stored to the referenced CycletimeResponse
		 */
		void FwGetCycletime(CycletimeResponse& response);

		/**
		 * Reads the current rtc time from the wifly device
		 *
		 */
		void FwGetRtc(RtcResponse& response);

		/**
		 * Reads the tracebuffer from wifly device and stores the data into the response object
		 * @param response reference to an object to store the read tracebuffer content
		 * @return tracebuffer content from the wifly device stored to the referenced TracebufferResponse
		 */
		void FwGetTracebuffer(TracebufferResponse& response);

		/**
		 * Reads the firmware version currently running on the wifly device.
		 * @param response reference to an object to store the read version number
		 * @return the wifly device version number stored to the referenced FirmwareVersionResponse
		 */
		void FwGetVersion(FirmwareVersionResponse& response);

		/**
		 * Injects a LoopOn command into the wifly script controller
		 * @param response will be modified according to the success of this operation
		 */
		void FwLoopOn(WiflyResponse& response);

		/**
		 * Injects a LoopOff command into the wifly script controller
		 * @param response will be modified according to the success of this operation
		 * @param numLoops number of rounds before termination of the loop, use 0 for infinite loops. To terminate an infinite loop you have to call <FwClearScript>
		 */
		void FwLoopOff(WiflyResponse& response, uint8_t numLoops);

		/**
		 * Sets all leds with different colors directly. This doesn't affect the Wifly script controller
		 * Example: to set the first led to yellow and the second to blue and all others to off use a <pBuffer> like this:
		 * pBuffer[] = {0xff, 0xff, 0x00, 0x00, 0x00, 0xff}; bufferLength = 6;
		 * @param response will be modified according to the success of this operation
		 * @param pBuffer containing continouse rgb values r1g1b1r2g2b2...r32g32b32
		 * @param bufferLength number of bytes in <pBuffer> usally 32 * 3 bytes
		 */
		void FwSetColorDirect(WiflyResponse& response, uint8_t* pBuffer, size_t bufferLength);
		
		/**
		 * Injects a fade command into the wifly script controller
		 * @param response will be modified according to the success of this operation
		 * @param argb is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff) f.e.
		 *        black(  0,  0,  0) as argb is 0xff000000
		 *        green(  0,255,  0) as argb is 0xff00ff00
		 *        white(255,255,255) as argb is 0xffffffff
		 * @param fadeTmms fading time in milliseconds use 0 to set color immediately, default = 0
		 * @param addr bitmask of leds which should be effected by this command, set bit to 1 to affect the led, default 0xffffffff
		 * @param parallelFade if true other fades are allowed in parallel with this fade
		 */
		void FwSetFade(WiflyResponse& response, uint32_t argb, uint16_t fadeTmms = 0, uint32_t addr = 0xffffffff, bool parallelFade = false);

		/**
		 * Injects a fade command into the wifly script controller
		 * @param response will be modified according to the success of this operation
		 * @param rgb is a hex string representation of a rgb value without leading '0x'
		 *        black "0"
		 *        green "ff00"
		 *        white "ffffff"
		 * @param fadeTmms fading time in milliseconds use 0 to set color immediately, default = 0
		 * @param addr is a hex string representation of a 32 bit mask without leading '0x' of leds which should be effected by this comman.
		 *        all leds          "ffffffff" (default)
		 *        first three leds  "7"
		 *        only the last led "80000000"
		 * @param parallelFade if true other fades are allowed in parallel with this fade
		 */
		void FwSetFade(WiflyResponse& response, const std::string& rgb, uint16_t fadeTmms = 0, const std::string& addr = LEDS_ALL, bool parallelFade = false);

		/**
		 * Sets the rtc clock of the wifly device to the specified time.
		 * The wifly device has to be in firmware mode for this command.
		 * @param response will be modified according to the success of this operation.
		 * @param timeValue pointer to a posix tm struct containing the new time
		 */
		void FwSetRtc(SimpleResponse& response, struct tm* timeValue);
		
		/**
		 * Injects a wait command into the wifly script controller.
		 * This causes the script processing to wait before executing the next command fir the specified duration
		 * @param response will be modified according to the success of this operation
		 * @param waitTmms timme in milliseconds to wait until execution of the next command
		 */
		void FwSetWait(WiflyResponse& response, uint16_t waitTmms);

		/**
		 * Stops firmware and script controller execution and start the bootloader of the wifly device
		 * @param response will be modified according to the success of this operation.
		 */
		void FwStartBl(SimpleResponse& response);
		

		//TODO move this test functions to the integration test 
		void FwTest(void);
		void FwStressTest(void);


/** ------------------------- PRIVATE DECLARATIONS ------------------------- **/
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
		 * Internal command frame used to send to the wifly device, this member should be removed and replaced by local variables.
		 */
		struct cmd_frame mCmdFrame;

		/**
		 * Instructs the bootloader to erase the specified area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param endAddress address of the block to delete
		 * @param numPages number of pages in a block
		 */
		void BlFlashErase(const uint32_t endAddress, const uint8_t numPages) const;

		/**
		 * Send a request to the bootloader and read his response into pResponse
		 * @param request reference to a bootloader requested
		 * @param pResponse a buffer for the response of the bootloader
		 * @param responseSize sizeof of the <pResponse> buffer in bytes
		 * @param doSync if set to 'true' the uart sync is issued before data transfer default = true
		 * @return the number of bytes the bootloader send back in his response
		 */
		size_t BlRead(BlRequest& request, uint8_t* pResponse, const size_t responseSize, bool doSync = true) const;

		/**
		 * Instructs the bootloader of the wifly device to write data to the eeprom.
		 * @param address in eeprom
		 * @param pBuffer containing the new data for eeprom
		 * @param bufferLength number of bytes to write to eeprom
		 */
		void BlWriteEeprom(uint32_t address, uint8_t* pBuffer, size_t bufferLength) const;

		/**
		 * Instructs the bootloader of the wifly device to write data to the flash.
		 * @param address in flash
		 * @param pBuffer containing the new data for flash
		 * @param bufferLength number of bytes to write to flash
		 */
		void BlWriteFlash(uint32_t address, uint8_t* pBuffer, size_t bufferLength) const;

		/**
		 * Sends a wifly command frame to te wifly device
		 * @param pFrame pointer to the frame, which should be send
		 * @param length number of bytes on the <pFrame>
		 * @param response will be modified according to the success of this operation
		 */		
		WiflyResponse& FwSend(struct cmd_frame* pFrame, size_t length, WiflyResponse& response) const;

/** ------------------ friendships for unittesting only ------------------- **/
friend size_t ut_WiflyControl_BlEepromWrite(void);
friend size_t ut_WiflyControl_BlFlashWrite(void);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
