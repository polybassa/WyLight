/*
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

#include <stdint.h>
#include "hw_types.h"
#include "hw_memmap.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "../../driverlib/timer.h"
#include "prcm.h"

//Common interface includes
#include "pwm.h"

//Application Includes
#include "osi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//
// GLOBAL VARIABLES -- Start
//
static xTaskHandle g_PwmTaskHandle;
static xQueueHandle g_PwmMessageQ;

OsiTaskHandle PwmTaskHandle = &g_PwmTaskHandle;
OsiMsgQ_t PwmMessageQ = &g_PwmMessageQ;
//
// GLOBAL VARIABLES -- End
//

//
// The PWM works based on the following settings:
//     Timer reload interval -> determines the time period of one cycle
//     Timer match value -> determines the duty cycle
//                          range [0, timer reload interval]
// The computation of the timer reload interval and dutycycle granularity
// is as described below:
// Timer tick frequency = 80 Mhz = 80000000 cycles/sec
// For a time period of 0.5 ms,
//      Timer reload interval = 80000000/2000 = 40000 cycles
// To support steps of duty cycle update from [0, 255]
//      duty cycle granularity = ceil(40000/255) = 157
// Based on duty cycle granularity,
//      New Timer reload interval = 255*157 = 40035
//      New time period = 0.5004375 ms
//      Timer match value = (update[0, 255] * duty cycle granularity)
//
#define TIMER_INTERVAL_RELOAD   40036 /* =(255*157) */
#define DUTYCYCLE_GRANULARITY   157

void UpdateDutyCycle(unsigned long ulBase, unsigned long ulTimer, unsigned char ucLevel) {
	MAP_TimerMatchSet(ulBase, ulTimer, (ucLevel * DUTYCYCLE_GRANULARITY));
}

void SetupTimerPWMMode(unsigned long ulBase, unsigned long ulTimer, unsigned long ulConfig, unsigned char ucInvert) {

	// Set GPT - Configured Timer in PWM mode.
	MAP_TimerConfigure(ulBase, ulConfig);
	MAP_TimerPrescaleSet(ulBase, ulTimer, 0);

	// Inverting the timer output if required
	MAP_TimerControlLevel(ulBase, ulTimer, ucInvert);

	// Load value set to ~0.5 ms time period
	MAP_TimerLoadSet(ulBase, ulTimer, TIMER_INTERVAL_RELOAD);

	// Match value set so as to output level 0
	MAP_TimerMatchSet(ulBase, ulTimer, TIMER_INTERVAL_RELOAD);
}

void Pwm_TaskInit(void) {

	// Initialization of timers to generate PWM output
	MAP_PRCMPeripheralClkEnable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
	MAP_PRCMPeripheralClkEnable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);

	// TIMERA2 (TIMER B) as RED of RGB light. GPIO 9 --> PWM_5
	SetupTimerPWMMode(TIMERA2_BASE, TIMER_B, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM), 1);

	// TIMERA3 (TIMER B) as YELLOW of RGB light. GPIO 10 --> PWM_6
	SetupTimerPWMMode(TIMERA3_BASE, TIMER_A, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM), 1);
	//
	// TIMERA3 (TIMER A) as GREEN of RGB light. GPIO 11 --> PWM_7
	SetupTimerPWMMode(TIMERA3_BASE, TIMER_B, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM), 1);

	MAP_TimerEnable(TIMERA2_BASE, TIMER_B);
	MAP_TimerEnable(TIMERA3_BASE, TIMER_A);
	MAP_TimerEnable(TIMERA3_BASE, TIMER_B);

	osi_MsgQCreate(&g_PwmMessageQ, "PwmMsgQ", 3, 6);
}

void DeInitPWMModules() {
	// Disable the peripherals
	MAP_TimerDisable(TIMERA2_BASE, TIMER_B);
	MAP_TimerDisable(TIMERA3_BASE, TIMER_A);
	MAP_TimerDisable(TIMERA3_BASE, TIMER_B);
	MAP_PRCMPeripheralClkDisable(PRCM_TIMERA2, PRCM_RUN_MODE_CLK);
	MAP_PRCMPeripheralClkDisable(PRCM_TIMERA3, PRCM_RUN_MODE_CLK);
}

void Pwm_Task(void *pvParameters) {
	uint8_t buffer[3];

	for (;;) {
		osi_MsgQRead(&g_PwmMessageQ, buffer, OSI_WAIT_FOREVER);

		UpdateDutyCycle(TIMERA2_BASE, TIMER_B, buffer[1]);
		UpdateDutyCycle(TIMERA3_BASE, TIMER_B, buffer[2]);
		UpdateDutyCycle(TIMERA3_BASE, TIMER_A, buffer[0]);
	}
}

