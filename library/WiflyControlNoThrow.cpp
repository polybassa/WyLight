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

namespace WyLight
{
static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

ControlNoThrow::ControlNoThrow(uint32_t addr, uint16_t port) : mControl(addr, port) {}

uint32_t ControlNoThrow::BlEnableAutostart(void) const
{
    return Try(std::bind(&BootloaderControl::BlEnableAutostart, std::ref(mControl.mBootloader)));
}

uint32_t ControlNoThrow::BlEraseEeprom(void) const
{
    return Try(std::bind(&BootloaderControl::BlEraseEeprom, std::ref(mControl.mBootloader)));
}

uint32_t ControlNoThrow::BlEraseFlash(void) const
{
    return Try(std::bind(&BootloaderControl::BlEraseFlash, std::ref(mControl.mBootloader)));
}

uint32_t ControlNoThrow::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBlocks) const
{
    return Try(std::bind(static_cast<void (BootloaderControl::*)(std::ostream&, uint32_t,
                                                                 size_t) const>(&BootloaderControl::BlReadCrcFlash),
                         std::ref(mControl.mBootloader),
                         std::ref(out), address, numBlocks));
}

uint32_t ControlNoThrow::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const
{
    return Try(std::bind(static_cast<void (BootloaderControl::*)(std::ostream&, uint32_t,
                                                                 size_t) const>(&BootloaderControl::BlReadEeprom),
                         std::ref(mControl.mBootloader), std::ref(out), address, numBytes));
}

uint32_t ControlNoThrow::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const
{
    return Try(std::bind(static_cast<void (BootloaderControl::*)(std::ostream&, uint32_t,
                                                                 size_t) const>(&BootloaderControl::BlReadFlash),
                         std::ref(mControl.mBootloader), std::ref(out), address, numBytes));
}

uint32_t ControlNoThrow::BlReadFwVersion(uint16_t& version) const
{
    return Try(std::bind(&BootloaderControl::BlReadFwVersion, std::ref(mControl.mBootloader)), version);
}

uint32_t ControlNoThrow::BlReadInfo(BlInfo& blInfo) const
{
    return Try(std::bind(&BootloaderControl::BlReadInfo, std::ref(mControl.mBootloader), std::ref(blInfo)));
}

uint32_t ControlNoThrow::BlProgramFlash(const std::string& filename) const
{
    return Try(std::bind(&BootloaderControl::BlProgramFlash, std::ref(mControl.mBootloader), filename));
}

uint32_t ControlNoThrow::BlRunApp(void) const
{
    return Try(std::bind(&BootloaderControl::BlRunApp, std::ref(mControl.mBootloader)));
}

uint32_t ControlNoThrow::ConfGetSsid(std::string& ssid) const
{
    ssid = mControl.mConfig.GetSsid();
    return NO_ERROR;
}

uint32_t ControlNoThrow::ConfModuleForWlan(const std::string& phrase, const std::string& ssid,
                                           const std::string& name) const
{
    return mControl.mConfig.ModuleForWlan(phrase, ssid, name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t ControlNoThrow::ConfModuleAsSoftAP(const std::string& name) const
{
    return mControl.mConfig.ModuleAsSoftAP(name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t ControlNoThrow::ConfRebootWlanModule(void) const
{
    return mControl.mConfig.RebootWlanModule() ? NO_ERROR : FATAL_ERROR;
}

uint32_t ControlNoThrow::ConfSetDeviceId(const std::string& name) const
{
    return mControl.mConfig.SetDeviceId(name) ? NO_ERROR : FATAL_ERROR;
}

uint32_t ControlNoThrow::ConfChangeWlanChannel(void) const
{
    return mControl.mConfig.ChangeWlanChannel() ? NO_ERROR : FATAL_ERROR;
}

uint32_t ControlNoThrow::ConfSetParameters(std::list<std::string> commands) const
{
    return mControl.mConfig.SetParameters(commands) ? NO_ERROR : FATAL_ERROR;
}

uint32_t ControlNoThrow::FwClearScript(void)
{
    return Try(FwCmdClearScript {}
               );
}

uint32_t ControlNoThrow::FwGetCycletime(std::string& output)
{
    return Try(std::bind(&FirmwareControl::FwGetCycletime, std::ref(mControl.mFirmware)), output);
}

uint32_t ControlNoThrow::FwGetRtc(tm& timeValue)
{
    return Try(std::bind(&FirmwareControl::FwGetRtc, std::ref(mControl.mFirmware), std::ref(timeValue)));
}

uint32_t ControlNoThrow::FwGetTracebuffer(std::string& output)
{
    return Try(std::bind(&FirmwareControl::FwGetTracebuffer, std::ref(mControl.mFirmware)), output);
}

uint32_t ControlNoThrow::FwGetVersion(uint16_t& output)
{
    return Try(std::bind(&FirmwareControl::FwGetVersion, std::ref(mControl.mFirmware)), output);
}

uint32_t ControlNoThrow::FwGetLedTyp(uint8_t& output)
{
    return Try(std::bind(&FirmwareControl::FwGetLedTyp, std::ref(mControl.mFirmware)), output);
}

uint32_t ControlNoThrow::FwLoopOff(const uint8_t numLoops)
{
    return Try(FwCmdLoopOff {numLoops}
               );
}

uint32_t ControlNoThrow::FwLoopOn(void)
{
    return Try(FwCmdLoopOn {}
               );
}

uint32_t ControlNoThrow::FwSetColorDirect(const std::vector<uint8_t> buffer)
{
    return Try(FwCmdSetColorDirect {buffer.data(), buffer.size()}
               );
}

uint32_t ControlNoThrow::FwSetFade(const uint32_t argb,
                                   const uint16_t fadeTime,
                                   const uint32_t addr,
                                   const bool     parallelFade)
{
    return Try(FwCmdSetFade {argb, fadeTime, addr, parallelFade}
               );
}

uint32_t ControlNoThrow::FwSetGradient(const uint32_t argb_1,
                                       const uint32_t argb_2,
                                       const uint16_t fadeTime,
                                       const bool     parallelFade,
                                       const uint8_t  length,
                                       uint8_t        offset)
{
    return Try(FwCmdSetGradient {argb_1, argb_2, fadeTime, parallelFade, length, offset}
               );
}

uint32_t ControlNoThrow::FwSetRtc(const tm& timeValue)
{
    return Try(FwCmdSetRtc {timeValue}
               );
}

uint32_t ControlNoThrow::FwSetWait(const uint16_t waitTime)
{
    return Try(FwCmdWait {waitTime}
               );
}

uint32_t ControlNoThrow::FwStartBl(void)
{
    return Try(FwCmdStartBl {}
               );
}

uint32_t ControlNoThrow::ExtractFwVersion(const std::string& pFilename, uint16_t& extractedFwVersion) const
{
    return Try(std::bind(&Control::ExtractFwVersion, std::ref(mControl), pFilename), extractedFwVersion);
}

uint32_t ControlNoThrow::Try(FwCommand&& cmd)
{
    try {
        mControl.mFirmware << std::move(cmd);
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

uint32_t ControlNoThrow::Try(const std::function<std::string(void)> call, std::string& returnString) const
{
    try {
        returnString = call();
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

uint32_t ControlNoThrow::Try(const std::function<uint16_t(void)> call, uint16_t& returnValue) const
{
    try {
        returnValue = call();
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

uint32_t ControlNoThrow::Try(const std::function<uint8_t(void)> call, uint8_t& returnValue) const
{
    try {
        returnValue = call();
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

uint32_t ControlNoThrow::Try(const std::function<void(void)> call) const
{
    try {
        call();
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
} /* namespace WyLight */
