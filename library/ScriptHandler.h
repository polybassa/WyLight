//
//  ScriptHandler.h
//  WiflyControlCli
//
//  Created by Nils Weiß on 23.04.13.
//  Copyright (c) 2013 Nils Weiß. All rights reserved.
//

#ifndef __WiflyControlCli__ScriptHandler__
#define __WiflyControlCli__ScriptHandler__

#include <iostream>
#include "FwCommand.h"
#include <list>

class Script {
	std::list<ILedCommand> mList;
	
	uint32_t totalDuration = 0;
	
public:
	Script(void) {};
	
	void addCommand(ILedCommand cmd) { mList.insert(mList.begin(), cmd); totalDuration += cmd.getTimeValue(); };
	
	
};

#endif /* defined(__WiflyControlCli__ScriptHandler__) */
