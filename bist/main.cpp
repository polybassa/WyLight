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
#include "SimplelinkDriver.h"

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
    SimplelinkDriver simplelinkChip("TESTSSID", "TESTPWD", SL_SEC_TYPE_OPEN);
    if (!simplelinkChip) {
        // ERROR
    }

    vTaskStartScheduler();
    while (1) {}
}
