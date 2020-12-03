#include <Arduino.h>
#include <Ticker.h>

#define LED_PIN LED_BUILTIN

Ticker tickerSetHigh(TIMERID1);
Ticker tickerSetLow(TIMERID2);

void setPin(PinStatus state) {
  digitalWrite(LED_PIN, state);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  // every 25 ms, call setPin(0) 
  tickerSetLow.attach_ms(25, setPin, LOW);
  
  // every 26 ms, call setPin(1)
  tickerSetHigh.attach_ms(26, setPin, HIGH);
}

void loop() {

}
