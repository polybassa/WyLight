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

#ifndef _BOOTLOADERCONTROL_H_
#define _BOOTLOADERCONTROL_H_

#include <string>
#include "BlRequest.h"
#include "ComProxy.h"

namespace WyLight
{
/******************************************************************************/
/*! \file BootloaderControl.h
 * \author Nils Weiss, Patrick Bruenn
 *
 *! \cond
 * class - BootloaderControl
 * \endcond
 *
 * \brief Class to communicate with a Wifly_Light Bootloader.
 *
 *
 *******************************************************************************/
class BootloaderControl {
public:
    /**
     * Connect to a wifly device
     * @param addr ipv4 address as 32 bit value in host byte order
     * @param port number of the wifly device server in host byte order
     */
    BootloaderControl(const ComProxy& proxy);

/* ------------------------- BOOTLOADER METHODES ------------------------- */
    /**
     * Instructs the bootloader to set the autostart flag to true. This ensures
     * the bootloader will be started on the next reboot automatically.
     */
    void BlEnableAutostart(void) const;

    /**
     * Instructs the bootloader to erase the whole eeprom.
     * The wifly device has to be in bootloader mode for this command.
     */
    void BlEraseEeprom(void) const;

    /**
     * Instructs the bootloader to erase the whole flash which is not occupied
     * by the bootloader itself.
     * The wifly device has to be in bootloader mode for this command.
     */
    void BlEraseFlash(void) const;

    /**
     * Instructs the bootloader to update the wifly device with new firmware.
     * The wifly device has to be in bootloader mode for this command.
     * @param filename path to the *.hex file containing the new firmware
     */
    void BlProgramFlash(const std::string& filename) const;

    /**
     * Instructs the bootloader to create crc-16 checksums for the content of
     * the specified flash area. TODO crc values are in little endian byte order
     * The wifly device has to be in bootloader mode for this command.
     * @param out ostream for the resulting 16bit crc values
     * @param address crc generation starts from this flash address
     * @param numBytes size of the flash area for which the crc are calculated
     */
    void BlReadCrcFlash(std::ostream& out, uint32_t address, size_t numBytes) const;

    /**
     * Instructs the bootloader to read the specified memory area of the eeprom.
     * The wifly device has to be in bootloader mode for this command.
     * @param out ostream where the eeprom content is written
     * @param address start of the eeprom region to read
     * @param numBytes size of the eeprom region to read
     */
    void BlReadEeprom(std::ostream& out, uint32_t address, size_t numBytes) const;

    /**
     * Instructs the bootloader to read the specified memory area of the flash.
     * The wifly device has to be in bootloader mode for this command.
     * @param out ostream where the flash content is written
     * @param address start of the flash region to read
     * @param numBytes size of the flash region to read
     */
    void BlReadFlash(std::ostream& out, uint32_t address, size_t numBytes) const;

    /**
     * Instructs the bootloader to read the version string from the firmware memory.
     * The wifly device has to be in bootloader mode for this command.
     * @return the version string from pic flash memory
     */
    uint16_t BlReadFwVersion(void) const;

    /**
     * Instructs the bootloader to return a struct of bootloader informations
     * like bootloader version, flash and eeprom size. see \<BlInfo\> for details.
     * The wifly device has to be in bootloader mode for this command.
     */
    void BlReadInfo(BlInfo& info) const;

    /**
     * Instructs the bootloader to start the wifly device firmware.
     * The wifly device has to be in bootloader mode for this command.
     */
    void BlRunApp(void) const;

/* ------------------------- PRIVATE DECLARATIONS ------------------------- */
private:
    /**
     * Proxy object handling the low level communication with bootloader and firmware.
     */
    const ComProxy& mProxy;

    /**
     * Instructs the bootloader to erase the specified area of the flash.
     * The wifly device has to be in bootloader mode for this command.
     * @param endAddress address of the block to delete
     * @param numPages number of pages in a block
     */
    void BlEraseFlashArea(const uint32_t endAddress, const uint8_t numPages) const;

    /**
     * Send a request to the bootloader and read his response into pResponse
     * @param request reference to a bootloader requested
     * @param pResponse a buffer for the response of the bootloader
     * @param responseSize sizeof of the <pResponse> buffer in bytes
     * @param doSync if set to 'true' the uart sync is issued before data transfer default = true
     * @return the number of bytes the bootloader send back in his response
     */
    size_t BlRead(const BlRequest& request, uint8_t* pResponse, const size_t responseSize,
                  bool doSync = true) const;

    /**
     * Instructs the bootloader of the wifly device to write data to the eeprom.
     * @param address in eeprom
     * @param pBuffer containing the new data for eeprom
     * @param bufferLength number of bytes to write to eeprom
     */
    void BlWriteEeprom(uint32_t address, const uint8_t* pBuffer, size_t bufferLength) const;

    /**
     * Instructs the bootloader of the wifly device to write data to the flash.
     * @param address in flash
     * @param pBuffer containing the new data for flash
     * @param bufferLength number of bytes to write to flash
     */
    void BlWriteFlash(uint32_t address, uint8_t* pBuffer, size_t bufferLength) const;

    /**
     * Instructs the bootloader to create crc-16 checksums for the content of
     * the specified flash area. TODO crc values are in little endian byte order
     * The wifly device has to be in bootloader mode for this command.
     * @param pBuffer pointer to a buffer for the resulting 16bit crc values
     * @param address crc generation starts from this flash address
     * @param numBytes size of the flash area for which the crc are calculated
     * @return the number of bytes read (result / 2 = number of crc values)
     */
    size_t BlReadCrcFlash(uint8_t* pBuffer, uint32_t address, uint16_t numBytes) const;

    /**
     * Instructs the bootloader to read the specified memory area of the eeprom.
     * The wifly device has to be in bootloader mode for this command.
     * @param pBuffer destination for the copy of the eeprom content, should be at least \<numBytes\> wide
     * @param address start of the eeprom region to read
     * @param numBytes size of the eeprom region to read
     * @return the number of bytes read
     */
    size_t BlReadEeprom(uint8_t* pBuffer, uint32_t address, size_t numBytes) const;

    /**
     * Instructs the bootloader to read the specified memory area of the flash.
     * The wifly device has to be in bootloader mode for this command.
     * @param pBuffer destination for the copy of the flash content, should be at least \<numBytes\> wide
     * @param address start of the flash region to read
     * @param numBytes size of the flash region to read
     * @return the number of bytes read
     */
    size_t BlReadFlash(uint8_t* pBuffer, uint32_t address, size_t numBytes) const;

    /* ------------------ friendships for unittesting only ------------------- */
    friend size_t ut_WiflyControl_BlEepromWrite(void);
    friend size_t ut_WiflyControl_BlFlashWrite(void);
};
}
#endif /* #ifndef _BOOTLOADERCONTROL_H_ */
