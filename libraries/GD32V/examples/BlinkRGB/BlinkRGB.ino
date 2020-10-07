/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  Longduinos have an on-board RGB LED, it is attached to digital pins 13, 1 and 2.
  LED_RED, LED_GREEN and LED_BLUE is set to the correct LED pin independent of
  which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital LED pins as an output.
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BLUE, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED_RED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_RED, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED_GREEN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_GREEN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED_BLUE, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
}
