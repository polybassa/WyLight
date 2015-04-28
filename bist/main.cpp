/*
   Copyright (C) 2012 - 2014 Nils Weiss, Patrick Bruenn.

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

#include "cpp_overrides.h"
#include "CC3200_Platform.h"

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_Hooks.h"

//Application Includes
#include "wy_Task.h"
#include "wy_Pwm.h"
#include "SimplelinkDriver.h"
#include "trace.h"

#include "gpio.h"
#include "hw_memmap.h"

static const int __attribute__((unused)) g_DebugZones = ZONE_ERROR |
                                                        ZONE_WARNING | ZONE_INFO | ZONE_VERBOSE;

extern char _binary_bist_versionfile_txt_start;
extern char _binary_bist_versionfile_txt_end;

// GLOBAL VARIABLES
const CC3200_Platform g_platform(
    std::move(
        std::string(&_binary_bist_versionfile_txt_start,
                    (&_binary_bist_versionfile_txt_end -
                     &_binary_bist_versionfile_txt_start))));

int main(void)
{
    Task wifiTask("WIFI", 2048, 3,
                  [&](const bool& run) {
        std::array<Pwm, 3> pwms = {Pwm(Pwm::CHANNEL5), Pwm(Pwm::CHANNEL6), Pwm(Pwm::CHANNEL7)};
        for (const auto& pwm : pwms) {
            pwm.enable();
        }

        Trace(ZONE_ERROR, "Connecting!");

        SimplelinkDriver simplelinkChip("InternetBox",
                                        "F!r5tUn!ver5alCybernet!c-K!net!cUltra-M!croProgrammer",
                                        SL_SEC_TYPE_WPA);

        if (!simplelinkChip) {
            pwms[0] = 999;
            while (true) {}
        }

        GPIODirModeSet(GPIOA1_BASE, GPIO_PIN_7, GPIO_DIR_MODE_IN);         //GPIO15
        GPIODirModeSet(GPIOA2_BASE, GPIO_PIN_6, GPIO_DIR_MODE_IN);        //GPIO22
        GPIODirModeSet(GPIOA3_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);        //GPIO28

        for (const auto& pwm : pwms) {
            pwm = 0;
        }
        vTaskDelay(10);
        if ((GPIOPinRead(GPIOA1_BASE, GPIO_PIN_7) != 0) || (GPIOPinRead(GPIOA2_BASE, GPIO_PIN_6) != 0) || (GPIOPinRead(
                                                                                                               GPIOA3_BASE,
                                                                                                               GPIO_PIN_4) != 0))
        {
            pwms[0] = 999;
            while (true) {}
        }
        vTaskDelay(10);
        for (const auto& pwm : pwms) {
            pwm = 999;
        }
        vTaskDelay(10);
        if ((GPIOPinRead(GPIOA1_BASE, GPIO_PIN_7) != 1) || (GPIOPinRead(GPIOA2_BASE, GPIO_PIN_6) != 1) || (GPIOPinRead(
                                                                                                               GPIOA3_BASE,
                                                                                                               GPIO_PIN_4) != 1))
        {
            pwms[0] = 999;
            while (true) {}
        }
        vTaskDelay(10);
        Trace(ZONE_ERROR, "Connected!");
        for (const auto& pwm : pwms) {
            pwm = 0;
        }
        pwms[2] = 999;
        while (true) {}
    });

    Task initTask("INIT", Task::STACKSIZE_IN_BYTE(16), 4, [&](const bool& run) {
        Trace(ZONE_ERROR, "init!");
    });

    //initTask.run();
    wifiTask.run();

    vTaskStartScheduler();
    while (1) {}
}
