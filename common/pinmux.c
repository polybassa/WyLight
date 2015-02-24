/*
   Copyright (C) 2014 Nils Weiss, Patrick Bruenn.

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

#include "pinmux.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom_map.h"
#include "gpio.h"
#include "prcm.h"

//*****************************************************************************
void PinMuxConfig(void)
{
    // Enable Peripheral Clocks
    MAP_PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);

    // Configure PIN_55 for UART0 UART0_TX
    MAP_PinTypeUART(PIN_55, PIN_MODE_3);

    // Configure PIN_57 for UART0 UART0_RX
    MAP_PinTypeUART(PIN_57, PIN_MODE_3);

    // Configure PIN_58 for GPIOInput
    MAP_PinTypeGPIO(PIN_58, PIN_MODE_0, false);
    MAP_GPIODirModeSet(GPIOA0_BASE, 0x8, GPIO_DIR_MODE_IN);

    MAP_PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralClkEnable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);

    // Configure PIN_64 for TIMERPWM5 GT_PWM05
    MAP_PinTypeTimer(PIN_64, PIN_MODE_3);

    // Configure PIN_01 for TIMERPWM6 GT_PWM06
    MAP_PinTypeTimer(PIN_01, PIN_MODE_3);

    // Configure PIN_02 for TIMERPWM7 GT_PWM07
    MAP_PinTypeTimer(PIN_02, PIN_MODE_3);
}