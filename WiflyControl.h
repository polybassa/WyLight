/**
		Copyright (C) 2012 Nils Weiss, Patrick Bruenn.

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
#include <pthread.h>
#include "ClientSocket.h"
#include "wifly_cmd.h"
#include "BlRequest.h"

class WiflyControl
{
	private:
		const ClientSocket* mSock;
		pthread_t mRecvThread;
		struct cmd_frame mCmdFrame;
		unsigned long ToRGBA(std::string& s) const;
		
	public:
		WiflyControl(const char* pAddr, short port, bool useTcp);
		void Receiving(void) const;
		/**
			rgba is a 32 Bit rgb value with alpha channel. Alpha is unused, but easier to handle
			f.e. red(255, 0, 0) is in rgba as: 0xff000000
				 white(255, 255, 255) is in rgba as: 0xffffff00
		**/
		void AddColor(unsigned long addr, unsigned long rgba, unsigned char hour, unsigned char minute, unsigned char second);
		void AddColor(std::string& addr, std::string& rgba, unsigned char hour, unsigned char minute, unsigned char second);
		size_t BlRead(BlRequest& req, unsigned char* pResponse, const size_t responseSize) const;
		size_t BlReadEeprom(unsigned char* pBuffer, unsigned int address, const size_t numBytes) const;
		size_t BlReadFlash(unsigned char* pBuffer, unsigned int address, const size_t numBytes) const;
		size_t BlReadInfo(BlInfo& info);
		void SetColor(unsigned long addr, unsigned long rgba);
		void SetColor(std::string& addr, std::string& rgba);
		void SetFade(unsigned long addr, unsigned long rgba, unsigned short fadeTmms);
		void SetFade(std::string& addr, std::string& rgba, unsigned short fadeTmms);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
