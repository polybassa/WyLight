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
#include <iomanip>
#include <time.h>
#include <stdint.h>

using namespace std;

class WiflyControlCmd
{
	public:
		WiflyControlCmd(string name, string description = "' - missing description") : m_Name(name), m_Description(description) {};

		friend ostream& operator<< (ostream& stream, const WiflyControlCmd& ref) {
			stream << "'" << ref.m_Name << ref.m_Description;
			return stream;
		};

		const string& GetName(void) const { return m_Name; };
		virtual void Run(WiflyControl& control) const = 0;
	protected:
		const string m_Name;
		const string m_Description;
		void Print(const unsigned char* const pBuffer, const size_t size, const uint32_t address) const {
			for(size_t i = 0; i < size; i++) {
				if(0 == (i % 16)) {
					cout << endl << "0x" << setw(4) << setfill('0') << hex << int(address+i) << ": ";
				}
				cout << setw(2) << setfill('0') << hex << int(pBuffer[i]) << ' ';
			}
			cout << endl;
		}
		void PrintCrc(const unsigned char* const pBuffer, const size_t size, const uint32_t address) const {
			for(size_t i = 0; i < size; i++) {
				if(0 == (i % 2)) {
					cout << endl 
					<< "0x" << setw(4) << setfill('0') << hex 
					<< int(address+i * FLASH_ERASE_BLOCKSIZE) 
					<< " - " 
					<< "0x" << setw(4) << setfill('0') << hex 
					<< int(address+ ((i+2) * FLASH_ERASE_BLOCKSIZE) -1) 
					<< ": ";
				}
				cout << setw(2) << setfill('0') << hex << int(pBuffer[i]) << ' ';
			}
			cout << endl;
		}
};

class ControlCmdAddColor : public WiflyControlCmd
{
	public:
		ControlCmdAddColor(void) : WiflyControlCmd(
			string("addcolor"),
				string(" <addr> <rgb> <hour> <minute> <second>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000\n")
			+ string("    <hour> hour of date event\n")
			+ string("    <minute> minute of date event\n")
			+ string("    <second> second of date event")) {};

		virtual void Run(WiflyControl& control) const {
			string addr, color;
			uint64_t hour, minute, second;
			cin >> addr;
			cin >> color;
			cin >> hour;
			cin >> minute;
			cin >> second;
			cout << addr << " " << color << " " << hour << " " << minute << " " << second << endl; 
			control.FwAddColor(addr, color, hour, minute, second);
		};
};

class ControlCmdBlAutostartEnable : public WiflyControlCmd
{
	public:
		ControlCmdBlAutostartEnable(void) : WiflyControlCmd(
				string("enable_bl_autostart")) {};

		virtual void Run(WiflyControl& control) const
		{
			if(control.BlEnableAutostart())
			{
			    cout << endl <<"Bootloader Autostart enabled!"<<endl;
			}
			else
			{
			    cout << endl <<"Bootloader Autostart not enabled!"<<endl;
			}
		}	
};

class ControlCmdBlInfo : public WiflyControlCmd
{
	public:
		ControlCmdBlInfo(void) : WiflyControlCmd(
				string("bl_info"), 
				string("' - read bootloader information")) {};

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
			uint32_t address;
			size_t numBlocks;
			cin >> address;
			cin >> numBlocks;
			unsigned char buffer[(FLASH_SIZE / FLASH_ERASE_BLOCKSIZE) * 2];
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
			PrintCrc(buffer, bytesRead, address);
		};
};

class ControlCmdBlEraseFlash : public WiflyControlCmd
{
	public:
		ControlCmdBlEraseFlash(void) : WiflyControlCmd(
				string("erase_flash")) {};

		virtual void Run(WiflyControl& control) const
		{
			if(control.BlFlashErase())
			{
			    cout << endl <<"Erase complete flash succesful"<<endl;
			}
			else
			{
			    cout << endl <<"Erase complete flash failed"<<endl;
			}
		}	
};

class ControlCmdBlEraseEeprom : public WiflyControlCmd
{
	public:
		ControlCmdBlEraseEeprom(void) : WiflyControlCmd(
				string("erase_eeprom")) {};

		virtual void Run(WiflyControl& control) const
		{
			if(control.BlEepromErase())
			{
			    cout << endl <<"Erase complete eeprom succesful"<<endl;
			}
			else
			{
			    cout << endl <<"Erase complete eeprom failed"<<endl;
			}
		}	
};

class ControlCmdBlProgramFlash : public WiflyControlCmd
{
	public:
		ControlCmdBlProgramFlash(void) : WiflyControlCmd(
				string("program_flash"),
				string(" <hexFile>' \n ") + string("    <hexFile> path of hexfile to write")) {};

		virtual void Run(WiflyControl& control) const
		{
			string path;
			
			cin >> path;
					  
			if(control.BlProgramFlash(path))
			{
			    cout << endl <<"Program device flash succesful"<<endl;
			}
			else
			{
			    cout << endl <<"Program device flash failed"<<endl;
			}
		}	
};

class ControlCmdBlRead : public WiflyControlCmd
{
	public:
		ControlCmdBlRead(string name) : WiflyControlCmd(
				string("read_") + name, string(" <addr> <numBytes>'\n")
			+ string("    <addr> address where to start reading\n")
			+ string("    <numBytes> number of bytes to read")) {};

		const string m_Name;
		
		virtual size_t Read(WiflyControl& control, unsigned char* pBuffer, uint32_t address, const size_t numBytes) const = 0;

		virtual void Run(WiflyControl& control) const {
			uint32_t address;
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

			if(bytesRead != numBytes) 
			{
				cout << "Read " << m_Name << " failed" << endl;
			} 
			else 
			{
				Print(buffer, bytesRead, address);
			}
		};
};

class ControlCmdBlReadEeprom : public ControlCmdBlRead
{
	public:
		ControlCmdBlReadEeprom(void) : ControlCmdBlRead("eeprom") {};
		size_t Read(WiflyControl& control, unsigned char* pBuffer, uint32_t address, const size_t numBytes) const {
			return control.BlReadEeprom(pBuffer, address, numBytes);
		};
};

class ControlCmdBlReadFlash : public ControlCmdBlRead
{
	public:
		ControlCmdBlReadFlash(void) : ControlCmdBlRead("flash") {};
		size_t Read(WiflyControl& control, unsigned char* pBuffer, uint32_t address, const size_t numBytes) const {
			return control.BlReadFlash(pBuffer, address, numBytes);
		};
};

class ControlCmdBlRunApp : public WiflyControlCmd
{
	public:
		ControlCmdBlRunApp(void) : WiflyControlCmd(
				string("run_app"),
				string("' - start application and terminate bootloader"))
		{};

		virtual void Run(WiflyControl& control) const {
			cout << "Starting application... ";
			cout << (control.BlRunApp() ? "done." : "failed!") << endl;
		};
};

class ControlCmdStartBl : public WiflyControlCmd
{
	public:
		ControlCmdStartBl(void) : WiflyControlCmd(
				  string("start_bl"),
					string("' - start bootloader and terminate application")) {};
				  
		virtual void Run(WiflyControl& control) const {
			cout << "Starting bootloader... ";
			cout << (control.FwStartBl() ? "done." : "failed!") << endl;
		};
  
};

class ControlCmdPrintTracebuffer : public WiflyControlCmd
{
	public:
		ControlCmdPrintTracebuffer(void) : WiflyControlCmd(
				  string("print_tracebuffer"),
					string("' - displays content in tracebuffer of pic")) {};
				  
		virtual void Run(WiflyControl& control) const {
			cout << "Reading tracebuffer... ";
			control.FwPrintTracebuffer(std::cout);
		};
  
};

class ControlCmdClearScript : public WiflyControlCmd
{
	public:
		ControlCmdClearScript(void) : WiflyControlCmd(
				string("clear"),
				string("' - clear script buffer")) {};

		virtual void Run(WiflyControl& control) const {
			cout << "Clearing script buffer... ";
			cout << (control.FwClearScript() ? "done." : "failed!") << endl;

		};
};

class ControlCmdPrintCycletime : public WiflyControlCmd
{
	public:
		ControlCmdPrintCycletime(void) : WiflyControlCmd(
				string("print_cycletime"),
				string("' - prints all timevalues of internal methode execution times")) {};

		virtual void Run(WiflyControl& control) const {
			cout << "Transmitting command print cycletime... ";
			cout << (control.FwPrintCycletime(std::cout) ? "done." : "failed!") << endl;

		};
};

class ControlCmdPrintRtc : public WiflyControlCmd
{
	public:
		ControlCmdPrintRtc(void) : WiflyControlCmd(
				string("print_rtc"),
				string("' - prints the current time of realtime-clock modul on target")) {};

		virtual void Run(WiflyControl& control) const {
			cout << "Transmitting command print rtc... ";
			cout << (control.FwPrintRtc(std::cout) ? "done." : "failed!") << endl;

		};
};

class ControlCmdLoopOn : public WiflyControlCmd
{
	public:
		ControlCmdLoopOn(void) : WiflyControlCmd(
				string("loopon"),
				string("' - indicates the start of a loop")) {};

		virtual void Run(WiflyControl& control) const {
			cout << "Transmitting command loop on... ";
			cout << (control.FwLoopOn() ? "done." : "failed!") << endl;

		};
};

class ControlCmdLoopOff : public WiflyControlCmd
{
	public:
		ControlCmdLoopOff(void) : WiflyControlCmd(
				string("loopoff"),
				string(" <numLoops>'\n")
			+ string("    <numLoops> number of executions for the loop. Enter 0 for infinity loop. Maximum 255")) {};

		virtual void Run(WiflyControl& control) const {
			int numLoops;
			cin >> numLoops;
			cout << "Transmitting command loop off... ";
			cout << (control.FwLoopOff( (unsigned char)numLoops ) ? "done." : "failed!") << endl;
		};
};

class ControlCmdWait : public WiflyControlCmd
{
	public:
		ControlCmdWait(void) : WiflyControlCmd(
				string("wait"),
				string(" <time>'\n")
			+ string("    <time> the number of milliseconds the wait should take")) {};

		virtual void Run(WiflyControl& control) const {
			uint16_t waitTmms;
			cin >> waitTmms;
			cout << "Transmitting command wait... ";
			cout << (control.FwSetWait( waitTmms ) ? "done." : "failed!") << endl;
		};
};

class ControlCmdSetColor : public WiflyControlCmd
{
	public:
		ControlCmdSetColor(void) : WiflyControlCmd(
				string("setcolor"),
				string(" <addr> <rgb>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000")) {};

		virtual void Run(WiflyControl& control) const {
			string addr, color;
			cin >> addr;
			cin >> color;
			cout << "Transmitting command set color... ";
			cout << (control.FwSetColor(addr, color) ? "done." : "failed!") << endl;
		};
};

class ControlCmdSetFade : public WiflyControlCmd
{
	public:
		ControlCmdSetFade(void) : WiflyControlCmd(
				string("setfade"),
				string(" <addr> <rgb> <time>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000\n")
			+ string("    <time> the number of milliseconds the fade should take")) {};

		virtual void Run(WiflyControl& control) const {
			string addr, color;
			uint64_t timevalue;
			cin >> addr;
			cin >> color;
			cin >> timevalue;
			cout << "Transmitting command set fade... ";
			cout << (control.FwSetFade(addr, color, (uint16_t)timevalue, false) ? "done." : "failed!") << endl;
		};
};

class ControlCmdSetRtc : public WiflyControlCmd
{
	public:
		ControlCmdSetRtc(void) : WiflyControlCmd(
				string("setrtc"),
				string("' - set time of rtc in target to current systemtime")){};

		virtual void Run(WiflyControl& control) const {
			struct tm* timeinfo;
			time_t rawtime;
			
			
			rawtime = time(NULL);
			timeinfo = localtime(&rawtime);
			
			cout << "Transmitting current time... ";
			cout << (control.FwSetRtc(timeinfo) ? "done." : "failed!") << endl;
		};
};

class ControlCmdGetRtc : public WiflyControlCmd
{
	public:
		ControlCmdGetRtc(void) : WiflyControlCmd(
				string("getrtc"),
				string("' - get time of rtc in target")){};

		virtual void Run(WiflyControl& control) const {
			struct tm timeinfo;
			
			cout << "Getting target time... ";
			if(control.FwGetRtc(&timeinfo))
			{
			  cout << "done." << endl;
			  cout << "Current time at target device is: " << asctime(&timeinfo) << endl;
			}
			else 
			{			  
			  cout << "failed!" << endl;
			}
		};
};

class ControlCmdTest : public WiflyControlCmd
{
	public:
		ControlCmdTest(void) : WiflyControlCmd(
				string("test"),
				string("' - run test loop")) {};

		virtual void Run(WiflyControl& control) const {
			control.FwTest();
		};
};

static const WiflyControlCmd* s_Cmds[] = {
	new ControlCmdAddColor(),
	new ControlCmdBlAutostartEnable(),
	new ControlCmdBlInfo(),
	new ControlCmdBlCrcFlash(),
	new ControlCmdBlEraseEeprom(),
	new ControlCmdBlEraseFlash(),
	new ControlCmdBlProgramFlash(),
	new ControlCmdBlReadEeprom(),
	new ControlCmdBlReadFlash(),
	new ControlCmdBlRunApp(),
	new ControlCmdClearScript(),
	new ControlCmdSetColor(),
	new ControlCmdSetFade(),
	new ControlCmdStartBl(),
	new ControlCmdTest(),
	new ControlCmdPrintTracebuffer(),
	new ControlCmdPrintCycletime(),
	new ControlCmdPrintRtc(),
	new ControlCmdSetRtc(),
	new ControlCmdGetRtc(),
	new ControlCmdLoopOn(),
	new ControlCmdLoopOff(),
	new ControlCmdWait(),
//TODO implement on demand	ControlCmdBlWriteEeprom writeEeprom;
//TODO	ControlCmdBlWriteFlash writeFlash;
};

/**
 * Class to provide easy access to all available control commands
 */
class WiflyControlCmdBuilder
{
		/**
		 * Constant number of available control commands
		 */
		static const size_t s_NumCmds = sizeof(s_Cmds) / sizeof(s_Cmds[0]);

	public:
		/**
		 * Find command with the given name
		 * @param name of the command
		 * @return pointer to an command object for the given name or NULL in case of an error
		 */
		static const WiflyControlCmd* GetCmd(const string name) {
			for(size_t i = 0; i < s_NumCmds; i++) {
				if(0 == name.compare(s_Cmds[i]->GetName())) {
					return s_Cmds[i];
				}
			}
			return NULL;
		}

		/**
		 * Use this function for iteration over all commands, which are available.
		 * @param index of the command
		 * @return pointer to an command object or NULL if index is out of range
		 */
		static const WiflyControlCmd* GetCmd(size_t index) {
			if(index < s_NumCmds) {
				return s_Cmds[index];
			}
			return NULL;
		}
};
#endif /* #ifndef _WIFLYCONTROLCMD_H_ */

