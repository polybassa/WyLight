/*
 Copyright (C) 2015 Nils Weiss, Patrick Bruenn.
 
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

#include "BroadcastMessage.h"
#include <string.h>

using WyLight::BroadcastMessage;
using WyLight::RN171BroadcastMessage;
using WyLight::CC3200BroadcastMessage;


bool BroadcastMessage::IsVersion(const std::string& deviceVersion) const {
	return (0 == memcmp(version, deviceVersion.data(), deviceVersion.size()));
}

const std::string RN171BroadcastMessage::DEVICE_VERSION("WiFly Ver 2.45, 10-09-2012");
const std::string RN171BroadcastMessage::DEVICE_VERSION4("wifly-EZX Ver 4.00.1, Apr 19");
const std::string RN171BroadcastMessage::DEVICE_ID("Wifly_Light");
const std::string RN171BroadcastMessage::DEVICE_ID_OLD("WiFly");

bool RN171BroadcastMessage::IsRN171Broadcast(const BroadcastMessage& msg, const size_t length) {
    return ((sizeof(BroadcastMessage) == length)
            &&  (msg.IsVersion(DEVICE_VERSION)
                || msg.IsVersion(DEVICE_VERSION4)
                || msg.IsDevice(DEVICE_ID)
                || msg.IsDevice(DEVICE_ID_OLD)));
}

bool BroadcastMessage::IsDevice(const std::string& deviceType) const {
				return (0 == memcmp(deviceId,   deviceType.data(), deviceType.size()));
}

const std::string CC3200BroadcastMessage::DEVICE_VERSION("WyLight CC3200 Version 0.1");

bool CC3200BroadcastMessage::IsCC3200Broadcast(const WyLight::BroadcastMessage &msg, const size_t length) {
    return (sizeof(BroadcastMessage) == length) &&  msg.IsVersion(DEVICE_VERSION);
}
