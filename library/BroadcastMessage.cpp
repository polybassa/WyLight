//
//  BroadcastMessage.cpp
//  WyLightCLI
//
//  Created by Nils Weiss on 1/24/15.
//  Copyright (c) 2015 Nils Weiss. All rights reserved.
//

#include "BroadcastMessage.h"

using WyLight::BroadcastMessage;
using WyLight::WiflyBroadcastMessage;

const std::string WiflyBroadcastMessage::DEVICE_ID("Wifly_Light");
const std::string WiflyBroadcastMessage::DEVICE_ID_OLD("WiFly");
const std::string WiflyBroadcastMessage::DEVICE_VERSION("WiFly Ver 2.45, 10-09-2012");
const std::string WiflyBroadcastMessage::DEVICE_VERSION4("wifly-EZX Ver 4.00.1, Apr 19");

bool WiflyBroadcastMessage::IsWiflyBroadcast(const WiflyBroadcastMessage& msg, const size_t length) {
    return ((sizeof(BroadcastMessage) == length)
            && (msg.IsDevice(WiflyBroadcastMessage::DEVICE_ID)
                || msg.IsDevice(WiflyBroadcastMessage::DEVICE_ID_OLD)
                || msg.IsVersion(WiflyBroadcastMessage::DEVICE_VERSION)
                || msg.IsVersion(WiflyBroadcastMessage::DEVICE_VERSION4)));
};

bool WiflyBroadcastMessage::IsVersion(const std::string& deviceVersion) const {
				return (0 == memcmp(version, deviceVersion.data(), deviceVersion.size()));
}

bool WiflyBroadcastMessage::IsDevice(const std::string& deviceType) const {
				return (0 == memcmp(deviceId,   deviceType.data(), deviceType.size()));
};
