/**
		Copyright (C) 2013 Nils Weiss, Patrick Bruenn.

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

#include "WiflyControlNoThrow.h"

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

WiflyControlNoThrow::WiflyControlNoThrow(uint32_t addr, uint16_t port)
: WiflyControl(addr, port)
{
}

uint32_t WiflyControlNoThrow::SolveException(void) const
{
	try {
		throw;
	} catch (FatalError& e) {
		return e.AsErrorCode();
	} catch (std::exception) {
		std::cout << "CATCH std::exception";
		std::cerr << "CATCH std::exception";
		//std::terminate();
		return FATAL_ERROR;
	}
}

/** ------------------------- BOOTLOADER METHODES ------------------------- **/
uint32_t WiflyControlNoThrow::BlEnableAutostart(void) const
{
	return Try(&WiflyControl::BlEnableAutostart);
}

uint32_t WiflyControlNoThrow::BlEraseEeprom(void) const
{
	return Try(&WiflyControl::BlEraseEeprom);
}

uint32_t WiflyControlNoThrow::BlEraseFlash(void) const
{
	return Try(&WiflyControl::BlEraseFlash);
}

uint32_t WiflyControlNoThrow::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBlocks) const
{
	try {
		WiflyControl::BlReadCrcFlash(out, address, numBlocks);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const
{
	try {
		WiflyControl::BlReadEeprom(out, address, numBytes);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const
{
	try {
		WiflyControl::BlReadFlash(out, address, numBytes);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::BlReadFwVersion(std::string& versionString) const
{
	try {
		versionString = WiflyControl::BlReadFwVersion();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::BlReadInfo(BlInfo& blInfo) const
{
	try {
		WiflyControl::BlReadInfo(blInfo);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::BlProgramFlash(const std::string& filename) const
{
	try {
		WiflyControl::BlProgramFlash(filename);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::BlRunApp(void) const
{
	return Try(&WiflyControl::BlRunApp);
}

uint32_t WiflyControlNoThrow::ConfGetSsid(std::string& ssid) const
{
	ssid = WiflyControl::ConfGetSsid();
	return NO_ERROR;
}

uint32_t WiflyControlNoThrow::ConfSetDefaults(void) const
{
	return WiflyControl::ConfSetDefaults() ? NO_ERROR : FATAL_ERROR;
}

uint32_t WiflyControlNoThrow::ConfSetWlan(const std::string& phrase, const std::string& ssid) const
{
	return WiflyControl::ConfSetWlan(phrase, ssid) ? NO_ERROR : FATAL_ERROR;
}

uint32_t WiflyControlNoThrow::ConfRebootWlanModul(void) const
{
	return WiflyControl::ConfRebootWlanModul() ? NO_ERROR : FATAL_ERROR;
}

uint32_t WiflyControlNoThrow::ConfSetDeviceId(const std::string &name) const
{
	return WiflyControl::ConfSetDeviceId(name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t WiflyControlNoThrow::FwClearScript(void)
{
	try {
		WiflyControl::FwClearScript();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwGetCycletime(std::string& output)
{
	try {
		output = WiflyControl::FwGetCycletime();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwGetRtc(tm& timeValue)
{
	try {
		WiflyControl::FwGetRtc(timeValue);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwGetTracebuffer(std::string& output)
{
	try {
		output = WiflyControl::FwGetTracebuffer();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwGetVersion(std::string& output)
{
	try {
		output = WiflyControl::FwGetVersion();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwLoopOff(uint8_t numLoops)
{
	try {
		WiflyControl::FwLoopOff(numLoops);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwLoopOn(void)
{
	try {
		WiflyControl::FwLoopOn();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength)
{
	try {
		WiflyControl::FwSetColorDirect(pBuffer, bufferLength);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade)
{
	try {
		WiflyControl::FwSetFade(argb,fadeTime,addr,parallelFade);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwSetRtc(const tm& timeValue)
{
	try {
		WiflyControl::FwSetRtc(timeValue);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwSetWait(uint16_t waitTime)
{
	try {
		WiflyControl::FwSetWait(waitTime);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::FwStartBl(void)
{
	try {
		WiflyControl::FwStartBl();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WiflyControlNoThrow::Try(const std::function<void(const WiflyControl&)> call) const
{
	try {
		call(*this);
		return NO_ERROR;
	} catch (FatalError& e) {
		return e.AsErrorCode();
	} catch (std::exception) {
		std::cout << "CATCH std::exception";
		std::cerr << "CATCH std::exception";
		//std::terminate();
		return FATAL_ERROR;
	}
}

