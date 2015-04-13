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

Control::Control(uint32_t                                   addr,
                 uint16_t                                   port,
                 std::unique_ptr<const BootloaderControl>&& bootloader,
                 std::unique_ptr<const ConfigControl>&&     config,
                 std::unique_ptr<const FirmwareControl>&&   firmware) :
    mBootloader(std::move(bootloader)),
    mConfig(std::move(config)),
    mFirmware(std::move(firmware)),
    mTcpSock(addr, port),
    mUdpSock(addr, port, false, 0),
    mProxy(mTcpSock),
    mTelnet(mTcpSock)
{}

size_t Control::GetTargetMode(void) const
{
    return mProxy.SyncWithTarget();
}

{
}

RN171Control::RN171Control(uint32_t addr, uint16_t port) : Control(addr,
                                                                   port,
                                                                   std::unique_ptr<const BootloaderControl>(new
                                                                                                            BootloaderControl(
                                                                                                                mProxy)),
                                                                   std::unique_ptr<const ConfigControl>(new
                                                                                                        ConfigControl(
                                                                                                            mTelnet)),
                                                                   std::unique_ptr<const FirmwareControl>(new
                                                                                                          RN171FirmwareControl(
                                                                                                              mUdpSock,
                                                                                                              mProxy))){}

CC3200Control::CC3200Control(uint32_t addr, uint16_t port) : Control(addr,
                                                                     port,
                                                                     nullptr,
                                                                     nullptr,
                                                                     std::unique_ptr<const FirmwareControl>(new
                                                                                                            RN171FirmwareControl(
                                                                                                                mUdpSock,
                                                                                                                mProxy)))
{}
} /* namespace WyLight */
