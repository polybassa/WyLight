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


#ifndef ____WiflyControlException__
#define ____WiflyControlException__

#include <stdio.h>
#include <string.h>
#include "wifly_cmd.h"

class WiflyControlException
{
public:
	WiflyControlException(const char* const errorString, int errorID = 0)
	: mID(errorID) { strncpy(mErrorString, errorString, sizeof(mErrorString)); }
		
	int getID() const { return mID; }
	const char* getErrorString() const { return mErrorString; }
	
private:
	int mID;
	char mErrorString[1024];
};

class WiflyControlBadResponseException : public WiflyControlException
{
public:
	WiflyControlBadResponseException(const struct cmd_frame* const failedFrame, const char* const errorString, int errorID )
	: WiflyControlException(errorString, errorID) { memcpy(&mFailedFrame, failedFrame, sizeof(struct cmd_frame)); }
	
	struct cmd_frame getFailedFrame() const { return mFailedFrame; }
	
private:
	struct cmd_frame mFailedFrame;
};

#endif /* defined(____WiflyControlException__) */
