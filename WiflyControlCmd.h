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

#ifndef _WIFLYCONTROLCMD_H_
#define _WIFLYCONTROLCMD_H_
#include <iostream>
#include <string>

using namespace std;

class WiflyControlCmd
{
	public:
		WiflyControlCmd(string description) : m_CmdDescription(description) {};
		friend ostream& operator<< (ostream& stream, const WiflyControlCmd& ref) {
			stream << ref.m_CmdDescription;
			return stream;
		};
		virtual void Run(const WiflyControl& control) const = 0;
	protected:
		const string m_CmdDescription;
		void Print(const unsigned char* const pBuffer, const size_t size, const unsigned int address) const {
			for(size_t i = 0; i < size; i++) {
				if(0 == (i % 16)) {
					cout << endl << "0x" << hex << int(address+i) << ": ";
				}
				cout << hex << int(pBuffer[i]) << ' ';
			}
			cout << endl;
		}
};

class ControlCmdReadEeprom : public WiflyControlCmd
{
	public:
		ControlCmdReadEeprom(void) : WiflyControlCmd(
				string("'read_eeprom <addr> <numBytes>'\n")
			+ string("    <addr> eeprom address where to start reading\n")
			+ string("    <numBytes> number of bytes to read")) {};
		virtual void Run(const WiflyControl& control) const {
			unsigned int address;
			size_t numBytes;
			cin >> address;
			cin >> numBytes;
			unsigned char buffer[1024];
			size_t bytesRead = control.BlReadEeprom(buffer, address, numBytes);
			if(bytesRead != numBytes) {
				cout << "Read eeprom failed" << endl;
			} else {
				Print(buffer, bytesRead, address);
			}
		};
};

class ControlCmdReadFlash : public WiflyControlCmd
{
	public:
		ControlCmdReadFlash(void) : WiflyControlCmd(
				string("'read_flash <addr> <numBytes>'\n")
			+ string("    <addr> flash address where to start reading\n")
			+ string("    <numBytes> number of bytes to read")) {};
		virtual void Run(const WiflyControl& control) const {
			unsigned int address;
			size_t numBytes;
			cin >> address;
			cin >> numBytes;
			unsigned char buffer[0x10000];
			size_t bytesRead = control.BlReadFlash(buffer, address, numBytes);
			if(bytesRead != numBytes) {
				cout << "Read flash failed" << endl;
			} else {
				Print(buffer, bytesRead, address);
			}
		};
};

class WiflyControlCmdBuilder
{
	public:
		static const WiflyControlCmd* GetCmd(string name) {
			if("read_eeprom" == name) {
				return new ControlCmdReadEeprom();
			}
			if("read_flash" == name) {
				return new ControlCmdReadFlash();
			}
			return NULL;
		}
};
#endif /* #ifndef _WIFLYCONTROLCMD_H_ */

