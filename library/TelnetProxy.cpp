/**
 Copyright (C) 2013 Nils Weiss, Patrick Bruenn.
 
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

#include "TelnetProxy.h"
#include "timeval.h"
#include "trace.h"

#include <algorithm>
#include <cctype>
#include <sstream>

static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

TelnetProxy::TelnetProxy(const TcpSocket& sock)
	: mSock(sock)
{
}

void TelnetProxy::ClearResponse(void) const
{
	timeval timeout{0, 1};
	uint8_t response[64];
	while(sizeof(response) <= mSock.Recv(response, sizeof(response), &timeout));
}

bool TelnetProxy::Close(bool doSave) const
{
	if(doSave && !Send("save\r\n", "\r\nStoring in config\r\n<2.45> "))
	{
		Trace(ZONE_ERROR, "saving changes failed\n");
		return false;
	}
	return Send("exit\r\n", "\r\nEXIT\r\n");
}

bool TelnetProxy::ExtractStringOfInterest(const std::string& buffer, const std::string& searchKey, std::string& result) const
{
	std::stringstream stream;
	stream << buffer;
	std::string line;
	getline(stream, line, '\r');
	while(!line.empty())
	{
		const size_t start = line.find(searchKey);
		if(start != std::string::npos)
		{
			result.assign(line.substr(start + searchKey.size()));
			TraceBuffer(ZONE_INFO, result.c_str(), result.size(), "%c", "%lu bytes found: ", result.size());
			return true;
		}
		getline(stream, line, '\r');
	}
	Trace(ZONE_ERROR, "Couldn't find '%s' in response\n", searchKey.c_str());
	return false;
}

void TelnetProxy::GetString(const std::string& getCmd, const std::string& searchKey, std::string& result) const
{
	result.clear();

	if(!SendAndWaitForEcho(getCmd))
	{
		Trace(ZONE_ERROR, ">>%s<< receive echo failed\n", getCmd.data());
		return;
	}
	RecvString(searchKey, result);
}

bool TelnetProxy::Open(void) const
{
	static const timespec _300_TMMS = {0, 300000000};
	static const uint8_t ENTER_CMD_MODE[] = {'$', '$', '$'}; 

	ClearResponse();
	if(sizeof(ENTER_CMD_MODE) != mSock.Send(ENTER_CMD_MODE, sizeof(ENTER_CMD_MODE)))
	{
		Trace(ZONE_ERROR, "send $$$ sequence failed\n");
		return false;
	}
	
	// after "$$$" we need to wait at least 250ms to enter command mode
	nanosleep(&_300_TMMS, NULL);

	if(!Recv("CMD\r\n"))
	{
		Trace(ZONE_ERROR, "start telnet console mode failed\n");
		return false;
	}
	
	// send carriage return to start telnet console mode
	return Send("\r\n", "\r\n<2.45> ");
}

bool TelnetProxy::Recv(const std::string& expectedResponse) const
{
	timeval timeout = {5, 0};
	uint8_t buffer[64];
	if(sizeof(buffer) < expectedResponse.size())
	{
		Trace(ZONE_ERROR, "expected response to long!\n");
		return false;
	}

	timeval endTime, now;
	gettimeofday(&endTime, NULL);
	timeval_add(&endTime, &timeout);
	size_t bytesRead = 0;
	do	
	{
		uint8_t* const pBufferPos = buffer + bytesRead;
		bytesRead += mSock.Recv(pBufferPos, expectedResponse.size() - bytesRead, &timeout);
		gettimeofday(&now, NULL);
	} while((bytesRead < expectedResponse.size()) && timeval_sub(&endTime, &now, &timeout));
	TraceBuffer(ZONE_INFO, buffer, bytesRead, "%02x ", "%zu bytes received: ", bytesRead);
	TraceBuffer(ZONE_INFO, buffer, bytesRead, "%c", "%zu bytes received: ", bytesRead);
	Trace(ZONE_INFO, "%zu:%lu\n", bytesRead, expectedResponse.size());
	return 0 == memcmp(expectedResponse.data(), buffer, expectedResponse.size());
}

bool TelnetProxy::RecvString(const std::string& searchKey, std::string& result) const
{
	timeval timeout = {5, 0};
	uint8_t buffer[128];
	uint8_t* pBufferPos = buffer;
	uint8_t* pAOKBegin = buffer;
	uint8_t* pAOKPos = pAOKBegin;

	timeval endTime, now;
	gettimeofday(&endTime, NULL);
	timeval_add(&endTime, &timeout);
	do	
	{
		pBufferPos += mSock.Recv(pBufferPos, sizeof(buffer) - 1 - (pBufferPos - buffer), &timeout);

		// check for AOK
		while(pAOKPos < pBufferPos)
		{
			if(*pAOKPos != AOK[pAOKPos-pAOKBegin])
			{
				// We are not in a AOK sequence -> move pointers ahead
				pAOKBegin = pAOKPos;
			}
			else
			{
				if(pAOKPos >= pAOKBegin + sizeof(AOK) - 2)
				{
					// We received a full AOK sequence -> extract response from our data
					buffer[sizeof(buffer) - 1] = 0;
					return ExtractStringOfInterest((const char*)buffer, searchKey, result);
				}
				else
				{
					Trace(ZONE_INFO, "Found next character of AOK, but end not reached yet\n");
				}
			}
			pAOKPos++;
		}

		gettimeofday(&now, NULL);
	} while((buffer + sizeof(buffer) > pBufferPos) && timeval_sub(&endTime, &now, &timeout));
	TraceBuffer(ZONE_ERROR, buffer, pBufferPos - buffer, "%c", "No end found in: ");
	return false;
}

bool TelnetProxy::Send(const std::string& telnetMessage, const std::string& expectedResponse) const
{
	if(!SendAndWaitForEcho(telnetMessage))
	{
		Trace(ZONE_ERROR, ">>%s<< receive echo failed\n", telnetMessage.data());
		return false;
	}
	
	if(!Recv(expectedResponse))
	{
		Trace(ZONE_ERROR, ">>%s<< receive acknowledgment failed\n", telnetMessage.data());
		return false;
	}
	return true;
}

bool TelnetProxy::SendAndWaitForEcho(const std::string& telnetMessage) const
{
	if(telnetMessage.size() != mSock.Send((uint8_t*)telnetMessage.data(), telnetMessage.size()))
	{
		Trace(ZONE_ERROR, "Send telnetMessage >>%s<< failed\n", telnetMessage.data());
		return false;
	}
	return Recv(telnetMessage);
}

bool TelnetProxy::SendString(const std::string& command, std::string value) const
{
	static const std::string REPLACE("\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x3A\x3B\x3C\x3D\x3E\x3F\x40\x5B\x5C\x5D\x5E\x5F\x60\x7B\x7C\x7D\x7E");

	// if value contains no spaces its simple
	if(std::string::npos == value.find_first_of(' ', 0))
	{
		return Send(value.insert(0, command).append("\r\n"));
	}

	// value contains spaces so we have to replace them with another character
	const size_t pos = REPLACE.find_first_not_of(value, 0);
	if(std::string::npos == pos)
	{
		Trace(ZONE_ERROR, "No replacement character available to replace spaces in string\n");
		return false;
	}

	const char replacement = REPLACE[pos];
	std::replace_if(value.begin(), value.end(), isblank, replacement);
	if(!SetReplaceChar(replacement))
	{
		Trace(ZONE_ERROR, "set replacement character failed\n");
		return false;
	}

	const bool valueWasSet = Send(value.insert(0, command).append("\r\n"));
	return SetReplaceChar() && valueWasSet;
}

bool TelnetProxy::SetReplaceChar(const char replace) const
{
	std::string replaceCmd("set opt replace " + std::string(1, replace) + "\r\n");
	return Send(replaceCmd);
}

