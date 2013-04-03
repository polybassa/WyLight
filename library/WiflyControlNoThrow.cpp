/**
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

#include "WiflyControlNoThrow.h"

static const int g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

WiflyControlNoThrow::WiflyControlNoThrow(uint32_t addr, uint16_t port)
: WiflyControl(addr, port)
{
}

#define TRY_CATCH_RETURN(X,...) \
		try { \
		X(__VA_ARGS__); \
		return NO_ERROR; \
	} catch (ConnectionTimeout) { \
		return CONNECTION_TIMEOUT; \
	} catch (InvalidParameter) { \
		return INVALID_PARAMETER; \
	} catch (FatalError) { \
		return FATAL_ERROR; \
	}

/** ------------------------- BOOTLOADER METHODES ------------------------- **/
uint32_t WiflyControlNoThrow::BlEnableAutostart(void) const
{
	TRY_CATCH_RETURN(WiflyControl::BlEnableAutostart);
}

uint32_t WiflyControlNoThrow::BlEraseEeprom(void) const
{
	TRY_CATCH_RETURN(WiflyControl::BlEraseEeprom);
}

uint32_t WiflyControlNoThrow::BlEraseFlash(void) const
{
	TRY_CATCH_RETURN(WiflyControl::BlEraseEeprom);
}

size_t WiflyControlNoThrow::BlReadCrcFlash(unsigned char* pBuffer, unsigned int address, uint16_t numBlocks) const
{
	//TODO
	return 0;
}

size_t WiflyControlNoThrow::BlReadEeprom(uint8_t* pBuffer, uint32_t address, size_t numBytes) const
{
	//TODO
	return 0;
}

size_t WiflyControlNoThrow::BlReadFlash(uint8_t* pBuffer, uint32_t address, size_t numBytes) const
{
	//TODO
	return 0;
}

std::string WiflyControlNoThrow::BlReadFwVersion(void) const
{
	return std::string{};
}

uint32_t WiflyControlNoThrow::BlReadInfo(BlInfo& blInfo) const
{
	TRY_CATCH_RETURN(WiflyControl::BlReadInfo, blInfo);
}

uint32_t WiflyControlNoThrow::BlProgramFlash(const std::string& filename) const
{
	TRY_CATCH_RETURN(WiflyControl::BlProgramFlash, filename);
}

uint32_t WiflyControlNoThrow::BlRunApp(void) const
{
	TRY_CATCH_RETURN(WiflyControl::BlRunApp);
}

std::string WiflyControlNoThrow::ConfGetSsid(void) const
{
	return WiflyControl::ConfGetSsid();
}

bool WiflyControlNoThrow::ConfSetDefaults(void) const
{
	return WiflyControl::ConfSetDefaults();
}

bool WiflyControlNoThrow::ConfSetWlan(const std::string& phrase, const std::string& ssid) const
{
	return WiflyControl::ConfSetWlan(phrase, ssid);
}

uint32_t WiflyControlNoThrow::FwClearScript(void)
{
	TRY_CATCH_RETURN(WiflyControl::FwClearScript);
}

std::string WiflyControlNoThrow::FwGetCycletime(void)
{
	//TODO
	return std::string{};
}

uint32_t WiflyControlNoThrow::FwGetRtc(tm& timeValue)
{
	TRY_CATCH_RETURN(WiflyControl::FwGetRtc, timeValue);
}

std::string WiflyControlNoThrow::FwGetTracebuffer(void)
{
	//TODO
	return std::string{};
}

std::string WiflyControlNoThrow::FwGetVersion(void)
{
	//TODO
	return std::string{};
}

uint32_t WiflyControlNoThrow::FwLoopOff(uint8_t numLoops)
{
	TRY_CATCH_RETURN(WiflyControl::FwLoopOff, numLoops);
}

uint32_t WiflyControlNoThrow::FwLoopOn(void)
{
	TRY_CATCH_RETURN(WiflyControl::FwLoopOn);
}

uint32_t WiflyControlNoThrow::FwSetColorDirect(const uint8_t* pBuffer, size_t bufferLength)
{
	TRY_CATCH_RETURN(WiflyControl::FwSetColorDirect, pBuffer, bufferLength);
}

uint32_t WiflyControlNoThrow::FwSetFade(uint32_t argb, uint16_t fadeTime, uint32_t addr, bool parallelFade)
{
	TRY_CATCH_RETURN(WiflyControl::FwSetFade, argb, fadeTime, addr, parallelFade);
}

uint32_t WiflyControlNoThrow::FwSetRtc(const tm& timeValue)
{
	TRY_CATCH_RETURN(WiflyControl::FwSetRtc, timeValue);
}

uint32_t WiflyControlNoThrow::FwSetWait(uint16_t waitTime)
{
	TRY_CATCH_RETURN(WiflyControl::FwSetWait, waitTime);
}

uint32_t WiflyControlNoThrow::FwStartBl(void)
{
	TRY_CATCH_RETURN(WiflyControl::FwStartBl);
}

