/*
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#ifndef __BROADCAST_TRANSMITTER_H_
#define __BROADCAST_TRANSMITTER_H_

#include <stdint.h>

#include "CPPTask.h"
#include "WifiConsumer.h"

#define BC_PORT_NUM        	55555

class BroadcastTransmitter final : public Task, WifiConsumer {
	struct __attribute__((__packed__)) BroadcastMessage {
		uint8_t MAC[6];
		uint8_t channel;
		uint8_t rssi;
		uint16_t port;
		uint32_t rtc;
		uint16_t battery;
		uint16_t gpio;
		uint8_t asciiTime[14];
		uint8_t version[28];
		uint8_t deviceId[32];
		uint16_t boottime;
		uint8_t sensors[16];
		void refresh(void);
	};
	
	BroadcastMessage mMsg;
	static const uint16_t port = BC_PORT_NUM;
	
public:
	BroadcastTransmitter(void);
	BroadcastTransmitter(const BroadcastTransmitter&) = delete;
	BroadcastTransmitter& operator=(const BroadcastTransmitter&) = delete;
	BroadcastTransmitter(BroadcastTransmitter&&) = delete;
	
	virtual void run(void);
	virtual void stop(void);
};

#endif
