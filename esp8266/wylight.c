/* Very basic example to test the pwm library
 * Hook up an LED to pin14 and you should see the intensity change
 *
 * Part of esp-open-rtos
 * Copyright (C) 2015 Javier Cardona (https://github.com/jcard0na)
 * BSD Licensed as described in the file LICENSE
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pwm.h"

#include "../firmware/main_common.c"
#include "../firmware/broadcast.c"
#include "../firmware/crc.c"
#include "../firmware/eeprom_ram.c"
#include "../firmware/error.c"
#include "../firmware/ledstrip.c"
#include "../firmware/spi_pwmesp.c"
#include "../firmware/RingBuf.c"
//#include "usart.c"
#include "../firmware/CommandIO.c"
#include "../firmware/platform.c"
//#include "rtc.c"
//#include "iic.c"
#include "../firmware/ScriptCtrl.c"
#include "../firmware/trace.c"
//#include "Flash.c"
#include "../firmware/timer.c"
#include "../firmware/Version.c"

//TODO refactor this dummy functions with x86_wrapper.c
void Rtc_Init()
{}
void Rtc_Ctl(enum RTC_request req, struct rtc_time* pRtcTime) {}
uns16 Timer_PrintCycletime(uns16* pArray, const uns16 arraySize)
{
    return arraySize;
}

void UART_Init()
{
    uart_set_baud(0, 115200);
}
void UART_Send(uns8 c)
{}

void task1(void* pvParameters)
{
    run_main();
}

void Platform_ExtraInit(void)
{
    xTaskCreate(&wifi_task, "wifi_task", 256, NULL, 2, NULL);
}

void user_init(void)
{
    xTaskCreate(task1, "tsk1", 256, NULL, 2, NULL);
}
