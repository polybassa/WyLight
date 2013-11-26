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

namespace WyLight {

	static const uint32_t g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

	typedef enum {
		TC_MODE_CHECK_FAIL,
		TC_START_BL_FAIL,
		TC_FW_VERSION_CHECK_FAIL,
		TC_BL_VERSION_CHECK_FAIL,
		TC_RUN_APP_FAIL,
		TC_UPDATE_FAIL,
		TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS,
		TC_BL_VERSION_CHECK_SUCCESS
	} TestCase;

	TestCase g_Testcase;

	const std::list<std::string> Control::RN171_BASIC_PARAMETERS;

	/***** Wrappers ****/
	ClientSocket::ClientSocket(uint32_t addr, uint16_t port, int style) throw (FatalError) : mSock(0), mSockAddr(addr, port) {}
	ClientSocket::~ClientSocket(void) {}
	TcpSocket::TcpSocket(uint32_t addr, uint16_t port) throw (ConnectionLost, FatalError) : ClientSocket(addr, port, 0) {}
	size_t TcpSocket::Recv(uint8_t *pBuffer, size_t length, timeval *timeout) const throw (FatalError) {
		return 0;
	}
	size_t TcpSocket::Send(const uint8_t *frame, size_t length) const {
		return 0;
	}
	ComProxy::ComProxy(const TcpSocket& sock) : mSock (sock) {}
	TelnetProxy::TelnetProxy(const TcpSocket& sock) : mSock (sock) {}
	UdpSocket::UdpSocket(uint32_t addr, uint16_t port, bool doBind, int enableBroadcast) throw (FatalError) : ClientSocket(addr, port, SOCK_DGRAM) {}
	size_t UdpSocket::Send(const uint8_t *frame, size_t length) const {
		return length;
	}


	Control::Control(uint32_t addr, uint16_t port)
		: mTcpSock(addr, port), mUdpSock(addr, port, false, 0), mProxy(mTcpSock), mTelnet(mTcpSock)
	{}

	uint16_t Control::FwGetVersion() throw (WyLight::ConnectionTimeout, WyLight::FatalError, WyLight::ScriptBufferFull) {
		switch(g_Testcase) {
		case TC_START_BL_FAIL:
			return 0;
		case TC_FW_VERSION_CHECK_FAIL:
			throw FatalError("");

		default:
			return 0;
		}
	}

	Control& Control::operator<<(WyLight::FwCommand &&cmd) throw (WyLight::ConnectionTimeout, WyLight::FatalError, WyLight::ScriptBufferFull) {
		if(strcmp( typeid(cmd).name(), typeid(FwCmdStartBl()).name())) {
			switch(g_Testcase) {
			case TC_START_BL_FAIL:
			case TC_MODE_CHECK_FAIL:
			case TC_FW_VERSION_CHECK_FAIL:
				throw FatalError("");
			default:
				break;
			}
		}



		return *this;
	}

	void Control::BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError)
	{}

	bool Control::ConfSetParameters(std::list<std::string> commands) const {
		return true;
	}


	size_t Control::GetTargetMode(void) const throw(FatalError)
	{
		switch(g_Testcase) {
		case TC_MODE_CHECK_FAIL:
			return 0;
		case TC_START_BL_FAIL:
		case TC_FW_VERSION_CHECK_FAIL:
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

	void Control::BlProgramFlash(const std::string& pFilename) const throw (ConnectionTimeout, FatalError)
	{
		switch(g_Testcase) {
		case TC_UPDATE_FAIL:
			throw FatalError("");

		default:
			return;
		}
	}

	uint16_t Control::BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError)
	{
		switch(g_Testcase) {
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
		switch(g_Testcase) {
		case TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS:
		case TC_UPDATE_FAIL:
		case TC_START_BL_FAIL:
			return 1;

		default:
			return 0;
		}
	}

	void Control::BlRunApp(void) const throw (ConnectionTimeout, FatalError)
	{
		switch(g_Testcase) {
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
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_SUCCESSFUL == testee.getCurrentState());

		TestCaseEnd();

	}

	size_t ut_StartupManager_VersionFailUpdateSuccess(void)
	{
		g_Testcase = TC_BL_VERSION_CHECK_FAIL_UPDATE_SUCCESS;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_SUCCESSFUL == testee.getCurrentState());

		TestCaseEnd();

	}

	size_t ut_StartupManager_UpdateFail(void)
	{
		g_Testcase = TC_UPDATE_FAIL;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

		TestCaseEnd();

	}

	size_t ut_StartupManager_RunAppFail(void)
	{
		g_Testcase = TC_RUN_APP_FAIL;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

		TestCaseEnd();

	}

	size_t ut_StartupManager_BlVersionCheckFail(void)
	{
		g_Testcase = TC_BL_VERSION_CHECK_FAIL;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

		TestCaseEnd();
	}

	size_t ut_StartupManager_FwVersionCheckFail(void)
	{
		g_Testcase = TC_FW_VERSION_CHECK_FAIL;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

		TestCaseEnd();
	}

	size_t ut_StartupManager_StartBlFail(void)
	{
		g_Testcase = TC_START_BL_FAIL;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();
		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

		TestCaseEnd();
	}

	size_t ut_StartupManager_ModeCheckFail(void)
	{
		g_Testcase = TC_MODE_CHECK_FAIL;
		StartupManager testee;
		Control ctrl(0,0);
		TestCaseBegin();

		testee.startup(ctrl, "");

		CHECK(StartupManager::STARTUP_FAILURE == testee.getCurrentState());

		TestCaseEnd();
	}
} /* namespace WyLight */

using namespace WyLight;

int main (int argc, const char *argv[])
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
	UnitTestMainEnd();
}
