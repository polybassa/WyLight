
#include <iostream>
#include "WiflyControl.h"

namespace WyLight
{
Control* Control__GetControl(const Endpoint& endpoint)
{
    if (endpoint.GetType() == Endpoint::RN171)
        return new RN171Control(endpoint);

    if (endpoint.GetType() == Endpoint::CC3200)
        return new CC3200Control(endpoint);

    throw FatalError("Invalid Endpoint\r\n");
}
}

using namespace WyLight;

int main(int argc, char* argv[])
{
    const WyLight::Endpoint endpoint {0x7F000001, 2000};
    auto ctrl = std::unique_ptr<Control>(WyLight::Control__GetControl(endpoint));
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
