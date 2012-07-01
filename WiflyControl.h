#ifndef _WIFLYCONTROL_H_
#define _WIFLYCONTROL_H_

#include <string>
#include "ClientSocket.h"
#include "wifly_cmd.h"

class WiflyControl
{
	private:
		struct cmd_frame mCmdFrame;
		const ClientSocket mSock;
		unsigned long ToRGBA(std::string& s) const;
		
	public:
		WiflyControl();
		void SetColor(char r, char g, char b);
		void SetColor(unsigned long rgba);
		void SetColor(std::string& s);
};
#endif /* #ifndef _WIFLYCONTROL_H_ */
