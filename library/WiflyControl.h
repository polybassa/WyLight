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
		unsigned long ToRGBA(std::string& s) const;
		size_t BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize, bool doSync = true) const;
		WiflyResponse& FwSend(struct cmd_frame* pFrame, size_t length, WiflyResponse& response) const;
		
	public:
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
		std::string ConfGetSsid(void) const;
		bool ConfSetDefaults(void) const;
		bool ConfSetWlan(const std::string& phrase, const std::string& ssid) const;
		bool ConfUpdate(void) const;
		
		/** ------------------------------ FIRMWARE METHODES ------------------------------ **/
		void FwClearScript(WiflyResponse&);
		void FwLoopOn(WiflyResponse&);
		void FwLoopOff(WiflyResponse&, unsigned char numLoops);
		

		/**
			rgba is a 32 Bit rgb value with alpha channel. Alpha is unused, but easier to handle
			f.e. red(255, 0, 0) is in rgba as: 0xff000000
				 white(255, 255, 255) is in rgba as: 0xffffff00
		**/
		void FwSetColorDirect(WiflyResponse&, unsigned char* pBuffer, size_t bufferLength);
		
		void FwSetFade(WiflyResponse&, unsigned long addr, unsigned long rgba, unsigned short fadeTmms = 0, bool parallelFade = false);
		void FwSetFade(WiflyResponse&, std::string& addr, std::string& rgba, unsigned short fadeTmms = 0, bool parallelFade = false);
		
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
