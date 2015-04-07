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

#include "WyFileDownloader.h"
#include "bootloader.h"
#include "ClientSocket.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "timeval.h"
#include <openssl/sha.h>
#include <memory>

namespace WyLight
{
BootloaderClient::BootloaderClient(const uint32_t Addr, const uint16_t port) : TcpSocket(Addr, port)
{
    uint8_t buffer[sizeof(WELCOME_RESPONSE) + 16];
    timeval timeout = {0, 500000};
    size_t bytesReceived = this->Recv(buffer, sizeof(buffer), &timeout);
    if (bytesReceived < sizeof(WELCOME_RESPONSE))
        throw FatalError("No welcome message received!\r\n");

    uint32_t nlBlVersion;
    memcpy(&nlBlVersion, buffer, std::min(sizeof(buffer), sizeof(nlBlVersion)));
    uint32_t blVersion = ntohl(nlBlVersion);
    if (blVersion != BOOTLOADER_VERSION)
        throw FatalError("Wrong bootloader version on target! \r\n");

    const char welcomeMessage[] = WELCOME_RESPONSE;
    memset(buffer, 0, sizeof(blVersion));
    if (memcmp(buffer, welcomeMessage, std::min(sizeof(welcomeMessage), sizeof(buffer))))
        throw FatalError("Wrong welcome message received! \r\n");
}

int BootloaderClient::sendData(std::istream& inData)
{
    uint8_t buffer[256];
    inData.seekg(0, std::ios::end);
    size_t size = inData.tellg();
    inData.seekg(0, std::ios::beg);
    while (inData.tellg() < size) {
        inData.read((char*)buffer, sizeof(buffer));
        if (inData.gcount() != this->Send(buffer, inData.gcount()))
            throw FatalError("Error during send! \r\n");
    }
    timeval timeout = {2, 0};
    buffer[0] = 0;
    this->Recv(buffer, sizeof(buffer), &timeout);
    if (buffer[0] == DONE_RESPONSE)
        return 0;
    return -1;
}

int FileDownloader::loadFirmware(const std::string& path) const
{
    return loadFile(path, FW_FILENAME);
}

int FileDownloader::loadBootloader(const std::string& path) const
{
    return loadFile(path, BL_FILENAME);
}

int FileDownloader::loadFile(const std::string& srcPath, const std::string& destPath) const
{
    std::string tempDestPath(destPath);
    tempDestPath.resize(FILENAME_SIZE, ' ');

    std::stringstream dataStream(std::stringstream::binary | std::stringstream::in | std::stringstream::out);
    dataStream << tempDestPath;

    std::fstream fsrc(srcPath, std::stringstream::binary | std::stringstream::in);
    if (!fsrc) return -1;

    fsrc.seekg(0, fsrc.end);
    const size_t size = fsrc.tellg();
    fsrc.seekg(0, fsrc.beg);

    std::unique_ptr<char[]> buffer(new char[size]());
    fsrc.read((char*)buffer.get(), size);
    fsrc.close();

    dataStream.write(buffer.get(), size);

    std::string hashPath = srcPath.substr(0, srcPath.find_last_of('.')) + ".sha";
    std::fstream fhash(hashPath, std::stringstream::binary | std::stringstream::in);

    unsigned char hash[SHA256_DIGEST_LENGTH];

    if (fhash) {
        fhash.read((char*)hash, sizeof(hash));
        fhash.close();
    } else {
        SHA256((const unsigned char*)buffer.get(), size, hash);
    }
    dataStream.write((const char*)hash, sizeof(hash));

    return BootloaderClient(mAddr, mPort).sendData(dataStream);
}
}
