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
static const uint8_t GREEN_PIN = 11;
static const uint8_t BLUE_PIN = 13;
static const uint8_t RED_PIN = 12;
static const size_t NUM_OF_LED = 32;

byte slaveSelectPin = 13;

int leds[3 * NUM_OF_LED] {};


void leds_update() {
  analogWrite(RED_PIN,   leds[0]);
  analogWrite(GREEN_PIN, leds[1]);
  analogWrite(BLUE_PIN,  leds[2]);

//  digitalWrite(slaveSelectPin, LOW);
  for (int i = 0; i < sizeof(leds); ++i) {
    //SPI.transfer(leds[(curOffset + i) % sizeof(leds)]);
  }
//  digitalWrite(slaveSelectPin, HIGH);
}

void setup()
{
  //pinMode(slaveSelectPin, OUTPUT);
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
    if (!next) {
      leds_update();
      lastUpdate = millis();
    }
  }
}
