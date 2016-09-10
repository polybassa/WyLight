/*
   Copyright (C) 2016 Patrick Bruenn.

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

#include <SPI.h>

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))

static const uint8_t PLLFRQ_96_MHZ_OUTPUT = _BV(PDIV1) | _BV(PDIV3);
static const uint8_t PLLFRQ_64_MHZ_TIMER4 = _BV(PLLTM1);
static const uint32_t PLL_FREQ_64MHZ = 64000000UL;
static const uint16_t TIMER4_RESOLUTION = 1023UL;
static const uint32_t PWM_FREQ = 100 * 1000UL;

static const size_t NUM_OF_LED = 32;

static int leds[3 * NUM_OF_LED] {32, 64, 128};
static uint32_t g_pwm_period;

static void write10BitReg(volatile uint8_t* reg, uint16_t value)
{
    TC4H = (value & 0x03ff) >> 8;
    *reg = (value & 0x00ff);
}

static void init_pll()
{
    static const uint8_t PLLFRQ_96_MHZ_OUTPUT = _BV(PDIV1) | _BV(PDIV3);
    static const uint8_t PLLFRQ_64_MHZ_TIMER4 = _BV(PLLTM1);

    /* select frequencies */
    PLLFRQ = PLLFRQ_96_MHZ_OUTPUT | PLLFRQ_64_MHZ_TIMER4;

    /* enable PLL and wait until unlocked */
    PLLCSR = _BV(PLLE);
    while (!(PLLCSR & _BV(PLOCK))) {}
}

static void init_timer4_pwm()
{
    uint8_t prescaler = 1;

    /* enable output compare on port A, B and D*/
    TCCR4A = _BV(PWM4A) | _BV(PWM4B);
    TCCR4C = _BV(PWM4D);

    /* enable enhanced mode with more accuracy in OCR4x registers */
    TCCR4E = _BV(ENHC4);

    /* enable phase and frequency correct PWM mode */
    TCCR4D = _BV(WGM40);

    /* calculate prescaling and period */
    g_pwm_period = PLL_FREQ_64MHZ / 2 / PWM_FREQ;
    while ((g_pwm_period >= TIMER4_RESOLUTION) && (prescaler < 0x10)) {
        g_pwm_period >>= 1;
        ++prescaler;
    }

    /* configure prescaling for timer4 */
    TCCR4B = prescaler & 0x0F;

    /* configure TOP for timer4 */
    write10BitReg(&OCR4C, g_pwm_period);
}

static void pwm_enable()
{
    DDRB |= _BV(PORTB6);
    DDRC |= _BV(PORTC7);
    DDRD |= _BV(PORTD7);
    TCCR4A |= _BV(COM4A1) | _BV(COM4B1);
    TCCR4C |= _BV(COM4D1);
}

static void pwm_disable()
{
    DDRB &= ~(_BV(PORTB6));
    DDRC &= ~(_BV(PORTC7));
    DDRD &= ~(_BV(PORTD7));
    TCCR4A &= ~(_BV(COM4A1) | _BV(COM4B1));
    TCCR4C &= ~(_BV(COM4D1));
}

static void pwm_set_duty(volatile uint8_t* reg, uint16_t duty)
{
    uint32_t dutyCycle = g_pwm_period * duty;
    dutyCycle >>= 9;
    write10BitReg(reg, dutyCycle);
}

static void leds_update()
{
    static volatile uint8_t* const reg[3] = {
        &OCR4A,
        &OCR4B,
        &OCR4D
    };
    static uint32_t last[3] = {0};

    for (size_t i = 0; i < 3; ++i) {
        if (last[i] != leds[i]) {
            last[i] = leds[i];
            pwm_set_duty(reg[i], leds[i] << 2);
        }
    }
}

void setup()
{
    init_pll();
    init_timer4_pwm();
    pwm_enable();

    SPI.begin();

    leds_update();
    Serial1.begin(230400);
}

void loop()
{
    static const unsigned long timeout = 1000;
    static auto lastUpdate = millis();
    static size_t next = 0;

    if (millis() - lastUpdate > 1000) {
        while (Serial1.available()) {
            Serial1.read();
        }
        lastUpdate = millis();
        next = 0;
    }

    if (Serial1.available() > 2) {
        leds[next++] = Serial1.read();
        leds[next++] = Serial1.read();
        leds[next++] = Serial1.read();

        next = next % ARRAY_SIZE(leds);
        if (!next)
            //leds_update();
            lastUpdate = millis();
    }
}
