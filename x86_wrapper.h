#ifndef _X86_WRAPPER_H_
#define _X86_WRAPPER_H_
#include <pthread.h>
#include "platform.h"
#include "ledstrip.h"

extern bit g_led_off; //X86 replacement for PORTC.0
extern uns8 g_led_status[NUM_OF_LED*3];
extern pthread_mutex_t g_led_mutex;

void* gl_start(void* unused);

#endif /* #ifndef _X86_WRAPPER_H_ */
