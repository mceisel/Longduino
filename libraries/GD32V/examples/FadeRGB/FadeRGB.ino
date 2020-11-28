/*
  GD32V analogWrite() test

  This sketch fades the RGB LED up and down on digital pins PC13, PA1 and PA2.
  This sketch was written for the Longan Nano, and will only work on other GD32V boards.

  The circuit:
  - LEDs attached on pins PC13, PA1 and PA2 to ground (already builtin on Longan Nano).

  created 8 Feb 2009
  by Tom Igoe
  modified 28 Nov 2020
  by SCP

  This example code is in the public domain.

  Based on:
  http://www.arduino.cc/en/Tutorial/AnalogWriteMega
*/

pin_size_t ledPins[] = { LED_RED, LED_GREEN, LED_BLUE };

// These constants won't change. They're used to give names to the pins used:
const int lowestPin = 0;
const int highestPin = 2;


void doWrite(pin_size_t pinNumber, int value) {
  if (value == 255) {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, HIGH);
  } else if (value == 0)  {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  } else if (analogWritable(pinNumber)) {
    pinMode(pinNumber, OUTPUT_ANALOG);
    analogWrite(pinNumber, value);
  } else {
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
  }
}

void doRGB(int mask, int brightness)
{
  int r, b, g;
  r = (mask & 4) ? brightness : 255;
  g = (mask & 2) ? brightness : 255;
  b = (mask & 1) ? brightness : 255;
  doWrite(ledPins[0], r);
  doWrite(ledPins[1], g);
  doWrite(ledPins[2], b);
}

void setup() {
  // set LED pins as outputs:
  for (int thisPin = lowestPin; thisPin <= highestPin; thisPin++) {
    if (analogWritable(ledPins[thisPin])) {
      /* only available on pins with timer/PWM */
      pinMode(ledPins[thisPin], OUTPUT_ANALOG);
    } else {
      /* fallback to digital output */
      pinMode(ledPins[thisPin], OUTPUT);
    }
  }
}

void loop() {
  // iterate over the pins:
  for (int mask = 0; mask <= 0x7; mask++) {
    // fade the LED on thisPin from off to brightest:
    for (int brightness = 0; brightness <= 255; brightness++) {
      doRGB(mask, brightness);
      delay(4);
    }
    // fade the LED on thisPin from brightest to off:
    for (int brightness = 255; brightness >= 0; brightness--) {
      doRGB(mask, brightness);
      delay(4);
    }
    // pause between LED states:
    delay(100);
  }
}
