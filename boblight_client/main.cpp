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

#include "WiflyControl.h"
#include <iostream>

using namespace WyLight;

int main(int argc, char* argv[])
{
    //const WyLight::Endpoint endpoint {0x7F000001, 2000};
    const WyLight::Endpoint endpoint {0xC0A8651E, 2000};

    auto ctrl = std::unique_ptr<Control>(WyLight::Control::Create(endpoint));
    auto& wylight = *(ctrl->mFirmware);

    std::cout << "Running...\n";
    for ( ; ; ) {
        std::string line;
        float rgb[3];
        //std::getline(std::cin, line);
        std::cin >> rgb[0] >> rgb[1] >> rgb[2];

        rgb[0] *= 255.0f;
        rgb[1] *= 255.0f;
        rgb[2] *= 255.0f;

        const uint32_t argb = ((uint32_t)rgb[0]) << 16 | ((uint32_t)rgb[1]) << 8 | ((uint32_t)rgb[2]);

        wylight << FwCmdSetColorDirect {argb, 0xffffffff};

        std::cout << (int)rgb[0] << ' ' << (int)rgb[1] << ' ' << (int)rgb[2] << '\n';
    }
}
