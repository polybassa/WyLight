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

byte slaveSelectPin = 13;

byte leds[] = {
  255, 255, 255,
  255,   0,   0,
    0, 255,   0,
    0,   0, 255,
  255, 255,   0,
    0, 255,   0,
    0, 255, 255,
    0,   0, 255,
  255,   0, 255,
    0, 255,   0,
};

static const uint8_t GREEN_PIN = 11;
static const uint8_t BLUE_PIN = 13;
static const uint8_t RED_PIN = 12;

int curOffset = 2 * 3;

void go4LED(int r = 0, int g = 0, int b = 0) {
#if 0
  analogWrite(RED_PIN,   leds[(curOffset + 0) % sizeof(leds)]);
  analogWrite(GREEN_PIN, leds[(curOffset + 1) % sizeof(leds)]);
  analogWrite(BLUE_PIN,  leds[(curOffset + 2) % sizeof(leds)]);
#else
  analogWrite(RED_PIN,   r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN,  b);
#endif
//  digitalWrite(slaveSelectPin, LOW);
  for (int i = 0; i < sizeof(leds); ++i) {
    //SPI.transfer(leds[(curOffset + i) % sizeof(leds)]);
  }
//  digitalWrite(slaveSelectPin, HIGH);
  curOffset += 3;
}

void setup()
{
  //pinMode(slaveSelectPin, OUTPUT);
  SPI.begin();
  
  go4LED();
  Serial1.begin(57600);
  
}

int counter = 0;
void loop()
{
  if (Serial1.available() > 2) {
    int r = Serial1.read();
    int g = Serial1.read();
    int b = Serial1.read();
    if (!(counter % 256)) {
      go4LED(r, g, b);
    }
  }
}
