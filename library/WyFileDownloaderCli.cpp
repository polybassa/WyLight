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

#include "WyFileDownloader.h"
#include "Endpoint.h"
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace WyLight;
using std::cin;
using std::cout;

char* getCmdOption(char** begin, char** end, const std::string& option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);

// functions to parse argument list
char* getCmdOption(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if ((itr != end) && (++itr != end))
        return *itr;
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char* argv[])
{
    if (cmdOptionExists(argv, argv + argc, "-?") || cmdOptionExists(argv, argv + argc, "-help")) {
        std::cout <<
            "Usage: \nno parameters: default usage.\n-c \"123.123.123.123\" to connect to a specific IP immediately.\n-p \"12345\" to specify a port\n-f \"firmware.bin\" to specify a file to load\n-d \"destination_name.bin\" to specify filename on target\n";
        return 0;
    }

    WyLight::Endpoint e;

    if (cmdOptionExists(argv, argv + argc, "-c")) {
        in_addr_t addr;
        inet_pton(AF_INET, getCmdOption(argv, argv + argc, "-c"), &(addr));
        /* Reverse the bytes in the binary address */
        addr = ((addr & 0xff000000) >> 24) | ((addr & 0x00ff0000) >> 8) | ((addr & 0x0000ff00) << 8) |
               ((addr & 0x000000ff) << 24);
        e = WyLight::Endpoint(addr, 2000);
    }

    if (cmdOptionExists(argv, argv + argc, "-p"))
        e = WyLight::Endpoint(e.GetIp(), std::strtoul(getCmdOption(argv, argv + argc, "-p"), NULL, 0));

    if (!cmdOptionExists(argv, argv + argc, "-f") || !cmdOptionExists(argv, argv + argc, "-d")) {
        std::cout << "No files specified. Use -? for more information\n";
        return 0;
    }
    std::string srcPath(getCmdOption(argv, argv + argc, "-f"), strlen(getCmdOption(argv, argv + argc, "-f")));
    std::string destPath(getCmdOption(argv, argv + argc, "-d"), strlen(getCmdOption(argv, argv + argc, "-d")));

    return FileDownloader(e.GetIp(), e.GetPort()).loadFile(srcPath, destPath);
}
