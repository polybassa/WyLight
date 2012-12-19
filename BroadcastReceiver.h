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

#include <sys/socket.h>
#include <netinet/in.h>
#include <map>
#include <string>
#include <vector>
#include <boost/asio.hpp>
using boost::asio::ip::udp;
#include <boost/thread.hpp>


#pragma pack(push)
#pragma pack(1)
struct BroadcastMessage
{
	unsigned char mac[6];
	unsigned char channel;
	unsigned char rssi;
	unsigned short port;
	unsigned long rtc;
	unsigned short bat_mV;
	unsigned short gpioValue;
	char asciiTime[13+1];
	char version[26+1+1];// this seems a little strange. bug in wifly fw?
	char deviceId[32];
	unsigned short bootTmms;
	unsigned short sensor[8];

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
};
#pragma pack(pop)

class BroadcastReceiver
{
	public:
		static const unsigned short BROADCAST_PORT = 55555;
		static const char BROADCAST_DEVICE_ID[7];
		static const char STOP_MSG[];
		static const size_t STOP_MSG_LENGTH;
		const unsigned short mPort;

		BroadcastReceiver(unsigned short port);
		~BroadcastReceiver(void);

		/**
		 * Main loop wait for wifly broadcast messages and save them to the known IP list
		 */
		void operator()(void);

		/**
		 * Returns a list of IP address strings from available Wifly_Lights
		 * @param outputVector a vector to add the strings to
		 * @return number of IP address strings added to <outputVector>
		 */
		size_t GetIpTable(std::vector<boost::asio::ip::udp::endpoint>& outputVector);

		/**
		 * Sends a stop event to terminate execution of operator()
		 */
		void Stop(void);

	private:
		std::map<unsigned long, boost::asio::ip::udp::endpoint> mIpTable;
		boost::thread mThread;
		boost::mutex mMutex;
};

#endif /* #ifndef _BROADCAST_RECEIVER_H_ */
