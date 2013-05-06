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

WyLight::ControlNoThrow::ControlNoThrow(uint32_t addr, uint16_t port)
: Control(addr, port)
{
}

uint32_t WyLight::ControlNoThrow::SolveException(void) const
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
uint32_t WyLight::ControlNoThrow::BlEnableAutostart(void) const
{
	return Try(&Control::BlEnableAutostart);
}

uint32_t WyLight::ControlNoThrow::BlEraseEeprom(void) const
{
	return Try(&Control::BlEraseEeprom);
}

uint32_t WyLight::ControlNoThrow::BlEraseFlash(void) const
{
	return Try(&Control::BlEraseFlash);
}

uint32_t WyLight::ControlNoThrow::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBlocks) const
{
	try {
		Control::BlReadCrcFlash(out, address, numBlocks);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const
{
	try {
		Control::BlReadEeprom(out, address, numBytes);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const
{
	try {
		Control::BlReadFlash(out, address, numBytes);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::BlReadFwVersion(std::string& versionString) const
{
	try {
		versionString = Control::BlReadFwVersion();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::BlReadInfo(BlInfo& blInfo) const
{
	try {
		Control::BlReadInfo(blInfo);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::BlProgramFlash(const std::string& filename) const
{
	try {
		Control::BlProgramFlash(filename);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::BlRunApp(void) const
{
	return Try(&Control::BlRunApp);
}

uint32_t WyLight::ControlNoThrow::ConfGetSsid(std::string& ssid) const
{
	ssid = Control::ConfGetSsid();
	return NO_ERROR;
}

uint32_t WyLight::ControlNoThrow::ConfModuleForWlan(const std::string &phrase, const std::string &ssid, const std::string &name) const
{
	return Control::ConfModuleForWlan(phrase, ssid, name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t WyLight::ControlNoThrow::ConfModuleAsSoftAP(const std::string &name) const
{
	return Control::ConfModuleAsSoftAP(name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t WyLight::ControlNoThrow::ConfRebootWlanModule(void) const
{
	return Control::ConfRebootWlanModule() ? NO_ERROR : FATAL_ERROR;
}

uint32_t WyLight::ControlNoThrow::ConfSetDeviceId(const std::string &name) const
{
	return Control::ConfSetDeviceId(name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t WyLight::ControlNoThrow::FwClearScript(void)
{
	try {
		Control::FwClearScript();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwGetCycletime(std::string& output)
{
	try {
		output = Control::FwGetCycletime();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwGetRtc(tm& timeValue)
{
	try {
		Control::FwGetRtc(timeValue);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwGetTracebuffer(std::string& output)
{
	try {
		output = Control::FwGetTracebuffer();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwGetVersion(std::string& output)
{
	try {
		output = Control::FwGetVersion();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwLoopOff(uint8_t numLoops)
{
	try {
		Control::FwLoopOff(numLoops);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwLoopOn(void)
{
	try {
		Control::FwLoopOn();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength)
{
	try {
		Control::FwSetColorDirect(pBuffer, bufferLength);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade)
{
	try {
		Control::FwSetFade(argb,fadeTime,addr,parallelFade);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwSetGradient(uint32_t argb_1, uint32_t argb_2, uint16_t fadeTime, bool parallelFade, uint8_t length, uint8_t offset)
{
	try {
		Control::FwSetGradient(argb_1, argb_2, fadeTime, parallelFade, length, offset);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
	
}

uint32_t WyLight::ControlNoThrow::FwSetRtc(const tm& timeValue)
{
	try {
		Control::FwSetRtc(timeValue);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwSetWait(uint16_t waitTime)
{
	try {
		Control::FwSetWait(waitTime);
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::FwStartBl(void)
{
	try {
		Control::FwStartBl();
		return NO_ERROR;
	} catch (...) {
		return SolveException();
	}
}

uint32_t WyLight::ControlNoThrow::Try(const std::function<void(const Control&)> call) const
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

