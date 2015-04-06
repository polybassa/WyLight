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

#ifndef _WIFLYCONTROL_H_
#define _WIFLYCONTROL_H_

#include <string>
#include "ComProxy.h"
#include "wifly_cmd.h"
#include "TelnetProxy.h"
#include "WiflyControlException.h"
#include "FwCommand.h"
#include "Script.h"
#include "ConfigControl.h"
#include "BootloaderControl.h"

namespace WyLight
{
/******************************************************************************/
/*! \file WiflyControl.h
 * \author Nils Weiss, Patrick Bruenn
 *
 *! \cond
 * class - Control
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Hardware.
 *
 *
 * The Control class allows the user to control the Wifly_Light Hardware.
 * There are three target's at the Wifly_Light Hardware.
 * - Bootloader<br>
 *           All methodes with Bl* relate to the bootloader part.
 * - Firmware<br>
 *           All methodes with Fw* relate to the firmware part.
 * - RN-171 Wifi Interface<br>
 *           All methodes witch Conf* relate to the communication module.
 *******************************************************************************/
class Control {
public:
    BootloaderControl mBootloader;
    ConfigControl mConfig;
    /**
     * string constant to address all LEDs. String representation of 0xffffffff
     */
    static const std::string LEDS_ALL;
    /**
     * Connect to a wifly device
     * @param addr ipv4 address as 32 bit value in host byte order
     * @param port number of the wifly device server in host byte order
     */
    Control(uint32_t addr, uint16_t port);

    /*
     * Send a byte sequence to ident the current software running on PIC
     * @return mode of target: BL_IDENT for Bootloader mode, FW_IDENT for Firmware mode
     * @throw FatalError if synchronisation fails
     */
    size_t GetTargetMode(void) const throw(FatalError);

/* -------------------------- FIRMWARE METHODES -------------------------- */
    /**
     * Reads the cycletimes from wifly device and stores them into the response object
     * @return a string with all recorded cycletimes from PIC firmware
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    std::string FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

    /**
     * Reads the current rtc time from the wifly device
     * @param timeValue reference to a tm object, where to store the rtc time from PIC firmware
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    void FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

    /**
     * Reads the tracebuffer from wifly device and stores the data into the response object
     * @return a string with all recorded trace messages from PIC firmware
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    std::string FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

    /**
     * Reads the firmware version currently running on the wifly device.
     * @return a string representing the version number of the PIC firmware
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    uint16_t FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

    /**
     * Reads the led typ on the spi interface. To detect WS2801 Led's, the SPI IN and OUT Pin's has to conntect together, to build a loopback.
     * @return a value representing the led typ of the WyLight modul
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    uint8_t FwGetLedTyp(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

    //TODO move this test functions to the integration test
    void FwTest(void);
    void FwStressTest(void);

    Control& operator<<(FwCommand&& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
    Control& operator<<(FwCommand& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull);
    Control& operator<<(const Script& script) throw (ConnectionTimeout, FatalError, ScriptBufferFull);

/* ------------------------- VERSION EXTRACT METHODE ------------------------- */
    /**
     * Methode to extract the firmware version from a hex file
     * @return the version string from a given hex file
     */
    uint16_t ExtractFwVersion(const std::string& pFilename) const;

/* ------------------------- PRIVATE DECLARATIONS ------------------------- */
private:
    /**
     * Sockets used for communication with wifly device.
     * A reference to the TcpSocket is provided to the aggregated subobjects.
     */
    const TcpSocket mTcpSock;

    /**
     * The UdpSocket is used directly in WiflyControl, to send fast connectionless packets.
     */
    const UdpSocket mUdpSock;

    /**
     * Proxy object handling the low level communication with bootloader and firmware.
     */
    const ComProxy mProxy;

    /**
     * Proxy object handling the communication with the wlan module for its configuration
     */
    const TelnetProxy mTelnet;

    /**
     * Sends a wifly command frame to the wifly device
     * @param command FwCommand object with the frame, which should be send
     * @throw ConnectionTimeout if response timed out
     * @throw FatalError if command code of the response doesn't match the code of the request, or too many retries failed
     * @throw ScriptBufferFull if script buffer in PIC firmware is full and request couldn't be executed
     */
    void FwSend(FwCommand& cmd) const throw (ConnectionTimeout, FatalError, ScriptBufferFull);

/* ------------------ friendships for unittesting only ------------------- */
    friend size_t ut_WiflyControl_ConfSetDefaults(void);
    friend size_t ut_WiflyControl_ConfSetWlan(void);
};
}
#endif /* #ifndef _WIFLYCONTROL_H_ */
