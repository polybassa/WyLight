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

#ifndef _WIFLYCONTROLCMD_H_
#define _WIFLYCONTROLCMD_H_
#include "FwResponse.h"
#include "trace.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>
#include <stdint.h>

//TODO remove this dependencies!!!
using std::cin;
using std::cout;
using std::hex;
using std::setfill;
using std::setw;
using std::endl;
using std::string;
using std::ostream;

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

#define TRY_CATCH_COUT(X) \
	try { \
		(X); \
		cout << "done.\n"; \
	} catch(std::exception& e) { \
		cout << "failed! because of: " << e.what() << '\n'; \
	}

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
		void Print(std::istream& in, const size_t size, const uint32_t address) const {
			for(size_t i = 0; i < size; i++) {
				if(0 == (i % 16)) {
					cout << endl << "0x" << setw(4) << setfill('0') << hex << int(address+i) << ": ";
				}
				uint8_t b;
				in >> b;
				cout << setw(2) << setfill('0') << hex << int(b) << ' ';
			}
			cout << endl;
		}
		void PrintCrc(std::istream& in, const size_t size, const uint32_t address) const {
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
				uint8_t b;
				in >> b;
				cout << setw(2) << setfill('0') << hex << int(b) << ' ';
			}
			cout << endl;
		}
};

class ControlCmdBlEnableAutostart : public WiflyControlCmd
{
	public:
		ControlCmdBlEnableAutostart(void) : WiflyControlCmd(
				string("bl_enable_autostart")) {};

		virtual void Run(WiflyControl& control) const
		{
			cout << "\nBL: Enabling autostart... ";
			TRY_CATCH_COUT(control.BlEnableAutostart());
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
			try
			{
				control.BlReadInfo(info);
				info.Print();
			}
			catch(FatalError& e)
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
			try
			{
				std::stringstream mStream;
				control.BlReadCrcFlash(mStream, address, numBlocks);
				mStream.seekp(0, std::ios::end);
				size_t bytesRead = mStream.tellp();
				if(2 * numBlocks != bytesRead)
				{
					cout << "Read CRC failed" << endl;
					return;
				}
				PrintCrc(mStream, bytesRead, address);
			}
			catch(FatalError& e)
			{
				cout << "Read CRC failed because of " << e << endl;
			}
			
		};
};

class ControlCmdBlEraseFlash : public WiflyControlCmd
{
	public:
		ControlCmdBlEraseFlash(void) : WiflyControlCmd(
				string("erase_flash")) {};

		virtual void Run(WiflyControl& control) const
		{
			cout << "\nErasing flash... ";
			TRY_CATCH_COUT(control.BlEraseFlash());
		}	
};

class ControlCmdBlEraseEeprom : public WiflyControlCmd
{
	public:
		ControlCmdBlEraseEeprom(void) : WiflyControlCmd(
				string("erase_eeprom")) {};

		virtual void Run(WiflyControl& control) const
		{
			cout << "\nErasing eeprom... ";
			TRY_CATCH_COUT(control.BlEraseEeprom());
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
			cout << "Programming device flash... ";
			TRY_CATCH_COUT(control.BlProgramFlash(path));
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
		
		virtual void Read(WiflyControl& control, std::ostream& out, uint32_t address, const size_t numBytes) const = 0;

		virtual void Run(WiflyControl& control) const {
			uint32_t address;
			size_t numBytes;
			cin >> address;
			cin >> numBytes;
			try
			{
				std::stringstream mStream;

				Read(control, mStream, address, numBytes);
				mStream.seekp(0, std::ios::end);
				size_t bytesRead = mStream.tellp();
				if(bytesRead != numBytes)
				{
					cout << "Read " << m_Name << " failed" << endl;
				} 
				else 
				{
					Print(mStream, bytesRead, address);
				}

			}
			catch(FatalError& e)
			{
				cout << "Read " << m_Name << " failed, because of " << e << endl;
			}
	};
};

class ControlCmdBlReadEeprom : public ControlCmdBlRead
{
	public:
		ControlCmdBlReadEeprom(void) : ControlCmdBlRead("eeprom") {};
		void Read(WiflyControl& control, std::ostream& out, uint32_t address, const size_t numBytes) const {
			control.BlReadEeprom(out, address, numBytes);
		};
};

class ControlCmdBlReadFlash : public ControlCmdBlRead
{
	public:
		ControlCmdBlReadFlash(void) : ControlCmdBlRead("flash") {};
		void Read(WiflyControl& control, std::ostream& out, uint32_t address, const size_t numBytes) const {
			control.BlReadFlash(out, address, numBytes);
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
			TRY_CATCH_COUT(control.BlRunApp());
		};
};
			
class ControlCmdBlReadFwVersion : public WiflyControlCmd
{
	public:
		ControlCmdBlReadFwVersion(void) : WiflyControlCmd(
					string("bl_read_version"),
					string("' - prints the version of firmware in program memory"))
				{};
				
				virtual void Run(WiflyControl& control) const {
					cout << "Reading firmware version... ";
					try
					{
						cout << endl << "Version: " << control.BlReadFwVersion() << endl << endl << "done." << endl;
					}
					catch(FatalError& e)
					{
						cout << "failed, because of " << e << endl;
					}
				};
			};


class ControlCmdConfGetSsid : public WiflyControlCmd
{
	public:
		ControlCmdConfGetSsid(void) : WiflyControlCmd(
					string("conf_ssid"),
					string("' - get configured ssid from wlan module"))
		{};

		virtual void Run(WiflyControl& control) const {
			cout << "Reading wifly configuration ssid: ";
			cout << control.ConfGetSsid() << "\n";
		};
};

class ControlCmdConfSetDefaults : public WiflyControlCmd
{
	public:
		ControlCmdConfSetDefaults(void) : WiflyControlCmd(
					string("conf_defaults"),
					string("' - set connection parameters to default"))
		{};

		virtual void Run(WiflyControl& control) const {
			cout << "Setting wifly configuration to defaults... ";
			cout << (control.ConfSetDefaults() ? "done.\n" : "failed!\n");
		};
};

class ControlCmdConfSetWlan : public WiflyControlCmd
{
	public:
		ControlCmdConfSetWlan(void) : WiflyControlCmd(
				string("conf_wlan"),
				string(" <passphrase> <ssid>'\n")
			+ string("    <passphrase> wpa passphrase 1-63 characters\n")
			+ string("    <ssid> wlan ssid 1-32 characters")) {};

		virtual void Run(WiflyControl& control) const {
			string phrase, ssid;
			cin >> phrase;
			cin >> ssid;
			cout << "Setting passphrase '" << phrase << "' and ssid '" << ssid << "'... ";
			cout << (control.ConfSetWlan(phrase, ssid) ? "done.\n" : "failed!\n");
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
			TRY_CATCH_COUT(control.FwStartBl());
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
			try
			{
				cout << "done.\n" << control.FwGetCycletime() << '\n';
			}
			catch(FatalError& e)
			{
				cout << "failed, because of " << e << '\n';
			}
		};
};
			
class ControlCmdPrintFwVersion : public WiflyControlCmd
{
	public:
		ControlCmdPrintFwVersion(void) : WiflyControlCmd(
			string("print_fwversion"),
			string("' - displays current firmware version of pic")) {};
				
		virtual void Run(WiflyControl& control) const {
			cout << "Reading firmware version... ";
			try
			{
				cout << "done.\n\n" << control.FwGetVersion() << '\n';
			}
			catch(FatalError& e)
			{
				cout << "failed! because of: " << e << '\n';
			}
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
			try
			{
				cout << "done.\n\n" << control.FwGetTracebuffer() << '\n';
			}
			catch(FatalError& e)
			{
				cout << "failed! because of: " << e << '\n';
			}
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
			TRY_CATCH_COUT(control.FwClearScript());
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
			TRY_CATCH_COUT(control.FwLoopOn());
		};
};

class ControlCmdLoopOff : public WiflyControlCmd
{
	public:
		ControlCmdLoopOff(void) : WiflyControlCmd(
				string("loopoff"),
				string(" <numLoops>'\n")
			+ string("    <numLoops> [0..255] number of executions for the loop. Enter 0 for infinity loop."))
		{};

		virtual void Run(WiflyControl& control) const {
			int numLoops;
			do {
				cin >> numLoops;
			} while((numLoops < 0) || (numLoops > UINT8_MAX)); 
			cout << "Transmitting command loop off... ";
			TRY_CATCH_COUT(control.FwLoopOff((uint8_t)numLoops));
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
			TRY_CATCH_COUT(control.FwSetWait(waitTmms));
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
			uint16_t timevalue;
			cin >> addr;
			cin >> color;
			cin >> timevalue;
			cout << "Transmitting command set fade... ";
			TRY_CATCH_COUT(control.FwSetFade(color, timevalue, addr, false));
		};
};

class ControlCmdSetRtc : public WiflyControlCmd
{
	public:
		ControlCmdSetRtc(void) : WiflyControlCmd(
				string("setrtc"),
				string("' - set time of rtc in target to current systemtime")){};

		virtual void Run(WiflyControl& control) const {
			cout << "Transmitting current time... ";
			try
			{
				tm timeinfo;
				time_t rawtime;
				rawtime = time(NULL);
				localtime_r(&rawtime, &timeinfo);
				control.FwSetRtc(timeinfo);
				cout << "done.\n";
			}
			catch(FatalError& e)
			{
				cout << "failed! because of: " << e << '\n';
			}
		};
};

class ControlCmdGetRtc : public WiflyControlCmd
{
	public:
		ControlCmdGetRtc(void) : WiflyControlCmd(
				string("getrtc"),
				string("' - get time of rtc in target")){};

		virtual void Run(WiflyControl& control) const {
			cout << "Getting target time... ";
			try
			{
				tm timeinfo;
				control.FwGetRtc(timeinfo);
				cout << "done.\n\n" << asctime(&timeinfo) << '\n';
			}
			catch(FatalError& e)
			{
				cout << "failed! because of: " << e << '\n';
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
			cout << "Running fw test loop... ";
			TRY_CATCH_COUT(control.FwTest());
		};
};
			
class ControlCmdStressTest : public WiflyControlCmd
{
	public:
		ControlCmdStressTest(void) : WiflyControlCmd(
			   string("stresstest"),
			   string("' - run test loop! Caution: Test run in endless loop")) {};
				
		virtual void Run(WiflyControl& control) const {
			cout << "Running stresstest... ";
			TRY_CATCH_COUT(control.FwStressTest());
		};
};


static const WiflyControlCmd* s_Cmds[] = {
	new ControlCmdBlEnableAutostart(),
	new ControlCmdBlInfo(),
	new ControlCmdBlCrcFlash(),
	new ControlCmdBlEraseEeprom(),
	new ControlCmdBlEraseFlash(),
	new ControlCmdBlProgramFlash(),
	new ControlCmdBlReadEeprom(),
	new ControlCmdBlReadFlash(),
	new ControlCmdBlReadFwVersion(),
	new ControlCmdBlRunApp(),
	new ControlCmdConfGetSsid(),
	new ControlCmdConfSetDefaults(),
	new ControlCmdConfSetWlan(),
	new ControlCmdClearScript(),
	new ControlCmdSetFade(),
	new ControlCmdStartBl(),
	new ControlCmdTest(),
	new ControlCmdStressTest(),
	new ControlCmdPrintTracebuffer(),
	new ControlCmdPrintFwVersion(),
	new ControlCmdPrintCycletime(),
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
				Trace(ZONE_INFO, "%zu:%s:%s\n", i, name.c_str(), s_Cmds[i]->GetName().c_str());
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

