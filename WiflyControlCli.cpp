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

#include "WiflyControlCli.h"
#include "WiflyControlCmd.h"
#include <iostream>
#include <cstdlib>

using namespace std;

WiflyControlCli::WiflyControlCli(const char* pAddr, short port, bool useTcp)
: mControl(pAddr, port, useTcp), mRunning(true)
{
}

void WiflyControlCli::Run(void)
{
	ControlCmdReadEeprom readEeprom;
	ControlCmdReadEeprom readFlash;
	string nextCmd;
	cout << "Command reference:" << endl;
	cout << "'exit' - terminate cli" << endl;
	cout << "'bl_info' - read bootloader information" << endl;
	cout << "'addcolor <addr> <rgb> <hour> <minute> <second>'" << endl;
	cout << "    <addr> hex bitmask, which leds should be set to the new color" << endl;
	cout << "    <rgb> hex rgb value of the new color f.e. red: ff0000" << endl;
	cout << "    <hour> hour of date event" << endl;
	cout << "    <minute> minute of date event" << endl;
	cout << "    <second> second of date event" << endl;
	cout << readEeprom << endl;
	cout << readFlash << endl;
	cout << "    <addr> hex bitmask, which leds should be set to the new color" << endl;
	cout << "    <rgb> hex rgb value of the new color f.e. red: ff0000" << endl;
	cout << "'setfade <addr> <rgb> <time>'" << endl;
	cout << "    <addr> hex bitmask, which leds should be set to the new color" << endl;
	cout << "    <rgb> hex rgb value of the new color f.e. red: ff0000" << endl;
	cout << "    <time> the number of milliseconds the fade should take" << endl;
	while(mRunning)
	{
		cout << "WiflyControlCli: ";
		cin >> nextCmd;

		if("exit" == nextCmd) {
			return;
		}
		if ("addcolor" == nextCmd) {
			string addr, color;
			unsigned long hour, minute, second;
			cin >> addr;
			cin >> color;
			cin >> hour;
			cin >> minute;
			cin >> second;
			cout << addr << " " << color << " " << hour << " " << minute << " " << second << endl; 
			mControl.AddColor(addr, color, hour, minute, second);
		} else if ("bl_info" == nextCmd) {
			BlInfo info;
			if(sizeof(info) == mControl.BlReadInfo(info))
			{
				PrintBlInfo(info);
			}
			else
			{
				std::cout << "Read bootloader info failed" << endl;
			}
		} else if ("setcolor" == nextCmd) {
			string addr, color;
			cin >> addr;
			cin >> color;
			mControl.SetColor(addr, color);
		}	else if ("setfade" == nextCmd)bl {
			string addr, color;
			unsigned long timevalue;
			cin >> addr;
			cin >> color;
			cin >> timevalue;
			mControl.SetFade(addr, color, (unsigned short)timevalue * 1024);
		} else
		{
			const WiflyControlCmd* pCmd = WiflyControlCmdBuilder::GetCmd(nextCmd);
			if(NULL != pCmd) {
				pCmd->Run(mControl);
			}
		}
	}
}

void WiflyControlCli::PrintBlInfo(const BlInfo& info) const
{
	switch(info.familyId)
	{
		case 0x02:
			printf("PIC16");
#ifdef PIC16
			printf("F%d", WORD(info.deviceIdHigh, info.deviceIdLow));
#endif
			break;
		case 0x04:
			printf("PIC18");
			break;
		default:
			printf("unknown(0x%1x)", info.familyId);
			break;
	}
	printf(" bootloader V%d.%d\n", info.versionMajor, info.versionMinor);
	printf("Size: %d\n", WORD(info.sizeHigh, info.sizeLow));
	printf("Startaddress: 0x%x\n", DWORD(WORD(info.zero, info.startU), WORD(info.startHigh, info.startLow)));
	printf("erase flash command %ssupported\n", ((0x02 == info.familyId) && (0x01 != info.cmdmaskHigh)) ? "not " : "");
}

#ifdef ANDROID
#include <jni.h>
extern "C" {
void Java_biz_bruenn_WiflyLight_WiflyLightActivity_runClient(JNIEnv* env, jobject ref)
{
	WiflyControl control;
	colorLoop(control);
}
}
#endif

int main(int argc, const char* argv[])
{
	cout << "Usage:   client.bin <ip> <port> [tcp]" << endl;
	cout << "Default: client.bin 127.0.0.1 2000 --> udp connection to localhost" << endl;
	const char* pAddr = "127.0.0.1";
	short port = 2000;
	bool useTcp = false;
	if(argc > 1)
	{
		pAddr = argv[1];
		if(argc > 2)
		{
			port = (short)atoi(argv[2]);
			if(argc > 3)
			{
				useTcp = (0 == strncmp(argv[3], "tcp", 3));
			}
		}
	}
	WiflyControlCli cli(pAddr, port, useTcp);
	cli.Run();
	return 0;
}
