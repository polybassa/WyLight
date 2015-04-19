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
#include "wifi.h"
#include "wy_firmware.h"
#include "pwm.h"
#include "BroadcastTransmitter.h"
#include "SimplelinkCustomer.h"
#include "SimplelinkServers.h"

extern char _binary_cc_fw_versionfile_txt_start;
extern char _binary_cc_fw_versionfile_txt_end;

// GLOBAL VARIABLES
const CC3200_Platform g_platform(
    std::move(
        std::string(&_binary_cc_fw_versionfile_txt_start,
                    (&_binary_cc_fw_versionfile_txt_end -
                     &_binary_cc_fw_versionfile_txt_start))));
std::vector<SimplelinkCustomer*> SimplelinkCustomer::Customers;
const BroadcastTransmitter g_broadcast;
const UdpServer g_udpserver;
const TcpServer g_tcpserver;

int main(void)
{
    Pwm_TaskInit();
    WyLightFirmware_TaskInit();

    xTaskCreate(WlanSupport_Task, (signed portCHAR*)"WlanSupport",
                Task::STACKSIZE_IN_BYTE(2048), NULL, 8, &g_WlanSupportTaskHandle);
    xTaskCreate(WyLightFirmware_Task, (signed portCHAR*)"WyLightFirmware",
                Task::STACKSIZE_IN_BYTE(2048), NULL, 7,
                &g_WyLightFirmwareTaskHandle);
    xTaskCreate(WyLightGetCommands_Task, (signed portCHAR*)"GetCommands",
                Task::STACKSIZE_IN_BYTE(2048), NULL, 6,
                &g_WyLightGetCommandsTaskHandle);
    xTaskCreate(Pwm_Task, (signed portCHAR*)"PWM",
                Task::STACKSIZE_IN_BYTE(2048), NULL, 2, &g_PwmTaskHandle);

    vTaskStartScheduler();
    while (1) {}
}
