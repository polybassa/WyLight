/**
                Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

    This file is part of WyLight.

    WyLight is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    WyLight is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with WyLight.  If not, see <http://www.gnu.org/licenses/>. */

#include "WiflyControl.h"
#include "trace.h"
#include "Version.h"
#include "intelhexclass.h"

#include <algorithm>
#include <sstream>
#include <cctype>
#include <stddef.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#include <thread>
#include <chrono>

using std::cout;
using std::hex;
using std::ifstream;

namespace WyLight
{
static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const size_t FwCmdScript::INDENTATION_MAX;
const char FwCmdScript::INDENTATION_CHARACTER;

const std::string FwCmdSetFade::TOKEN("fade");
const std::string FwCmdSetGradient::TOKEN("gradient");
const std::string FwCmdLoopOn::TOKEN("loop");
const std::string FwCmdLoopOff::TOKEN("loop_off");
const std::string FwCmdWait::TOKEN("wait");

Control::Control(uint32_t                 addr,
                 uint16_t                 port,
                 const BootloaderControl& bootloader,
                 const ConfigControl&     config,
                 const FirmwareControl&   firmware) :
    mBootloader(bootloader),
    mConfig(config),
    mFirmware(firmware),
    mTcpSock(addr, port),
    mUdpSock(addr, port, false, 0),
    mProxy(mTcpSock),
    mTelnet(mTcpSock)

{}

size_t Control::GetTargetMode(void) const
{
    return mProxy.SyncWithTarget();
}

uint16_t Control::ExtractFwVersion(const std::string& pFilename) const
{
    std::ifstream hexFile;
    hexFile.open(const_cast<char*>(pFilename.c_str()), ifstream::in);

    if (!hexFile.good())
        throw FatalError("opening '" + pFilename + "' failed");

    intelhex hexConverter;
    hexFile >> hexConverter;
    union versionUnion {
        uint16_t version;
        uint8_t bytes[2];
    }
    versUnion;

    hexConverter.getData(&versUnion.bytes[1], VERSION_STRING_ORIGIN);
    hexConverter.getData(&versUnion.bytes[0], VERSION_STRING_ORIGIN + 1);

    return ntohs(versUnion.version) > 300 ? 0 : ntohs(versUnion.version);
}

RN171Control::RN171Control(uint32_t addr, uint16_t port) : Control(addr,
                                                                   port,
                                                                   mBootloaderInstance,
                                                                   mConfigInstance,
                                                                   mFirmwareInstance), mBootloaderInstance(mProxy),
    mConfigInstance(mTelnet),
    mFirmwareInstance(mUdpSock, mProxy) {}

CC3200Control::CC3200Control(uint32_t addr, uint16_t port) : Control(addr,
                                                                     port,
                                                                     mBootloaderInstance,
                                                                     mConfigInstance,
                                                                     mFirmwareInstance), mBootloaderInstance(mProxy),
    mConfigInstance(mTelnet),
    mFirmwareInstance(mUdpSock, mProxy) {}
} /* namespace WyLight */
