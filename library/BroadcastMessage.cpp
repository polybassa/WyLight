//
//  BroadcastMessage.cpp
//  WyLightCLI
//
//  Created by Nils Weiss on 1/24/15.
//  Copyright (c) 2015 Nils Weiss. All rights reserved.
//

#include "BroadcastMessage.h"
#include "string.h"

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
