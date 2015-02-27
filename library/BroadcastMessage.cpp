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
using WyLight::CC3200BroadcastMessage;

bool BroadcastMessage::IsVersion(const std::string& deviceVersion) const
{
    return 0 == deviceVersion.compare(std::string(version, sizeof(version)));
}

const std::string BroadcastMessage::CC3200_VERSION("WyLight CC3200 Version 0.1");
const std::string BroadcastMessage::RN171_VERSION_2_45("WiFly Ver 2.45, 10-09-2012");
const std::string BroadcastMessage::RN171_VERSION_4_00("wifly-EZX Ver 4.00.1, Apr 19");

bool BroadcastMessage::IsRN171Broadcast() const
{
    return IsVersion(RN171_VERSION_2_45) || IsVersion(RN171_VERSION_4_00);
}

bool BroadcastMessage::IsCC3200Broadcast() const
{
    return IsVersion(CC3200_VERSION);
}
