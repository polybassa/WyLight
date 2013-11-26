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
#include "FwCommand.h"
#include "trace.h"
#include "StartupManager.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <stdint.h>
#include <memory>

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
	} \
	catch(std::exception& e) { \
		cout << "failed! because of: " << e.what() << '\n'; \
	}

void TrySend(WyLight::Control& ctrl, WyLight::FwCommand&& cmd)
{
	try {
		ctrl << std::move(cmd);
		cout << "done.\n";
	} catch(std::exception& e) {
		cout << "failed! because of: " << e.what() << '\n';
	}
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
	virtual void Run(WyLight::Control& control) const = 0;
protected:
	const string m_Name;
	const string m_Description;
	void Print(std::istream& in, const size_t size, const uint32_t address) const {
		for(size_t i = 0; i < size; i++) {
			char asciiString[16];
			if(0 == (i % 16)) {
				string asciiOutput(asciiString);
				for(char& c : asciiOutput) { if(c < 0x20) c = ' '; }
				i == 0 ? cout << "" : cout << "   :   ";
				cout << asciiOutput;
				cout << endl << "0x" << setw(4) << setfill('0') << hex << int(address + i) << ": ";
			}
			uint8_t b;
			in >> b;
			asciiString[i % 16] = b;
			cout << setw(2) << setfill('0') << hex << int(b) << ' ';
		}
		cout << endl;
	}
	void PrintCrc(std::istream& in, const size_t size, const uint32_t address) const {
		for(size_t i = 0; i < size; i++) {
			if(0 == (i % 2)) {
				cout << endl
				     << "0x" << setw(4) << setfill('0') << hex
				     << int(address + i * FLASH_ERASE_BLOCKSIZE)
				     << " - "
				     << "0x" << setw(4) << setfill('0') << hex
				     << int(address + ((i + 2) * FLASH_ERASE_BLOCKSIZE) - 1)
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

	virtual void Run(WyLight::Control& control) const
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

	virtual void Run(WyLight::Control& control) const {
		WyLight::BlInfo info;
		try {
			control.BlReadInfo(info);
			info.Print();
		} catch(WyLight::FatalError& e)   {
			std::cout << "Read bootloader info failed" << endl;
		}
	};
};

class ControlCmdBlCrcFlash : public WiflyControlCmd
{
public:
	ControlCmdBlCrcFlash(void) : WiflyControlCmd("crc_flash") {};
	virtual void Run(WyLight::Control& control) const {
		uint32_t address;
		size_t numBlocks;
		cin >> address;
		cin >> numBlocks;
		try {
			std::stringstream mStream;
			control.BlReadCrcFlash(mStream, address, numBlocks);
			mStream.seekp(0, std::ios::end);
			size_t bytesRead = mStream.tellp();
			if(2 * numBlocks != bytesRead) {
				cout << "Read CRC failed" << endl;
				return;
			}
			PrintCrc(mStream, bytesRead, address);
		} catch(WyLight::FatalError& e)   {
			cout << "Read CRC failed because of " << e << endl;
		}

	};
};

class ControlCmdBlEraseFlash : public WiflyControlCmd
{
public:
	ControlCmdBlEraseFlash(void) : WiflyControlCmd(
			string("erase_flash")) {};

	virtual void Run(WyLight::Control& control) const
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

	virtual void Run(WyLight::Control& control) const
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

	virtual void Run(WyLight::Control& control) const
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

	virtual void Read(WyLight::Control& control, std::ostream& out, uint32_t address, const size_t numBytes) const = 0;

	virtual void Run(WyLight::Control& control) const {
		uint32_t address;
		size_t numBytes;
		cin >> address;
		cin >> numBytes;
		try {
			std::stringstream mStream;

			Read(control, mStream, address, numBytes);
			mStream.seekp(0, std::ios::end);
			size_t bytesRead = mStream.tellp();
			if(bytesRead != numBytes) {
				cout << "Read " << m_Name << " failed" << endl;
			} else {
				Print(mStream, bytesRead, address);
			}

		} catch(WyLight::FatalError& e)   {
			cout << "Read " << m_Name << " failed, because of " << e << endl;
		}
	};
};

class ControlCmdBlReadEeprom : public ControlCmdBlRead
{
public:
	ControlCmdBlReadEeprom(void) : ControlCmdBlRead("eeprom") {};
	void Read(WyLight::Control& control, std::ostream& out, uint32_t address, const size_t numBytes) const {
		control.BlReadEeprom(out, address, numBytes);
	};
};

class ControlCmdBlReadFlash : public ControlCmdBlRead
{
public:
	ControlCmdBlReadFlash(void) : ControlCmdBlRead("flash") {};
	void Read(WyLight::Control& control, std::ostream& out, uint32_t address, const size_t numBytes) const {
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

	virtual void Run(WyLight::Control& control) const {
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Reading firmware version... ";
		try {
			cout << endl << "Version: " << control.BlReadFwVersion() << endl << endl << "done." << endl;
		} catch(WyLight::FatalError& e)   {
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Reading wifly configuration ssid: ";
		cout << control.ConfGetSsid() << "\n";
	};
};

class ControlCmdConfRebootWlanModule : public WiflyControlCmd
{
public:
	ControlCmdConfRebootWlanModule(void) : WiflyControlCmd(
			string("conf_rebootWlanModule"),
			string("' - reboots the wlan modul. Cli terminates after this command!"))
	{};

	virtual void Run(WyLight::Control& control) const {
		cout << "Rebooting wlan module... ";
		if(control.ConfRebootWlanModule()) {
			cout << "done.\n";
			cout << "Terminating WyLight::Control commandline interface now!!!! Please restart." << endl;
			std::exit(EXIT_SUCCESS);
		} else {
			cout << "failed!\n";
		}
	};
};

class ControlCmdConfWlanAsClient : public WiflyControlCmd
{
public:
	ControlCmdConfWlanAsClient(void) : WiflyControlCmd(
			string("conf_wlanAsClient"),
			string(" <passphrase> <ssid> <name>'\n")
			+ string("    <passphrase> wpa passphrase 1-63 characters\n")
			+ string("    <ssid> wlan ssid 1-32 characters\n")
			+ string("    <name> device name for broadcasts 1-32 characters")) {};

	virtual void Run(WyLight::Control& control) const {
		string phrase, ssid, name;
		cin >> phrase;
		cin >> ssid;
		cin >> name;
		cout << "Setting passphrase '" << phrase << "' and ssid '" << ssid << "' and name '" << name << "' ... ";

		if(control.ConfModuleForWlan(phrase, ssid, name)) {
			cout << "done.\n";
			cout << "Terminating WyLight::Control commandline interface now!!!! Please restart." << endl;
			std::exit(EXIT_SUCCESS);
		} else {
			cout << "failed!\n";
		}
	};
};

class ControlCmdConfWlanAsSoftAP : public WiflyControlCmd
{
public:
	ControlCmdConfWlanAsSoftAP(void) : WiflyControlCmd(
			string("conf_wlanAsSoftAP"),
			string(" <ssid>'\n")
			+ string("    <ssid> wlan ssid 1-32 characters")) {};

	virtual void Run(WyLight::Control& control) const {
		string ssid;
		cin >> ssid;
		cout << "Setting as soft-AP with ssid '" << ssid << "'... ";
		if(control.ConfModuleAsSoftAP(ssid)) {
			cout << "done.\n";
			cout << "Terminating WyLight::Control commandline interface now!!!! Please restart." << endl;
			std::exit(EXIT_SUCCESS);
		} else {
			cout << "failed!\n";
		}
	};
};


class ControlCmdConfSetDeviceId : public WiflyControlCmd
{
public:
	ControlCmdConfSetDeviceId(void) : WiflyControlCmd(
			string("conf_deviceId"),
			string(" <name>'\n")
			+ string("    <name> device name 1-32 characters")) {};

	virtual void Run(WyLight::Control& control) const {
		string name;
		cin >> name;
		cout << "Setting device name '" << name << "'... ";
		cout << (control.ConfSetDeviceId(name) ? "done.\n" : "failed!\n");
	};
};


class ControlCmdStartBl : public WiflyControlCmd
{
public:
	ControlCmdStartBl(void) : WiflyControlCmd(
			string("start_bl"),
			string("' - start bootloader and terminate application")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Starting bootloader... ";
		TrySend(control, WyLight::FwCmdStartBl {}
			);
	};

};

class ControlCmdPrintCycletime : public WiflyControlCmd
{
public:
	ControlCmdPrintCycletime(void) : WiflyControlCmd(
			string("print_cycletime"),
			string("' - prints all timevalues of internal methode execution times")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Transmitting command print cycletime... ";
		try {
			cout << "done.\n" << control.FwGetCycletime() << '\n';
		} catch(WyLight::FatalError& e)   {
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Reading firmware version... ";
		try {
			cout << "done.\n\n" << control.FwGetVersion() << '\n';
		} catch(WyLight::FatalError& e)   {
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Reading tracebuffer... ";
		try {
			cout << "done.\n\n" << control.FwGetTracebuffer() << '\n';
		} catch(WyLight::FatalError& e)   {
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Clearing script buffer... ";
		TrySend(control, WyLight::FwCmdClearScript {}
			);
	};
};

class ControlCmdLoopOn : public WiflyControlCmd
{
public:
	ControlCmdLoopOn(void) : WiflyControlCmd(
			string("loopon"),
			string("' - indicates the start of a loop")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Transmitting command loop on... ";
		TrySend(control, WyLight::FwCmdLoopOn {}
			);
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Transmitting command loop off... ";
		TrySend(control, WyLight::FwCmdLoopOff {cin}
			);
	};
};

class ControlCmdWait : public WiflyControlCmd
{
public:
	ControlCmdWait(void) : WiflyControlCmd(
			string("wait"),
			string(" <time>'\n")
			+ string("    <time> the number of milliseconds the wait should take")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Transmitting command wait... ";
		TrySend(control, WyLight::FwCmdWait {cin}
			);
	};
};

class ControlCmdSetFade : public WiflyControlCmd
{
public:
	ControlCmdSetFade(void) : WiflyControlCmd(
			string("setfade"),
			string(" <addr> <rgb> <time> <parallel>'\n")
			+ string("    <addr> hex bitmask, which leds should be set to the new color\n")
			+ string("    <rgb> hex rgb value of the new color f.e. red: ff0000\n")
			+ string("    <time> the number of ten milliseconds the fade should take")
			+ string("    <parallel> 1 to allow parallel fades, 0 to disable")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Parsing and transmitting command set fade... ";
		TrySend(control, WyLight::FwCmdSetFade {cin}
			);
	};
};

class ControlCmdSetGradient : public WiflyControlCmd
{
public:
	ControlCmdSetGradient(void) : WiflyControlCmd(
			string("setgradient"),
			string(" <rgb_1> <rgb_2> <time> <offset> <num_leds> <parallel>'\n")
			+ string("    <rgb_1> hex rgb value of the start color f.e. green: 00ff00\n")
			+ string("    <rgb_2> hex rgb value of the end color f.e. red: ff0000\n")
			+ string("    <time> the number of ten milliseconds the fade should take")
			+ string("    <offset> position of the first led")
			+ string("    <num_leds> position of the last led - <offset>")
			+ string("    <parallel> 1 to allow parallel fades, 0 to disable")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Parsing and transmitting command set gradient... ";
		TrySend(control, WyLight::FwCmdSetGradient {cin}
			);
	};
};

class ControlCmdSetRtc : public WiflyControlCmd
{
public:
	ControlCmdSetRtc(void) : WiflyControlCmd(
			string("setrtc"),
			string("' - set time of rtc in target to current systemtime")){};

	virtual void Run(WyLight::Control& control) const {
		cout << "Parsing and transmitting current time... ";
		TrySend(control, WyLight::FwCmdSetRtc {}
			);
	};
};

class ControlCmdGetRtc : public WiflyControlCmd
{
public:
	ControlCmdGetRtc(void) : WiflyControlCmd(
			string("getrtc"),
			string("' - get time of rtc in target")){};

	virtual void Run(WyLight::Control& control) const {
		cout << "Getting target time... ";
		try {
			tm timeinfo;
			control.FwGetRtc(timeinfo);
			cout << "done.\n\n" << asctime(&timeinfo) << '\n';
		} catch(WyLight::FatalError& e)   {
			cout << "failed! because of: " << e << '\n';
		}
	};

};

class ControlCmdGetTargetMode : public WiflyControlCmd
{
public:
	ControlCmdGetTargetMode(void) : WiflyControlCmd(
			string("get_mode"),
			string("' - displays current operation mode of pic")) {};

	virtual void Run(WyLight::Control& control) const {
		cout << "Reading target mode... ";
		try {
			size_t result = control.GetTargetMode();
			cout << "done.\n\n";
			if(result) {
				result == FW_IDENT ? cout << "Target in FW-Mode \n" : cout << "Target in BL-Mode \n";
			} else {
				cout << "No operation mode detected!\n";
			}
		} catch(WyLight::FatalError& e)   {
			cout << "failed! because of: " << e << '\n';
		}
	};
};

class ControlCmdDoStartup : public WiflyControlCmd
{
public:
	ControlCmdDoStartup(void) : WiflyControlCmd(
			string("startup"),
			string(" <hexFile>' \n ") + string("    <hexFile> path of hexfile to write")) {};

	virtual void Run(WyLight::Control& control) const
	{
		string path;
		cin >> path;
		cout << "Startup in progress... ";

		WyLight::StartupManager manager;
		TRY_CATCH_COUT(manager.startup(control, path));
		if(manager.getCurrentState() == WyLight::StartupManager::STARTUP_FAILURE) {
			cout << endl << "Startup failure occured" << endl;
		}
	}
};


class ControlCmdTest : public WiflyControlCmd
{
public:
	ControlCmdTest(void) : WiflyControlCmd(
			string("test"),
			string("' - run test loop")) {};

	virtual void Run(WyLight::Control& control) const {
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

	virtual void Run(WyLight::Control& control) const {
		cout << "Running stresstest... ";
		TRY_CATCH_COUT(control.FwStressTest());
	};
};

static const std::shared_ptr<const WiflyControlCmd> s_Cmds[] = {
//static const WiflyControlCmd* s_Cmds[] = {
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlEnableAutostart()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlInfo()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlCrcFlash()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlEraseEeprom()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlEraseFlash()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlProgramFlash()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlReadEeprom()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlReadFlash()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlReadFwVersion()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdBlRunApp()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdConfGetSsid()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdConfWlanAsClient()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdConfWlanAsSoftAP()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdConfSetDeviceId()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdConfRebootWlanModule()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdClearScript()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdSetFade()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdSetGradient()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdStartBl()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdTest()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdStressTest()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdPrintTracebuffer()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdPrintFwVersion()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdPrintCycletime()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdSetRtc()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdGetRtc()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdLoopOn()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdLoopOff()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdWait()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdGetTargetMode()),
	std::shared_ptr<const WiflyControlCmd>(new ControlCmdDoStartup()),
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
	static const std::shared_ptr<const WiflyControlCmd> GetCmd(const string name) {
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
	static const std::shared_ptr<const WiflyControlCmd> GetCmd(size_t index) {
		if(index < s_NumCmds) {
			return s_Cmds[index];
		}
		return NULL;
	}
};
#endif /* #ifndef _WIFLYCONTROLCMD_H_ */

