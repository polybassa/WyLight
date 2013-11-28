/*
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
#include <thread>
#include <chrono>
#include <regex>
#include <vector>
#include <list>
#include <iostream>
#include "__stl_patches.h"

namespace WyLight {

	static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

	TelnetProxy::TelnetProxy(const TcpSocket& sock)
		: mSock(sock)
	{}

	void TelnetProxy::ClearResponse(void) const
	{
		timeval timeout {0, 1};
		uint8_t response[64];
		while(sizeof(response) <= mSock.Recv(response, sizeof(response), &timeout)) ;
	}

	bool TelnetProxy::Close(bool doSave) const
	{
		if(doSave && !Send("save\r\n", "Storing in config" PROMPT)) {
			Trace(ZONE_ERROR, "saving changes failed\n");
			return false;
		}
		return Send("exit\r\n", "EXIT\r\n");
	}

	bool TelnetProxy::ExtractStringOfInterest(const std::string& buffer, const std::string& searchKey, std::string& result) const
	{
			Trace(ZONE_VERBOSE, buffer.c_str());
		std::stringstream stream;
		stream << buffer;
		std::string line;
		getline(stream, line, '\r');
		Trace(ZONE_VERBOSE, "Line: %s\n", line.c_str());
		while(!line.empty())
		{
			const size_t start = line.find(searchKey);
			if(start != std::string::npos) {
				result.assign(line.substr(start + searchKey.size()));
				TraceBuffer(ZONE_INFO, result.c_str(), result.size(), "%c", "%lu bytes found: ", result.size());
				return true;
			}
			getline(stream, line, '\r');
		}
		Trace(ZONE_ERROR, "Couldn't find '%s' in response\n", searchKey.c_str());
		return false;
	}

	bool TelnetProxy::Open(void) const
	{
		static const uint8_t ENTER_CMD_MODE[] = {'$', '$', '$'};

		ClearResponse();
		if(sizeof(ENTER_CMD_MODE) != mSock.Send(ENTER_CMD_MODE, sizeof(ENTER_CMD_MODE))) {
			Trace(ZONE_ERROR, "send $$$ sequence failed\n");
			return false;
		}

		// after "$$$" we need to wait at least 250ms to enter command mode
		std::this_thread::sleep_for(std::chrono::nanoseconds(300000000));

		if(!Recv("CMD\r\n")) {
			Trace(ZONE_ERROR, "start telnet console mode failed\n");
			return false;
		}

		// send carriage return to start telnet console mode
		return true;
	}

	bool TelnetProxy::Recv(const std::string& expectedResponse) const
	{
		timeval timeout = {5, 0};
		uint8_t buffer[128];
		if(sizeof(buffer) < expectedResponse.size()) {
			Trace(ZONE_ERROR, "expected response to long!\n");
			return false;
		}

		timeval endTime, now;
		gettimeofday(&endTime, NULL);
		timeval_add(&endTime, &timeout);
		size_t bytesRead = 0;
		do
		{
			uint8_t *const pBufferPos = buffer + bytesRead;
			bytesRead += mSock.Recv(pBufferPos, expectedResponse.size() - bytesRead, &timeout);

			gettimeofday(&now, NULL);
		}
		while((bytesRead < expectedResponse.size()) && timeval_sub(&endTime, &now, &timeout));

		TraceBuffer(ZONE_INFO, expectedResponse.data(), expectedResponse.size(), "%02x ", "%zu bytes expected: ", expectedResponse.size());
		TraceBuffer(ZONE_INFO, buffer,                  bytesRead,               "%02x ", "%zu bytes received: ", bytesRead);
		TraceBuffer(ZONE_INFO, buffer,                  bytesRead,               "%c",    "%zu bytes received: ", bytesRead);
		return 0 == memcmp(expectedResponse.data(), buffer, expectedResponse.size());
	}

	bool TelnetProxy::RecvString(const std::string& searchKey, std::string& result) const
	{
		timeval timeout = {5, 0};
		uint8_t buffer[256];
		uint8_t *pBufferPos = buffer;
		uint8_t *pPromptBegin = buffer;
		uint8_t *pPromptPos = pPromptBegin;

		timeval endTime, now;
		gettimeofday(&endTime, NULL);
		timeval_add(&endTime, &timeout);
		do
		{
			pBufferPos += mSock.Recv(pBufferPos, sizeof(buffer) - 1 - (pBufferPos - buffer), &timeout);
			TraceBuffer(ZONE_VERBOSE, pPromptPos, pBufferPos - pPromptPos, "%c", "telnet response:\n");

			// check for AOK
			while(pPromptPos < pBufferPos)
			{
				if(*pPromptPos != PROMPT[pPromptPos - pPromptBegin]) {
					// We are not in a AOK sequence -> move pointers ahead
					pPromptBegin = pPromptPos;
				} else {
					if(pPromptPos >= pPromptBegin + sizeof(PROMPT) - 2) {
						// We received a full PROMPT sequence -> extract response from our data
						buffer[sizeof(buffer) - 1] = 0;
						return ExtractStringOfInterest((const char *)buffer, searchKey, result);
					} else {
						Trace(ZONE_INFO, "Found next character of PROMPT, but end not reached yet\n");
					}
				}
				pPromptPos++;
			}

			gettimeofday(&now, NULL);
		}
		while((buffer + sizeof(buffer) > pBufferPos) && timeval_sub(&endTime, &now, &timeout));
		TraceBuffer(ZONE_ERROR, buffer, pBufferPos - buffer, "%c", "No end found in: ");
		return false;
	}

	void TelnetProxy::RecvString(const std::string& getCmd, const std::string& searchKey, std::string& result) const
	{
		result.clear();

		if(!SendAndWaitForEcho(getCmd)) {
			Trace(ZONE_ERROR, ">>%s<< receive echo failed\n", getCmd.data());
			return;
		}
		RecvString(searchKey, result);
	}

	bool TelnetProxy::Send(const std::string& telnetMessage, const std::string& expectedResponse) const
	{
		if(!SendAndWaitForEcho(telnetMessage)) {
			Trace(ZONE_ERROR, ">>%s<< receive echo failed\n", telnetMessage.data());
			return false;
		}

		if(!Recv(expectedResponse)) {
			Trace(ZONE_ERROR, ">>%s<< receive acknowledgment failed\n", telnetMessage.data());
			return false;
		}
		return true;
	}

	bool TelnetProxy::SendAndWaitForEcho(const std::string& telnetMessage) const
	{
		if(telnetMessage.size() != mSock.Send((uint8_t *)telnetMessage.data(), telnetMessage.size())) {
			Trace(ZONE_ERROR, "Send telnetMessage >>%s<< failed\n", telnetMessage.data());
			return false;
		}
		return Recv(telnetMessage + "\r\n");
	}

	bool TelnetProxy::SendRebootCommand(void) const
	{
		const std::string telnetMessage = "reboot\r\n";
		if(telnetMessage.size() != mSock.Send((uint8_t *)telnetMessage.data(), telnetMessage.size())) {
			Trace(ZONE_ERROR, "Send telnetMessage >>%s<< failed\n", telnetMessage.data());
			return false;
		}
		return Recv(telnetMessage);
	}

	bool TelnetProxy::SendString(const std::string& command, std::string value) const
	{
		static const std::string REPLACE("\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x3A\x3B\x3C\x3D\x3E\x3F\x40\x5B\x5C\x5D\x5E\x5F\x60\x7B\x7C\x7D\x7E");

		// find a replacement character
		const size_t pos = REPLACE.find_first_not_of(value, 0);
		if(std::string::npos == pos) {
			Trace(ZONE_ERROR, "No replacement character available to replace spaces in string\n");
			return false;
		}

		const char replacement = REPLACE[pos];
		if(!SetReplaceChar(replacement)) {
			Trace(ZONE_ERROR, "set replacement character failed\n");
			return false;
		}

		std::replace_if(value.begin(), value.end(), isblank, replacement);
		const bool valueWasSet = Send(value.insert(0, command).append("\r\n"));
		return SetReplaceChar() && valueWasSet;
	}

	bool TelnetProxy::SetReplaceChar(const char replace) const
	{
		std::stringstream replaceCmd;
		replaceCmd << "set opt replace 0x" << std::hex << static_cast<int>(replace) << "\r\n";
		return Send(replaceCmd.str());
	}

	bool TelnetProxy::PerformWifiScan(std::string &result) const
	{
		static const std::string scanCommand = "scan\r\n";

		if(!SendAndWaitForEcho(scanCommand)) {
			Trace(ZONE_ERROR, ">>%s<< receive echo failed\n", scanCommand.data());
			return false;
		}

		timeval timeout = {25, 0};
		uint8_t buffer[2048];
		uint16_t bytesRead = 0;

		timeval endTime, now;
		gettimeofday(&endTime, NULL);
		timeval_add(&endTime, &timeout);
		std::string answer;
		do
		{
			bytesRead += mSock.Recv(buffer, sizeof(buffer) - bytesRead, &timeout);
			TraceBuffer(ZONE_VERBOSE, buffer, bytesRead, "%c", "telnet response:\n");

			const std::string startRegEx = "\r\nSCAN:Found [0-9]+\r\n";
			const std::string endRegEx = "\r\nEND:\r\n";
			std::cmatch startMatch, endMatch;

			answer = std::string((char *)buffer, bytesRead);

			if(std::regex_search(answer.c_str(), startMatch, std::regex(startRegEx)) && std::regex_search(answer.c_str(), endMatch, std::regex(endRegEx))) {
				result = std::string(answer, startMatch.position() + startMatch.length(), endMatch.position());
				return true;
			}
			gettimeofday(&now, NULL);
		}
		while((sizeof(buffer) > bytesRead) && timeval_sub(&endTime, &now, &timeout));

		return false;
	}

	unsigned int TelnetProxy::ComputeFreeChannel(const std::string& scanResults) const
	{
		//split strings at CRLF
		std::stringstream mStream(scanResults);
		std::vector<std::string> tempLineVec;
		std::list<unsigned int> engagedChannelList;

		while(!mStream.eof()) {
			std::string temp;
			std::getline(mStream, temp);
			tempLineVec.push_back(temp);
		}

		for(auto str : tempLineVec) {
			size_t startdelimiter = str.find_first_of(',');
			if(startdelimiter > 10) {
				continue;
			}
			std::string channelString = std::string(str, startdelimiter + 1, 2);
			unsigned int tempChannel = std::stoi(channelString);
			if(tempChannel > 0 && tempChannel < 14 && str[startdelimiter + 3] == ',') {
				engagedChannelList.push_back(tempChannel);
			}
		}
		engagedChannelList.sort();

		unsigned int maxDistanceAt = 1;
		unsigned int maxDistance = 0;

		for(auto it = engagedChannelList.begin(); it != engagedChannelList.end(); it++) {
			auto next = it;
			next++;
			if(next == engagedChannelList.end()) {
				break;
			}
			unsigned int currentDistance = *next - *it;
			if(currentDistance > maxDistance) {
				maxDistance = currentDistance;
				maxDistanceAt = *it;
			}
		}

		unsigned int recommendChannel = maxDistanceAt + (unsigned int)(maxDistance / 2);

		if(recommendChannel > 0 && recommendChannel < 14) {
			return recommendChannel;
		} else {
			return 0;
		}
	}

	bool TelnetProxy::ChangeWifiChannel(const size_t channel) const
	{
		if(channel > 1 && channel < 14) {

			const std::string command = "set wlan channel " + std::to_string(channel) + " 1\r\n";

			if(!SendAndWaitForEcho(command)) {
				Trace(ZONE_ERROR, ">>%s<< receive echo failed\n", command.data());
				return false;
			}
			timeval timeout = {50, 0};
			timeval endTime, now;
			gettimeofday(&endTime, NULL);
			timeval_add(&endTime, &timeout);
			do
			{
				try {
					if(Recv(PROMPT)) return true;
				} catch(std::exception &e) {
					std::cout << e.what() << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				gettimeofday(&now, NULL);
			}
			while(timeval_sub(&endTime, &now, &timeout));
		}
		return false;
	}


} /* namespace WyLight */

