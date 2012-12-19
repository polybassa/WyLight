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
#include "BroadcastReceiver.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using std::cout;

WiflyControlCli::WiflyControlCli(const char* pAddr, short port, bool useTcp)
: mControl(pAddr, port, useTcp), mRunning(true)
{
}

void WiflyControlCli::Run(void)
{
	const WiflyControlCmd* pCmd;
	ShowHelp();
	string nextCmd;
	while(mRunning)
	{
		cout << "WiflyControlCli: ";
		cin >> nextCmd;

		if("exit" == nextCmd)
		{
			return;
		}
		else if("?" == nextCmd)
		{
			ShowHelp();
		}
		else
		{
			pCmd = WiflyControlCmdBuilder::GetCmd(nextCmd);
			if(NULL != pCmd) {
				pCmd->Run(mControl);
			}
		}
	}
}

void WiflyControlCli::ShowHelp(void) const
{
	cout << "Command reference:" << endl;
	cout << "'?' - this help" << endl;
	cout << "'exit' - terminate cli" << endl;

	size_t i = 0;
	const WiflyControlCmd* pCmd = WiflyControlCmdBuilder::GetCmd(i++);
	while(pCmd != NULL) {
		cout << *pCmd << endl;
		pCmd = WiflyControlCmdBuilder::GetCmd(i++);
	}
	return;
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

void Print(boost::asio::ip::udp::endpoint& s)
{
	static size_t i = 0;
	cout << i << ':' << s << '\n';
}

int main(int argc, const char* argv[])
{
	short port = 2000;
#if 1
	const char* pAddr = "127.0.0.1";
	bool useTcp = false;
#else
	const char* pAddr = "192.168.0.14";
	bool useTcp = true;
#endif

	BroadcastReceiver receiver(55555);
	cout << "please wait, scanning for remotes...";
	for(size_t timeout = 9; timeout > 0; timeout--) {
		(cout << timeout << ' ').flush();
		sleep(1);
	}
	cout << '\n';
	receiver.Stop();
	std::vector<boost::asio::ip::udp::endpoint> v;
	receiver.GetIpTable(v);
	cout << "Select remote:\n";
	for_each(v.begin(), v.end(), Print);

	size_t selection;
	std::cin >> selection;

	if(selection < v.size()) {
		cout << "You selected number: " << selection << '\n';
	} else {
		cout << selection << " is an invalid value\n";
	}
	return 0;
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
