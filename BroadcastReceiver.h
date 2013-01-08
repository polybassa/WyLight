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

#ifndef _BROADCAST_RECEIVER_H_
#define _BROADCAST_RECEIVER_H_

#include "ClientSocket.h"
#include <cstdint>
#include <cstring>
#include <map>
#include <ostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>

using std::vector;

class BroadcastReceiver
{
	public:
		static const uint16_t BROADCAST_PORT = 55555;
		static const char BROADCAST_DEVICE_ID[];
		static const size_t BROADCAST_DEVICE_ID_LENGTH;
		static const char STOP_MSG[];
		static const size_t STOP_MSG_LENGTH;
		const uint16_t mPort;

		BroadcastReceiver(uint16_t port = BROADCAST_PORT);
		~BroadcastReceiver(void);

		/**
		 * Wait for broadcasts and print them to a stream
		 * @param out stream to print collected remotes on
		 * @param timeout in seconds, until execution is terminated
		 */
		void operator() (std::ostream& out, unsigned short timeout);

		uint32_t GetIp(size_t index) const;
		uint32_t GetNextRemote(void);
		uint16_t GetPort(size_t index) const;

		/**
		 * @return number of known IP addresses
		 */
		size_t NumRemotes(void) const;

		/**
		 * Print remote list to outputstream
		 * @param out outputstream
		 */
		void ShowRemotes(std::ostream& out) const;

		/**
		 * Sends a stop event to terminate execution of operator()
		 */
		void Stop(void);

	private:
		vector<Endpoint*> mIpTable;
		pthread_t mThread;
		pthread_mutex_t mMutex;
};

#pragma pack(push)
#pragma pack(1)
struct BroadcastMessage
{
	uint8_t mac[6];
	uint8_t channel;
	uint8_t rssi;
	uint16_t port;
	uint32_t rtc;
	uint16_t bat_mV;
	uint16_t gpioValue;
	char asciiTime[13+1];
	char version[26+1+1];// this seems a little strange. bug in wifly fw?
	char deviceId[32];
	uint16_t bootTmms;
	uint16_t sensor[8];

	bool IsWiflyBroadcast(size_t length) {
		return ((sizeof(BroadcastMessage) == length) && (0 == memcmp(deviceId,	BroadcastReceiver::BROADCAST_DEVICE_ID, BroadcastReceiver::BROADCAST_DEVICE_ID_LENGTH)));
	};

// was used only for debugging
// deactivated because of possible byte order issues using IsWiflyBroadcast()
#if 0 
	void NetworkToHost(void) {
		port = ntohs(port);
		rtc = ntohl(rtc);
		bat_mV = ntohs(bat_mV);
		gpioValue = ntohs(gpioValue);
		bootTmms = ntohs(bootTmms);
		for(size_t i = 0; i < sizeof(sensor)/sizeof(sensor[0]); i++) {
			sensor[i] = ntohs(sensor[i]);
		}
	};

	void Print(std::ostream& out) const {

		out << "MAC:        "	<< std::hex
		<< (int) mac[0] << ' ' << (int) mac[1] << ' ' << (int) mac[2] << ' '
		<< (int) mac[3] << ' ' << (int) mac[4] << ' ' << (int) mac[5]
		<< "\nChannel:  " << std::dec << (int)channel
		<< "\nRssi:     " << (int)rssi
		<< "%\nPort:     " << port 
		<< "\nRTC:      0x" << std::hex << rtc
		<< "\nBat:      " << std::dec << bat_mV
		<< "mV\nGPIO:     0x" << std::hex << gpioValue
		<< "\nTime:     " << asciiTime
		<< "\nVersion:  " << version
		<< "\nDeviceID: " << deviceId
		<< "\nBoottime: " << std::dec << bootTmms << "ms\n";
	};
#endif
};
#pragma pack(pop)
#endif /* #ifndef _BROADCAST_RECEIVER_H_ */
