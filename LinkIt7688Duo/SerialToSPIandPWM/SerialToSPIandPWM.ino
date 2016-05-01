/*
 * Firmata is a generic protocol for communicating with microcontrollers
 * from software on a host computer. It is intended to work with
 * any host computer software package.
 *
 * To download a host software package, please clink on the following link
 * to open the download page in your default browser.
 *
 * http://firmata.org/wiki/Download
 */

/* Supports as many digital inputs and outputs as possible.
 *
 * This example code is in the public domain.
 */
#include <SPI.h>

#define ARRAY_SIZE(X) (sizeof(X) / sizeof(X[0]))
static const uint8_t GREEN_PIN = 11;
static const uint8_t BLUE_PIN = 13;
static const uint8_t RED_PIN = 12;
static const size_t NUM_OF_LED = 32;

byte slaveSelectPin = 13;

int leds[3 * NUM_OF_LED] {};


void go4LED() {
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
  
  go4LED();
  Serial1.begin(230400);
  
}

void loop()
{
  static size_t next = 0;

  if (Serial1.available() > 2) {
    leds[next++] = Serial1.read();
    leds[next++] = Serial1.read();
    leds[next++] = Serial1.read();

    next = next % ARRAY_SIZE(leds);
    if (!next) {
      go4LED();
    }
  }
}
