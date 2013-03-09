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
	private:
		const TcpSocket mSock;
		const ComProxy mProxy;
		const TelnetProxy mTelnet;
		pthread_t mRecvThread;
		struct cmd_frame mCmdFrame;
		unsigned long ToARGB(const std::string& s) const;
		size_t BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize, bool doSync = true) const;
		WiflyResponse& FwSend(struct cmd_frame* pFrame, size_t length, WiflyResponse& response) const;
		
	public:
		static const std::string LEDS_ALL;
		WiflyControl(uint32_t addr, uint16_t port);
		
		/** ----------------------------- BOOTLOADER METHODES ----------------------------- **/
		void BlFlashErase(unsigned int endAddress, const size_t numPages, bool doSync) const;
		void BlFlashErase(void) const;
		void BlEepromErase(void) const;
		
		size_t BlReadCrcFlash(unsigned char* pBuffer, unsigned int address, const size_t numBytes) const;
		size_t BlReadEeprom(unsigned char* pBuffer, unsigned int address, size_t numBytes) const;
		size_t BlReadFlash(unsigned char* pBuffer, unsigned int address, size_t numBytes) const;
		void BlReadInfo(BlInfo& info) const;
		
		void BlWriteFlash(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const;
		void BlWriteEeprom(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const;
		
		void BlProgramFlash(const std::string& Filename);
		void BlRunApp(void) const;
		void BlEnableAutostart(void) const;
		
		/** ----------------------------- Telnet METHODES ----------------------------- **/

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

		/**
		 * Update the Wifly module firmware
		 * @return true, if reboot was triggered successfully, connection will then be lost!
		 */
		bool ConfUpdate(void) const;
		
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
		void FwPrintCycletime(WiflyResponse&);
		void FwPrintTracebuffer(WiflyResponse&);
		void FwPrintFwVersion(WiflyResponse&);
		void FwStartBl(WiflyResponse&);
		
		void FwSetRtc(WiflyResponse&, struct tm* timeValue);
		void FwGetRtc(WiflyResponse&);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
