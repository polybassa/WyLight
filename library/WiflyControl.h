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
		
		/** ------------------------------ FIRMWARE METHODES ------------------------------ **/

		/**
		 * Wipe all commands from the Wifly script controller
		 * @param response will be modified according to the success of this operation
		 */
		void FwClearScript(WiflyResponse& response);

		/**
		 * Inject a LoopOn command into the Wifly script controller
		 * @param response will be modified according to the success of this operation
		 */
		void FwLoopOn(WiflyResponse&);

		/**
		 * Inject a LoopOff command into the Wifly script controller
		 * @param response will be modified according to the success of this operation
		 * @param numLoops number of rounds before termination of the loop, use 0 for infinite loops. To terminate an infinite loop you have to call <FwClearScript>
		 */
		void FwLoopOff(WiflyResponse&, uint8_t numLoops);

		/**
		 * Set all leds with different colors directly. This doesn't affect the Wifly script controller
		 */
		void FwSetColorDirect(WiflyResponse&, unsigned char* pBuffer, size_t bufferLength);
		
		/**
		 * Sends a fade command to the connected wifly module
		 * @param response reference to a response object, FwSetFade will modify the state of the referenced object according to the transmit status.
		 * @param argb is a 32 bit rgb value with unused alpha channel (set alpha always to 0xff) f.e.
		 *        black(  0,  0,  0) as argb is 0xff000000
		 *        green(  0,255,  0) as argb is 0xff00ff00
		 *        white(255,255,255) as argb is 0xffffffff
		 * @param fadeTmms fading time in milliseconds use 0 to set color immediately, default = 0
		 * @param addr bitmask of leds which should be effected by this command, set bit to 1 to affect the led, default 0xffffffff
		 * @param parallelFade if true other fades are allowed in parallel
		 */
		void FwSetFade(WiflyResponse& response, uint32_t argb, uint16_t fadeTmms = 0, uint32_t addr = 0xffffffff, bool parallelFade = false);
		void FwSetFade(WiflyResponse&, const std::string& rgba, uint16_t fadeTmms = 0, const std::string& addr = LEDS_ALL, bool parallelFade = false);
		
		void FwSetWait(WiflyResponse&, unsigned short waitTmms);
		
		void FwTest(void);
		void FwStressTest(void);
		void FwPrintCycletime(WiflyResponse&);
		void FwPrintTracebuffer(WiflyResponse&);
		void FwPrintFwVersion(WiflyResponse&);
		void FwStartBl(WiflyResponse&);
		
		void FwSetRtc(WiflyResponse&, struct tm* timeValue);
		void FwGetRtc(WiflyResponse&);

	private:
		const TcpSocket mSock;
		const ComProxy mProxy;
		const TelnetProxy mTelnet;
		pthread_t mRecvThread;
		struct cmd_frame mCmdFrame;
		unsigned long ToARGB(const std::string& s) const;

		/**
		 * Instructs the bootloader to erase the specified area of the flash.
		 * The wifly device has to be in bootloader mode for this command.
		 * @param endAddress address of the block to delete
		 * @param numPages number of pages in a block
		 */
		void BlFlashErase(const uint32_t endAddress, const uint8_t numPages) const;
		size_t BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize, bool doSync = true) const;
		void BlWriteFlash(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const;
		void BlWriteEeprom(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const;
		WiflyResponse& FwSend(struct cmd_frame* pFrame, size_t length, WiflyResponse& response) const;

/** ------------------ friendships for unittesting only ------------------- **/
friend size_t ut_WiflyControl_BlEepromWrite(void);
friend size_t ut_WiflyControl_BlFlashWrite(void);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
