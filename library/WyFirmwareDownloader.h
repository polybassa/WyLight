/**
 Copyright (C) 2014 Nils Weiss, Patrick Bruenn.
 
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

#ifndef __WyFirmwareDownloader__
#define __WyFirmwareDownloader__

#include <stdint.h>
#include <string>
#include "ClientSocket.h"

namespace WyLight {
    
    class BootloaderClient : private TcpSocket {
    public:
        BootloaderClient(const uint32_t Addr, const uint16_t port) throw (ConnectionLost, FatalError);
        
        bool sendData(std::istream& inData) throw (FatalError);
    };
    
    class FirmwareDownloader {
        const uint32_t mAddr;
        const uint16_t mPort;
    
    public:
        FirmwareDownloader(const uint32_t Addr, const uint16_t port) : mAddr(Addr), mPort(port) {};
        
        int loadFile(const std::string& srcPath, const std::string& destPath) const;
        int loadFirmware(const std::string& path) const;
        int loadBootloader(const std::string &path) const;
        
    };
    
}

#endif /* defined(__WyFirmwareDownloader__) */
