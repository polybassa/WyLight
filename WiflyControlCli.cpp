/**
		Copyright (C) 2012 Nils Weiss, Patrick Brünn.

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
#include <iostream>

using namespace std;

WiflyControlCli::WiflyControlCli(void)
: mRunning(true)
{
}

void WiflyControlCli::run(void)
{
	string nextCmd;
	string nextOption;
	cout << "use 'exit' to terminate this cli" << endl;
	while(mRunning)
	{
		cout << "WiflyControlCli: ";
		cin >> nextCmd;

		if("exit" == nextCmd) {
			return;
		}
		else if ("setcolor" == nextCmd) {
			cin >> nextOption;
			if("red" == nextOption) {
				mControl.SetColor(0xFF000000);
			}
			else if ("green" == nextOption) {
				mControl.SetColor(0x00FF0000);
			}
			else if ("blue" == nextOption) {
				mControl.SetColor(0x0000FF00);
			}
			else {
				mControl.SetColor(nextOption);
			}
		}
	}
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
	WiflyControlCli cli;
	cli.run();
	return 0;
}
