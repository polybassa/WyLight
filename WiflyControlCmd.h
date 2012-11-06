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

class ControlCmdBlCrcFlash : public WiflyControlCmd
{
	public:
		ControlCmdBlCrcFlash(void) : WiflyControlCmd("crc_flash") {};
		virtual void Run(WiflyControl& control) const {
			unsigned int address;
			size_t numBlocks;
			cin >> address;
			cin >> numBlocks;
			unsigned char buffer[0xffff / FLASH_READ_BLOCKSIZE * 2];
			if(sizeof(buffer) / 2 < numBlocks)
			{
				cout << "Read CRC failed. Too many CRCs requested" << endl;
				return;
			}

			size_t bytesRead = control.BlReadCrcFlash(buffer, address, numBlocks);
			if(2 * numBlocks != bytesRead)
			{
				cout << "Read CRC failed" << endl;
				return;
			}
			Print(buffer, bytesRead, 0);
		};
};

class ControlCmdBlEraseFlash : public WiflyControlCmd
{
	public:
		ControlCmdBlEraseFlash(void) : WiflyControlCmd(
				string("'erase_flash'")) {};

		virtual void Run(WiflyControl& control) const
		{
			BlInfo info;
			if(sizeof(info) != control.BlReadInfo(info))
			{
				cout << "Erase flash failed, couldn't determine bootloader location" << endl;
				return;
			}

			// bootloader is expected to reside at the end of the flash
			unsigned int address = info.GetAddress() - 1;
			size_t numPages = (address + FLASH_ERASE_BLOCKSIZE - 1) / FLASH_ERASE_BLOCKSIZE;
			unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
			size_t bytesRead = control.BlFlashErase(buffer, address, numPages);
			if((bytesRead != 1) || (0x03 != buffer[0])) {
				cout << "Erase flash failed, for " << numPages << " pages below 0x" << hex << address << endl;
			}
		};
};

class ControlCmdBlRead : public WiflyControlCmd
{
	public:
		ControlCmdBlRead(string name) : WiflyControlCmd(
				string("'read_") + name + string(" <addr> <numBytes>'\n")
			+ string("    <addr> address where to start reading\n")
			+ string("    <numBytes> number of bytes to read")), m_Name(name) {};

		const string m_Name;
		
		virtual size_t Read(WiflyControl& control, unsigned char* pBuffer, unsigned int address, const size_t numBytes) const = 0;

		virtual void Run(WiflyControl& control) const {
			unsigned int address;
			size_t numBytes;
			cin >> address;
			cin >> numBytes;
			unsigned char buffer[0x10000];
			if(sizeof(buffer) < numBytes)
			{
				cout << "Read " << m_Name << " failed. Too many bytes requested" << endl;
				return;
			}

			size_t bytesRead = Read(control, buffer, address, numBytes);

			if(bytesRead != numBytes) {
				cout << "Read " << m_Name << " failed" << endl;
			} else {
				Print(buffer, bytesRead, address);
			}
		};
};

class ControlCmdBlReadEeprom : public ControlCmdBlRead
{
	public:
		ControlCmdBlReadEeprom(void) : ControlCmdBlRead("eeprom") {};
		size_t Read(WiflyControl& control, unsigned char* pBuffer, unsigned int address, const size_t numBytes) const {
			return control.BlReadEeprom(pBuffer, address, numBytes);
		};
};

class ControlCmdBlReadFlash : public ControlCmdBlRead
{
	public:
		ControlCmdBlReadFlash(void) : ControlCmdBlRead("flash") {};
		size_t Read(WiflyControl& control, unsigned char* pBuffer, unsigned int address, const size_t numBytes) const {
			return control.BlReadFlash(pBuffer, address, numBytes);
		};
};

class ControlCmdBlRunApp : public WiflyControlCmd
{
	public:
		ControlCmdBlRunApp(void) : WiflyControlCmd(
				string("'run_app' - start application and terminate bootloader")) {};

		virtual void Run(WiflyControl& control) const {
			cout << "Starting application... ";
			cout << (control.BlRunApp() ? "done." : "failed!") << endl;
		};
};

class ControlCmdStartBl : public WiflyControlCmd
{
	public:
		ControlCmdStartBl(void) : WiflyControlCmd(
				  string("'start_bl' - start bootloader and terminate application")) {};
				  
		virtual void Run(WiflyControl& control) const {
			cout << "Starting bootloader... ";
			control.StartBl();
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
			} else if("crc_flash" == name) {
				return new ControlCmdBlCrcFlash();
			} else if("erase_flash" == name) {
				return new ControlCmdBlEraseFlash();
			} else if("read_eeprom" == name) {
				return new ControlCmdBlReadEeprom();
			} else if("read_flash" == name) {
				return new ControlCmdBlReadFlash();
			} else if("run_app" == name) {
				return new ControlCmdBlRunApp();
			} else if("setcolor" == name) {
				return new ControlCmdSetColor();
			} else if("setfade" == name) {
				return new ControlCmdSetFade();
			} else if("start_bl" == name) {
				return new ControlCmdStartBl();
			}
			return NULL;
		}
};
#endif /* #ifndef _WIFLYCONTROLCMD_H_ */

