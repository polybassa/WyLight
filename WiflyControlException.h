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
#include <exception>
#include <string>
#include "wifly_cmd.h"
#include "BlRequest.h"


class WiflyControlException : public std::exception
{
public:
	WiflyControlException(const std::string errorString = "WiflyControlException")
	: std::exception(), m_ErrorString(errorString) {};
		
	const std::string& getErrorString(void) const { return m_ErrorString; };
	
private:
	const std::string m_ErrorString;
};

class WiflyControlBlNoResponseException: public WiflyControlException
{
public:
	WiflyControlBlNoResponseException(const struct BlRequest failedRequest, const std::string errorString)
	: WiflyControlException(errorString), m_FailedRequest(failedRequest) {};
	
	const struct BlRequest& getFailedRequest(void) const { return m_FailedRequest; };
	
private:
	struct BlRequest m_FailedRequest;
};

class WiflyControlFwNoResponseException : public WiflyControlException
{
public:
	WiflyControlFwNoResponseException(const struct cmd_frame* const failedFrame, const std::string errorString)
	: WiflyControlException(errorString) { memcpy(&m_FailedFrame, failedFrame, sizeof(struct cmd_frame)); }
	
	const struct cmd_frame getFailedFrame(void) const { return m_FailedFrame; }
	
private:
	struct cmd_frame m_FailedFrame;
};

class WiflyControlConnectionAbortException : public WiflyControlException
{
public:
	WiflyControlConnectionAbortException(const bool doReconnect = false, const std::string errorString = "")
	: WiflyControlException(errorString), reconnect(doReconnect) {};
	
	const bool doReconnect(void) const {return reconnect; };
	
private:
	const bool reconnect;
};

class WiflyControlScriptBufferFullException : public WiflyControlException
{
public:
	WiflyControlScriptBufferFullException(const struct cmd_frame* const failedCommand, const std::string errorString)
	: WiflyControlException(errorString) { memcpy(&m_FailedCommand, failedCommand, sizeof(struct cmd_frame)); }
	
	const struct cmd_frame getFailedCommand(void) const { return m_FailedCommand; }
	
private:
	struct cmd_frame m_FailedCommand;
};



#endif /* defined(____WiflyControlException__) */
