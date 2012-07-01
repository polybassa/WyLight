#ifndef _WIFLYCONTROLCLI_H_
#define _WIFLYCONTROLCLI_H_
#include <string>
#include "WiflyControl.h"

#define SET_COLOR_RED 128, 0, 0
#define SET_COLOR_GREEN 0, 128, 0
#define SET_COLOR_BLUE 0, 0, 128


class WiflyControlCli
{
	private:
		WiflyControl mControl;
		bool mRunning;

	public:
		WiflyControlCli(void);
		void run(void);	
};
#endif /* #ifndef _WIFLYCONTROLCLI_H_ */
