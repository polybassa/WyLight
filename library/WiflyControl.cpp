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
#include "crc.h"
#include "trace.h"
#include "MaskBuffer.h"
#include "Version.h"

#include <algorithm>
#include <sstream>
#include <cctype>
#include <stddef.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <unistd.h>
#include <memory>
#include "intelhexclass.h"
#include "WiflyColor.h"
#include <thread>
#include <chrono>

using std::cout;
using std::hex;
using std::ifstream;

namespace WyLight
{
static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR | ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

const std::string Control::LEDS_ALL {"ffffffff"};

const size_t FwCmdScript::INDENTATION_MAX;
const char FwCmdScript::INDENTATION_CHARACTER;

const std::string FwCmdSetFade::TOKEN("fade");
const std::string FwCmdSetGradient::TOKEN("gradient");
const std::string FwCmdLoopOn::TOKEN("loop");
const std::string FwCmdLoopOff::TOKEN("loop_off");
const std::string FwCmdWait::TOKEN("wait");

Control::Control(uint32_t addr, uint16_t port) : mConfig(mTelnet),
    mTcpSock(addr, port),
    mUdpSock(addr, port, false, 0),
    mProxy(mTcpSock),
    mTelnet(mTcpSock)
{}

size_t Control::GetTargetMode(void) const throw(FatalError)
{
    return mProxy.SyncWithTarget();
}

/** ------------------------- BOOTLOADER METHODES ------------------------- **/
void Control::BlEnableAutostart(void) const throw(ConnectionTimeout, FatalError)
{
    static const uint8_t value = 0xff;
    try {
        BlWriteEeprom((uint32_t)BL_AUTOSTART_ADDRESS, &value, sizeof(value));
    } catch (InvalidParameter& e) {
        throw new FatalError(std::string(e.what()) + "\n Internal failure in BlWriteEeprom(BL_AUTOSTART_ADDRESS....)");
    } catch (std::exception& e) {
        throw e;
    }
}

void Control::BlEraseEeprom(void) const throw(ConnectionTimeout, FatalError)
{
    uint8_t buffer[EEPROM_SIZE];
    std::fill_n(buffer, EEPROM_SIZE, 0xff);
    BlWriteEeprom((uint32_t)0, buffer, sizeof(buffer));
}

void Control::BlEraseFlash(void) const throw(ConnectionTimeout, FatalError)
{
    BlInfo info;
    BlReadInfo(info);

    const unsigned int firstAddress = info.GetAddress() - 1;
    unsigned int address = firstAddress;

    while (address > FLASH_ERASE_BLOCKSIZE * FLASH_ERASE_BLOCKS) {
        BlEraseFlashArea(address, FLASH_ERASE_BLOCKS);
        address -= FLASH_ERASE_BLOCKSIZE * FLASH_ERASE_BLOCKS;
    }
    /* now we erased everything until a part of the flash smaller than FLASH_ERASE_BLOCKS * FLASH_ERASE_BLOCKSIZE
     * so we set our startaddress at the beginning of this block and erase */
    BlEraseFlashArea(FLASH_ERASE_BLOCKS * FLASH_ERASE_BLOCKSIZE - 1, FLASH_ERASE_BLOCKS);
}

void Control::BlEraseFlashArea(const uint32_t endAddress, const uint8_t numPages) const throw(ConnectionTimeout,
                                                                                              FatalError)
{
    unsigned char response;
    BlFlashEraseRequest request(endAddress, numPages);

    // always sync for flash erase
    BlRead(request, &response, sizeof(response), true);

    // we expect only one byte as response, the command code 0x03
    if (0x03 != response)
        throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
}

size_t Control::BlRead(const BlRequest& req, unsigned char* pResponse, const size_t responseSize,
                       bool doSync) const throw(ConnectionTimeout, FatalError)
{
    unsigned char buffer[BL_MAX_MESSAGE_LENGTH];
    size_t bytesReceived = mProxy.Send(req, buffer, sizeof(buffer), doSync);
    Trace(ZONE_INFO, " %zd:%ld \n", bytesReceived, sizeof(BlInfo));
    TraceBuffer(ZONE_VERBOSE, (uint8_t*)&buffer[0], bytesReceived, "0x%02x, ", "Message: ");
    if (responseSize != bytesReceived)
        throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": Too many retries");
    memcpy(pResponse, buffer, responseSize);
    return responseSize;
}

void Control::BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout,
                                                                                                FatalError,
                                                                                                InvalidParameter)
{
    out.flags(std::ios::right | std::ios::hex | std::ios::showbase);
    uint8_t buffer[5096];
    size_t bytesRead = BlReadCrcFlash(buffer, address, numBytes);
    for (size_t i = 0; i < bytesRead; i++) {
        out << buffer[i];
    }
}

size_t Control::BlReadCrcFlash(unsigned char* pBuffer, unsigned int address,
                               uint16_t numBlocks) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
    if (numBlocks * FLASH_ERASE_BLOCKSIZE + address > FLASH_SIZE)
        throw InvalidParameter(std::string(__FILE__) + ':' + __FUNCTION__ + ": address or numBlocks out of range");

    size_t bytesRead;
    size_t sumBytesRead = 0;
    while (numBlocks > FLASH_CRC_BLOCKSIZE) {
        BlFlashCrc16Request readRequest(address, FLASH_CRC_BLOCKSIZE);
        bytesRead = BlRead(readRequest, pBuffer, FLASH_CRC_BLOCKSIZE * 2);
        sumBytesRead += bytesRead;
        address += (FLASH_CRC_BLOCKSIZE * FLASH_ERASE_BLOCKSIZE);
        numBlocks -= FLASH_CRC_BLOCKSIZE;
        pBuffer += FLASH_CRC_BLOCKSIZE * 2;
    }

    BlFlashCrc16Request readRequest(address, numBlocks);
    bytesRead = BlRead(readRequest, pBuffer, numBlocks * 2);
    sumBytesRead += bytesRead;
    return sumBytesRead;
}

void Control::BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout,
                                                                                              FatalError,
                                                                                              InvalidParameter)
{
    uint8_t buffer[EEPROM_SIZE];
    size_t bytesRead = BlReadEeprom(buffer, address, numBytes);
    out.flags(std::ios::right | std::ios::hex | std::ios::showbase);
    for (size_t i = 0; i < bytesRead; i++) {
        out.put(buffer[i]);
    }
}

size_t Control::BlReadEeprom(uint8_t* pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout,
                                                                                               FatalError,
                                                                                               InvalidParameter)
{
    if (numBytes + address > EEPROM_SIZE)
        throw InvalidParameter(std::string(__FILE__) + ':' + __FUNCTION__ + ": address or numBytes out of range");

    size_t bytesRead;
    size_t sumBytesRead = 0;
    BlEepromReadRequest readRequest;
    while (numBytes > EEPROM_READ_BLOCKSIZE) {
        readRequest.SetAddressNumBytes(address, EEPROM_READ_BLOCKSIZE);
        bytesRead = BlRead(readRequest, pBuffer, EEPROM_READ_BLOCKSIZE);
        sumBytesRead += bytesRead;
        address += EEPROM_READ_BLOCKSIZE;
        numBytes -= EEPROM_READ_BLOCKSIZE;
        pBuffer += EEPROM_READ_BLOCKSIZE;
    }
    readRequest.SetAddressNumBytes(address, numBytes);
    bytesRead = BlRead(readRequest, pBuffer, numBytes);
    sumBytesRead += bytesRead;
    return sumBytesRead;
}

void Control::BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const throw (ConnectionTimeout,
                                                                                             FatalError,
                                                                                             InvalidParameter)
{
    out.flags(std::ios::right | std::ios::hex | std::ios::showbase);
    uint8_t buffer[FLASH_SIZE];
    size_t bytesRead = BlReadFlash(buffer, address, numBytes);
    for (size_t i = 0; i < bytesRead; i++) {
        out << buffer[i];
    }
}

size_t Control::BlReadFlash(uint8_t* pBuffer, uint32_t address, size_t numBytes) const throw (ConnectionTimeout,
                                                                                              FatalError,
                                                                                              InvalidParameter)
{
    if (numBytes + address > FLASH_SIZE)
        throw InvalidParameter(std::string(__FILE__) + ':' + __FUNCTION__ + ": address or numBytes out of range");

    size_t bytesRead;
    size_t sumBytesRead = 0;
    BlFlashReadRequest readRequest;
    while (numBytes > FLASH_READ_BLOCKSIZE) {
        readRequest.SetAddressNumBytes(address, FLASH_READ_BLOCKSIZE);
        bytesRead = BlRead(readRequest, pBuffer, FLASH_READ_BLOCKSIZE);
        sumBytesRead += bytesRead;
        address += FLASH_READ_BLOCKSIZE;
        numBytes -= FLASH_READ_BLOCKSIZE;
        pBuffer += FLASH_READ_BLOCKSIZE;
    }

    readRequest.SetAddressNumBytes(address, numBytes);
    bytesRead = BlRead(readRequest, pBuffer, numBytes);
    sumBytesRead += bytesRead;
    return sumBytesRead;
}

uint16_t Control::BlReadFwVersion(void) const throw (ConnectionTimeout, FatalError)
{
    BlInfo info;
    BlReadInfo(info);

    union versionUnion {
        uint16_t version;
        uint8_t bytes[2];
    }
    versUnion;

    BlReadFlash(versUnion.bytes, VERSION_STRING_ORIGIN, sizeof(versUnion));

    return versUnion.version > 300 ? 0 : versUnion.version;
}

void Control::BlReadInfo(BlInfo& blInfo) const throw (ConnectionTimeout, FatalError)
{
    BlInfoRequest request;
    BlRead(request, reinterpret_cast<unsigned char*>(&blInfo), sizeof(BlInfo));
}

void Control::BlWriteFlash(unsigned int address, unsigned char* pBuffer,
                           size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
    if (bufferLength + address > FLASH_SIZE)
        throw InvalidParameter("Address + bufferLength out of flash range\n");

    BlFlashWriteRequest request;
    unsigned char response;

    size_t bytesLeft = bufferLength;
    while (bytesLeft > FLASH_WRITE_BLOCKSIZE) {
        request.SetData(address, pBuffer, FLASH_WRITE_BLOCKSIZE);

        // we expect only the 0x04 command byte as response
        BlRead(request, &response, sizeof(response));
        if (response != 0x04)
            throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
        address += FLASH_WRITE_BLOCKSIZE;
        pBuffer += FLASH_WRITE_BLOCKSIZE;
        bytesLeft -= FLASH_WRITE_BLOCKSIZE;
    }

    request.SetData(address, pBuffer, bytesLeft);
    BlRead(request, &response, sizeof(response));
    if (response != 0x04)
        throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
}

void Control::BlWriteEeprom(unsigned int address, const uint8_t* pBuffer,
                            size_t bufferLength) const throw (ConnectionTimeout, FatalError, InvalidParameter)
{
    if (bufferLength + address > EEPROM_SIZE)
        throw InvalidParameter("Address + bufferLength out of eeprom range\n");

    BlEepromWriteRequest request;
    unsigned char response;

    size_t bytesLeft = bufferLength;
    while (bytesLeft > EEPROM_WRITE_BLOCKSIZE) {
        request.SetData(address, pBuffer, EEPROM_WRITE_BLOCKSIZE);

        // we expect only the 0x06 command byte as response
        BlRead(request, &response, sizeof(response));
        if (response != 0x06)
            throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");

        address += EEPROM_WRITE_BLOCKSIZE;
        pBuffer += EEPROM_WRITE_BLOCKSIZE;
        bytesLeft -= EEPROM_WRITE_BLOCKSIZE;
    }

    request.SetData(address, pBuffer, bytesLeft);
    BlRead(request, &response, sizeof(response));
    if (response != 0x06)
        throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
}

void Control::BlProgramFlash(const std::string& pFilename) const throw (ConnectionTimeout, FatalError)
{
    std::ifstream hexFile;
    hexFile.open(const_cast<char*>(pFilename.c_str()), ifstream::in);

    if (!hexFile.good())
        throw FatalError("opening '" + pFilename + "' failed");

    intelhex hexConverter;
    hexFile >> hexConverter;

    BlInfo info;
    BlReadInfo(info);

    /*Check if last address of programmcode is not in the bootblock */
    unsigned long endAddress;
    if (!hexConverter.endAddress(&endAddress))
        throw FatalError("can't read endAddress from hexConverter \n");

    if (endAddress >= (unsigned long)(info.GetAddress()))
        throw FatalError("endaddress of program code is in bootloader area of the target device flash \n");

    unsigned short word1, word2;
    unsigned int bootAddress;
    unsigned char resetVector[4];
    unsigned char appVector[4];

    /* Calculate the resetVector */
    bootAddress = (info.GetAddress() + 2) / 2;

    word1 = 0xEF00 | (bootAddress & 0xff);
    word2 = 0xF000 | ((bootAddress >> 8) & 0x0FFF);

    resetVector[0] = (unsigned char)word1;
    resetVector[1] = (unsigned char)(word1 >> 8);
    resetVector[2] = (unsigned char)word2;
    resetVector[3] = (unsigned char)(word2 >> 8);

    /*Put AppVektor from the beginning of hexfile at the startaddress of the bootloader */
    hexConverter.begin();
    if (hexConverter.currentAddress() != 0)
        throw FatalError("program code does not start at address 0x0000 \n");

    for (unsigned int i = 0; i < sizeof(appVector); i++) {
        if (!hexConverter.getData(&appVector[i], (unsigned long)i))

            throw FatalError("can not read data at address " + std::to_string(hexConverter.currentAddress()));
    }

    BlEnableAutostart();
    BlEraseFlash();

    unsigned int writeAddress = 0;
    unsigned char flashBuffer[FLASH_SIZE];
    unsigned char nextByte;

    if (endAddress > FLASH_SIZE)
        throw FatalError("endaddress of program code is outside the target device flash\n");

    /* Write the resetVector to temporary flashBuffer*/
    memcpy(&flashBuffer[0], &resetVector[0], sizeof(resetVector));

    writeAddress += sizeof(resetVector);

    for ( ; writeAddress <= (unsigned int)endAddress; writeAddress++) {
        if (hexConverter.getData(&nextByte, writeAddress))
            flashBuffer[writeAddress] = nextByte;
        else
            flashBuffer[writeAddress] = 0xff;
    }
    BlWriteFlash(0, &flashBuffer[0], (size_t)endAddress + 1);

    /* we always have to write a FLASH_WRITE Block when we wanna write to device flash,
     * so we have to pack the appVector at the end of a Block of data */

    unsigned char appVecBuf[FLASH_WRITE_BLOCKSIZE];

    for (int i = 0; i < FLASH_WRITE_BLOCKSIZE; i++) {
        appVecBuf[i] = 0xff;
    }

    memcpy(&appVecBuf[sizeof(appVecBuf) - sizeof(appVector)], &appVector[0], sizeof(appVector));
    BlWriteFlash(info.GetAddress() - FLASH_WRITE_BLOCKSIZE, &appVecBuf[0], FLASH_WRITE_BLOCKSIZE);
}

void Control::BlRunApp(void) const throw (ConnectionTimeout, FatalError)
{
    BlRunAppRequest request;
    unsigned char buffer[32];
    size_t bytesRead = BlRead(request, &buffer[0], 6);

    Trace(ZONE_VERBOSE, "We got %zd bytes response.\n", bytesRead);
    if (4 <= bytesRead) {
        struct response_frame* pResponse = (response_frame*)&buffer[0];
        if (pResponse->cmd == FW_STARTED) return;
        throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong command code");
    }
    throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": response of wrong length");
}

std::string Control::FwGetCycletime(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    FwCmdGetCycletime cmd;
    *this << cmd;
    return cmd.mResponse.ToString();
}

void Control::FwGetRtc(tm& timeValue) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    FwCmdGetRtc cmd;
    *this << cmd;
    timeValue = cmd.mResponse.GetRealTime();
}

std::string Control::FwGetTracebuffer(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    FwCmdGetTracebuffer cmd;
    *this << cmd;
    return cmd.mResponse.ToString();
}

uint16_t Control::FwGetVersion(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    FwCmdGetVersion cmd;
    *this << cmd;
    return cmd.mResponse.getVersion();
}

uint8_t Control::FwGetLedTyp(void) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    FwCmdGetLedTyp cmd;
    *this << cmd;
    return cmd.mResponse.getLedTyp();
}

void Control::FwSend(FwCommand& cmd) const throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    if (cmd.IsResponseRequired()) {
        response_frame buffer;
        size_t numCrcRetries = 8;
        do {
            const size_t bytesRead = mProxy.Send(cmd, &buffer, sizeof(buffer));
            TraceBuffer(ZONE_VERBOSE,
                        (uint8_t*)&buffer,
                        bytesRead,
                        "%02x ",
                        "We got %zd bytes response.\nMessage: ",
                        bytesRead);

            if (cmd.GetResponse().Init(buffer, bytesRead))
                return;
        } while (0 < --numCrcRetries);
        throw FatalError(std::string(__FILE__) + ':' + __FUNCTION__ + ": Too many retries");
    } else {
        MaskBuffer maskBuffer {BL_MAX_MESSAGE_LENGTH};
        maskBuffer.Mask(cmd.GetData(), cmd.GetData() + cmd.GetSize(), false);
        if (maskBuffer.Size() != mUdpSock.Send(maskBuffer.Data(), maskBuffer.Size()))
            throw FatalError("mUdpSock.Send() failed");
    }
}

void Control::FwStressTest(void)
{
    *this << FwCmdClearScript {};

    uns8 ledArr[NUM_OF_LED * 3];
    uns8 color = 0;
    while (true) {
        color++;
        std::fill_n(ledArr, sizeof(ledArr), color);
        *this << FwCmdSetColorDirect {ledArr, sizeof(ledArr)};
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
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

Control& Control::operator<<(FwCommand&& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    this->FwSend(cmd);
    return *this;
}

Control& Control::operator<<(FwCommand& cmd) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    this->FwSend(cmd);
    return *this;
}

Control& Control::operator<<(const Script& script) throw (ConnectionTimeout, FatalError, ScriptBufferFull)
{
    for (auto it = script.begin(); it != script.end(); ++it) {
        *this << **it;
    }
    return *this;
}

void Control::FwTest(void)
{
#if 0
    static const timespec sleepTime {0, 50000000};
    uint32_t color = 0xff;
    for (size_t i = 0; i < 100; ++i) {
        color = ((color & 0xff) << 24) | (color >> 8);
        *this << FwCmdSetFade {color};
        nanosleep(&sleepTime, NULL);
    }
#else

    *this << FwCmdClearScript() << FwCmdSetFade(WiflyColor::BLACK, 2);

/*		WyLight::Script testScript("test.script");

                testScript.emplace_front(FwCmdLoopOn());
                testScript.emplace_back(FwCmdSetFade(WiflyColor::GREEN, 2000));
                testScript.emplace_back(FwCmdSetFade(WiflyColor::RED, 2000));
                testScript.emplace_back(FwCmdSetFade(WiflyColor::BLUE, 2000));
                testScript.emplace_back(FwCmdLoopOff(5));

 * this << testScript;
 */		/*
                uint32_t bitMask = 0x01;
                for(unsigned int i = 0; i < NUM_OF_LED; i++)
                {
                        ledColor.red((uint8_t)((0xff / NUM_OF_LED) * i));
                        ledColor.green((uint8_t)((0xff / NUM_OF_LED) * i));
                        ledColor.blue(0xff);
                        FwSetFade(setFadeResp, bitMask, ledColor.argb(), 20000 + i * 200, true);
                        bitMask = bitMask << 1;
                }

                testScript.push_back((setWaitResp, 30000);
                FwSetFade(setFadeResp, WiflyColor::GREEN,2000, 0xFFFFFFFF, false);
                FwSetFade(setFadeResp, WiflyColor::RED,  2000, 0x000000FF, true);
                FwSetFade(setFadeResp, WiflyColor::GREEN,2000, 0x0000FF00, true);
                FwSetFade(setFadeResp, WiflyColor::BLUE, 2000, 0x00FF0000, true);
                FwSetFade(setFadeResp, WiflyColor::WHITE,2000, 0xFF000000, false);
                FwSetWait(setWaitResp, 2000);
                FwSetFade(setFadeResp, WiflyColor::BLACK,2000, 0xFFFFFFFF, false);
                FwLoopOff(loopOffResp, 0);*/
#endif
}
} /* namespace WyLight */
