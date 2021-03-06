/*
                Copyright (C) 2012 - 2015 Nils Weiss, Patrick Bruenn.

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

#ifndef _FIRMWARECONTROL_H_
#define _FIRMWARECONTROL_H_

#include <string>
#include "ComProxy.h"
#include "WiflyControlException.h"
#include "FwCommand.h"
#include "Script.h"

namespace WyLight
{
/******************************************************************************/
/*! \file FirmwareControl.h
 * \author Nils Weiss, Patrick Bruenn
 *
 *! \cond
 * class - FirmwareControl
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Firmware.
 *
 *******************************************************************************/

class FirmwareControl {
public:
    /**
     * string constant to address all LEDs. String representation of 0xffffffff
     */
    static const std::string LEDS_ALL;
    /**
     * Connect to a wifly device
     * @param addr ipv4 address as 32 bit value in host byte order
     * @param port number of the wifly device server in host byte order
     */
    FirmwareControl(const UdpSocket& udp, const ComProxy& proxy);

    /**
     * Reads the cycletimes from wifly device and stores them into the response object
     * @return a string with all recorded cycletimes from PIC firmware
     */
    virtual std::string FwGetCycletime(void) const = 0;

    /**
     * Reads the current rtc time from the wifly device
     * @param timeValue reference to a tm object, where to store the rtc time from PIC firmware
     */
    virtual void FwGetRtc(tm& timeValue) const = 0;

    /**
     * Reads the tracebuffer from wifly device and stores the data into the response object
     * @return a string with all recorded trace messages from PIC firmware
     */
    virtual std::string FwGetTracebuffer(void) const = 0;

    /**
     * Reads the firmware version currently running on the wifly device.
     * @return a string representing the version number of the PIC firmware
     */
    virtual uint16_t FwGetVersion(void) const;

    /**
     * Reads the led typ on the spi interface. To detect WS2801 Led's, the SPI IN and OUT Pin's has to conntect together, to build a loopback.
     * @return a value representing the led typ of the WyLight modul
     */
    virtual uint8_t FwGetLedTyp(void) const = 0;

    /**
     * Methode to extract the firmware version from a hex file
     * @return the version string from a given hex file
     */
    virtual uint16_t ExtractFwVersion(const std::string& pFilename) const = 0;

    //TODO move this test functions to the integration test
    void FwTest(void) const;
    void FwStressTest(void) const;

    const FirmwareControl& operator<<(FwCommand&& cmd) const;
    const FirmwareControl& operator<<(FwCommand& cmd) const;
    const FirmwareControl& operator<<(const Script& script) const;

/* ------------------------- PRIVATE DECLARATIONS ------------------------- */
private:
    /**
     * The UdpSocket is used directly in WiflyControl, to send fast connectionless packets.
     */
    const UdpSocket& mUdpSock;

    /**
     * Proxy object handling the low level communication with bootloader and firmware.
     */
    const ComProxy& mProxy;

    /**
     * Sends a wifly command frame to the wifly device
     * @param command FwCommand object with the frame, which should be send
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    void FwSend(FwCommand& cmd) const;
};

class CC3200FirmwareControl : public FirmwareControl {
public:
    using FirmwareControl::FirmwareControl;

    std::string FwGetCycletime(void) const override;

    void FwGetRtc(tm& timeValue) const override;

    std::string FwGetTracebuffer(void) const override;

    uint8_t FwGetLedTyp(void) const override;

    uint16_t ExtractFwVersion(const std::string& pFilename) const override;
};

class RN171FirmwareControl : public FirmwareControl {
public:
    using FirmwareControl::FirmwareControl;

    std::string FwGetCycletime(void) const override;

    void FwGetRtc(tm& timeValue) const override;

    std::string FwGetTracebuffer(void) const override;

    uint8_t FwGetLedTyp(void) const override;

    uint16_t ExtractFwVersion(const std::string& pFilename) const override;
};
}
#endif /* #ifndef _FIRMWARECONTROL_H_ */
