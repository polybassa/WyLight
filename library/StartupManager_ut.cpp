/*
   Copyright (C) 2012, 2013 Nils Weiss, Patrick Bruenn.

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

#include "trace.h"
#include "unittest.h"
#include "StartupManager.h"

namespace WyLight
{
static const uint32_t __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

typedef enum {
    TC_MODE_CHECK_FAIL,
    TC_START_BL_FAIL,
    TC_FW_VERSION_CHECK_FAIL,
    TC_BL_VERSION_CHECK_FAIL,
    TC_RUN_APP_FAIL,
    TC_UPDATE_FAIL,
    TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS,
    TC_BL_VERSION_CHECK_SUCCESS,
    TC_APP_OUTDATED
} TestCase;

TestCase g_Testcase;

const std::list<std::string> ConfigControl::RN171_BASIC_PARAMETERS;

/***** Wrappers ****/
ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) : mSock(0),
    mSockAddr(addr, port) {}
ClientSocket::~ClientSocket(void) {}
TcpSocket::TcpSocket(uint32_t addr, uint16_t port) : ClientSocket(addr, port, 0) {}
size_t TcpSocket::Recv(uint8_t* pBuffer, size_t length, timeval* timeout) const
{
    return 0;
}
size_t TcpSocket::Send(const uint8_t* frame, size_t length) const
{
    return 0;
}
ComProxy::ComProxy(const TcpSocket& sock) : mSock(sock) {}
TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock(sock) {}
UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast) : ClientSocket(
        addr,
        port,
        SOCK_DGRAM)
{}
size_t UdpSocket::Send(const uint8_t* frame, size_t length) const
{
    return length;
}

ConfigControl::ConfigControl(const TelnetProxy& telnet) : mTelnet(telnet){}

BootloaderControl::BootloaderControl(const ComProxy& proxy) : mProxy(proxy) {}

FirmwareControl::FirmwareControl(const UdpSocket& sock, const ComProxy& proxy) : mUdpSock(sock), mProxy(proxy) {}

bool ConfigControl::SetParameters(std::list<std::string> commands) const
{
    return true;
}

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

uint16_t FirmwareControl::FwGetVersion() const
{
    switch (g_Testcase) {
    case TC_START_BL_FAIL:
        return 0;

    case TC_FW_VERSION_CHECK_FAIL:
        throw FatalError("");

    case TC_APP_OUTDATED:
        return 1;

    default:
        return 0;
    }
}

std::string RN171FirmwareControl::FwGetCycletime(void) const
{
    return "";
}

void RN171FirmwareControl::FwGetRtc(tm& timeValue) const
{}

std::string RN171FirmwareControl::FwGetTracebuffer(void) const
{
    return "";
}

uint8_t RN171FirmwareControl::FwGetLedTyp(void) const
{
    return 0;
}

const FirmwareControl& FirmwareControl::operator<<(WyLight::FwCommand&& cmd) const
{
    if (strcmp(typeid(cmd).name(), typeid(FwCmdStartBl()).name()))
        switch (g_Testcase) {
        case TC_START_BL_FAIL:
        case TC_MODE_CHECK_FAIL:
        case TC_FW_VERSION_CHECK_FAIL:
            throw FatalError("");

        default:
            break;
        }

    return *this;
}

void BootloaderControl::BlEraseEeprom(void) const
{}

size_t Control::GetTargetMode(void) const
{
    switch (g_Testcase) {
    case TC_MODE_CHECK_FAIL:
        return 0;

    case TC_START_BL_FAIL:
    case TC_FW_VERSION_CHECK_FAIL:
    case TC_APP_OUTDATED:
        return FW_IDENT;

    case TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS:
    case TC_RUN_APP_FAIL:
    case TC_UPDATE_FAIL:
    case TC_BL_VERSION_CHECK_FAIL:
    case TC_BL_VERSION_CHECK_SUCCESS:
        return BL_IDENT;

    default:
        break;
    }

    return 0;
}

void BootloaderControl::BlProgramFlash(const std::string& pFilename) const
{
    switch (g_Testcase) {
    case TC_UPDATE_FAIL:
        throw FatalError("");

    default:
        return;
    }
}

uint16_t BootloaderControl::BlReadFwVersion(void) const
{
    switch (g_Testcase) {
    case TC_BL_VERSION_CHECK_FAIL:
        throw FatalError("");

    case TC_UPDATE_FAIL:
    case TC_RUN_APP_FAIL:
        return 0;

    default:
        return 0;
    }
}

uint16_t Control::ExtractFwVersion(const std::string& pFilename) const
{
    switch (g_Testcase) {
    case TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS:
    case TC_UPDATE_FAIL:
    case TC_START_BL_FAIL:
        return 1;

    default:
        return 0;
    }
}

void BootloaderControl::BlRunApp(void) const
{
    switch (g_Testcase) {
    case TC_RUN_APP_FAIL:
        throw FatalError("");

    default:
        return;
    }
}

size_t ut_StartupManager_VersionCheckSuccessUpdateSuccess(void)
{
    g_Testcase = TC_BL_VERSION_CHECK_SUCCESS;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_SUCCESSFUL == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_AppOutdated(void)
{
    g_Testcase = TC_APP_OUTDATED;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_SUCCESSFUL == testee.getCurrentState());
    CHECK(testee.isAppOutdated() == true);

    TestCaseEnd();
}

size_t ut_StartupManager_VersionFailUpdateSuccess(void)
{
    g_Testcase = TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_SUCCESSFUL == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_UpdateFail(void)
{
    g_Testcase = TC_UPDATE_FAIL;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_RunAppFail(void)
{
    g_Testcase = TC_RUN_APP_FAIL;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_BlVersionCheckFail(void)
{
    g_Testcase = TC_BL_VERSION_CHECK_FAIL;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_FwVersionCheckFail(void)
{
    g_Testcase = TC_FW_VERSION_CHECK_FAIL;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_StartBlFail(void)
{
    g_Testcase = TC_START_BL_FAIL;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();
    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

    TestCaseEnd();
}

size_t ut_StartupManager_ModeCheckFail(void)
{
    g_Testcase = TC_MODE_CHECK_FAIL;
    StartupManager testee;
    RN171Control ctrl(0, 0);
    TestCaseBegin();

    testee.startup(ctrl, "");

    CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

    TestCaseEnd();
}
} /* namespace WyLight */

using namespace WyLight;

int main(int argc, const char* argv[])
{
    UnitTestMainBegin();
    RunTest(true, ut_StartupManager_VersionFailUpdateSuccess);
    RunTest(true, ut_StartupManager_ModeCheckFail);
    RunTest(true, ut_StartupManager_StartBlFail);
    RunTest(true, ut_StartupManager_FwVersionCheckFail);
    RunTest(true, ut_StartupManager_BlVersionCheckFail);
    RunTest(true, ut_StartupManager_RunAppFail);
    RunTest(true, ut_StartupManager_UpdateFail);
    RunTest(true, ut_StartupManager_VersionCheckSuccessUpdateSuccess);
    RunTest(true, ut_StartupManager_AppOutdated);
    UnitTestMainEnd();
}
