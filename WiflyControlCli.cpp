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
