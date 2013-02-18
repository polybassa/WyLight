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

class WiflyControl
{
	private:
		const ComProxy mProxy;
		pthread_t mRecvThread;
		struct cmd_frame mCmdFrame;
		unsigned long ToRGBA(std::string& s) const;
		size_t BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize, bool doSync = true) const;
		int FwSend(struct cmd_frame* pFrame, size_t length, unsigned char* pResponse, size_t responseSize) const;
		
	public:
		WiflyControl(unsigned long addr, unsigned short port);
		
		/** ----------------------------- BOOTLOADER METHODES ----------------------------- **/
		size_t BlFlashErase(unsigned char* pBuffer, unsigned int endAddress, const size_t numPages, bool doSync) const;
		bool BlFlashErase(void) const;
		bool BlEepromErase(void) const;
		
		size_t BlReadCrcFlash(unsigned char* pBuffer, unsigned int address, const size_t numBytes) const;
		size_t BlReadEeprom(unsigned char* pBuffer, unsigned int address, size_t numBytes) const;
		size_t BlReadFlash(unsigned char* pBuffer, unsigned int address, size_t numBytes) const;
		size_t BlReadInfo(BlInfo& info) const;
		
		bool BlWriteFlash(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const;
		bool BlWriteEeprom(unsigned int address, unsigned char* pBuffer, size_t bufferLength) const;
		
		bool BlProgramFlash(const std::string& Filename);
		ErrorCode BlRunApp(void) const;
		bool BlEnableAutostart(void) const;
		
		/** ----------------------------- Telnet METHODES ----------------------------- **/
		bool ConfSetDefaults(void) const;
		bool ConfSetWlan(const std::string& phrase, const std::string& ssid) const;
		
		
		/** ------------------------------ FIRMWARE METHODES ------------------------------ **/
		ErrorCode FwClearScript(void);
		ErrorCode FwLoopOn(void);
		ErrorCode FwLoopOff(unsigned char numLoops);
		

		/**
			rgba is a 32 Bit rgb value with alpha channel. Alpha is unused, but easier to handle
			f.e. red(255, 0, 0) is in rgba as: 0xff000000
				 white(255, 255, 255) is in rgba as: 0xffffff00
		**/
		ErrorCode FwSetColor(unsigned long addr, unsigned long rgba);
		ErrorCode FwSetColorDirect(unsigned char* pBuffer, size_t bufferLength);
		
		ErrorCode FwSetFade(unsigned long addr, unsigned long rgba, unsigned short fadeTmms, bool parallelFade);
		ErrorCode FwSetFade(std::string& addr, std::string& rgba, unsigned short fadeTmms, bool parallelFade);
		
		ErrorCode FwSetWait(unsigned short waitTmms);
		
		void FwTest(void);
		ErrorCode FwPrintCycletime(std::ostream& out);
		ErrorCode FwPrintTracebuffer(std::ostream& out);
		ErrorCode FwPrintFwVersion(std::ostream& out);
		ErrorCode FwStartBl(void);
		
		ErrorCode FwSetRtc(struct tm* timeValue);
		ErrorCode FwGetRtc(struct tm* timeValue);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
