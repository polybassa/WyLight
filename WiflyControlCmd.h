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
		virtual void Run(WiflyControl& control) const = 0;
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

class ControlCmdAddColor : public WiflyControlCmd
{
	public:
		ControlCmdAddColor(void) : WiflyControlCmd(
				string("'addcolor <addr> <rgb> <hour> <minute> <second>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000\n")
			+ string("    <hour> hour of date event\n")
			+ string("    <minute> minute of date event\n")
			+ string("    <second> second of date event")) {};

		virtual void Run(WiflyControl& control) const {
			string addr, color;
			unsigned long hour, minute, second;
			cin >> addr;
			cin >> color;
			cin >> hour;
			cin >> minute;
			cin >> second;
			cout << addr << " " << color << " " << hour << " " << minute << " " << second << endl; 
			control.AddColor(addr, color, hour, minute, second);
		};
};

class ControlCmdBlInfo : public WiflyControlCmd
{
	public:
		ControlCmdBlInfo(void) : WiflyControlCmd(
				string("'bl_info' - read bootloader information")) {};

		virtual void Run(WiflyControl& control) const {
			BlInfo info;
			if(sizeof(info) == control.BlReadInfo(info))
			{
				info.Print();
			}
			else
			{
				std::cout << "Read bootloader info failed" << endl;
			}
		};
};

class ControlCmdReadEeprom : public WiflyControlCmd
{
	public:
		ControlCmdReadEeprom(void) : WiflyControlCmd(
				string("'read_eeprom <addr> <numBytes>'\n")
			+ string("    <addr> eeprom address where to start reading\n")
			+ string("    <numBytes> number of bytes to read")) {};

		virtual void Run(WiflyControl& control) const {
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

		virtual void Run(WiflyControl& control) const {
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

class ControlCmdSetColor : public WiflyControlCmd
{
	public:
		ControlCmdSetColor(void) : WiflyControlCmd(
				string("'setcolor <addr> <rgb>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000")) {};

		virtual void Run(WiflyControl& control) const {
			string addr, color;
			cin >> addr;
			cin >> color;
			control.SetColor(addr, color);
		};
};

class ControlCmdSetFade : public WiflyControlCmd
{
	public:
		ControlCmdSetFade(void) : WiflyControlCmd(
				string("'setfade <addr> <rgb> <time>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000\n")
			+ string("    <time> the number of milliseconds the fade should take")) {};

		virtual void Run(WiflyControl& control) const {
			string addr, color;
			unsigned long timevalue;
			cin >> addr;
			cin >> color;
			cin >> timevalue;
			control.SetFade(addr, color, (unsigned short)timevalue * 1024);
		};
};

class WiflyControlCmdBuilder
{
	public:
		static const WiflyControlCmd* GetCmd(string name) {
			if("addcolor" == name) {
				return new ControlCmdAddColor();
			} else if("bl_info" == name) {
				return new ControlCmdBlInfo();
			} else if("read_eeprom" == name) {
				return new ControlCmdReadEeprom();
			} else if("read_flash" == name) {
				return new ControlCmdReadFlash();
			} else if("setcolor" == name) {
				return new ControlCmdSetColor();
			 }else if("setfade" == name) {
				return new ControlCmdSetFade();
			}
			return NULL;
		}
};
#endif /* #ifndef _WIFLYCONTROLCMD_H_ */

