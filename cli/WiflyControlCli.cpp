/**
                Copyright (C) 2012 - 2015 Nils Weiss, Patrick Bruenn.

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

#include "BroadcastReceiver.h"
#include "WiflyControlCli.h"
#include "WiflyControlCmd.h"
#include "WiflyControlException.h"
#include "StartupManager.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace WyLight;
using std::cin;
using std::cout;

void newRemoteCallback(const size_t index, const WyLight::Endpoint& newEndpoint);
char* getCmdOption(char** begin, char** end, const std::string& option);
bool cmdOptionExists(char** begin, char** end, const std::string& option);

WiflyControlCli::WiflyControlCli(const WyLight::Endpoint& endpoint) : mRunning(true)
{
    mControl = std::unique_ptr<WyLight::Control>(WyLight::Control::Create(endpoint));
    cout << "Connecting to " << std::hex << endpoint.GetIp() << ':' << endpoint.GetPort() << std::endl;
}

void WiflyControlCli::Run(void)
{
    std::shared_ptr<const WiflyControlCmd> pCmd;
    ShowHelp();
    string nextCmd;
    while (mRunning) {
        cout << "WiflyControlCli: ";
        cin >> nextCmd;

        if ("exit" == nextCmd) {
            return;
        } else if ("?" == nextCmd) {
            ShowHelp();
        } else {
            pCmd = WiflyControlCmdBuilder::GetCmd(nextCmd);
            if (NULL != pCmd)
                pCmd->Run(*mControl);
        }
    }
}

void WiflyControlCli::ShowHelp(void) const
{
    cout << "Command reference:" << endl;
    cout << "'?' - this help" << endl;
    cout << "'exit' - terminate cli" << endl;

    size_t i = 0;
    std::shared_ptr<const WiflyControlCmd> pCmd = WiflyControlCmdBuilder::GetCmd(i++);
    while (pCmd != NULL) {
        cout << *pCmd << endl;
        pCmd = WiflyControlCmdBuilder::GetCmd(i++);
    }
}

WyLight::Control& WiflyControlCli::getControl()
{
    return *mControl;
}

void newRemoteCallback(const size_t index, const WyLight::Endpoint& newEndpoint)
{
    std::cout << "New: " << index << ':' << newEndpoint << '\n';
}

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
            "Usage: \nno parameters: default usage.\n-c \"123.123.123.123\" to connect to a specific IP immediately.\n\n";
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
    } else {
        WyLight::BroadcastReceiver receiver(55555, "recent.txt", newRemoteCallback);
        std::thread t(std::ref(receiver));

        // wait for user input
        size_t selection;
        do {
            std::cin >> selection;
        } while (selection >= receiver.NumRemotes());

        receiver.Stop();
        t.join();

        e = receiver.GetEndpoint(selection);
    }

    WiflyControlCli cli(e);

    cli.Run();
}
