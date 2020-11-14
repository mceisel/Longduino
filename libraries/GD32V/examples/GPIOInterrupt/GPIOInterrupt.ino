#include <Arduino.h>

struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {PB0, 0, false};
Button button2 = {BUTTON_BUILTIN, 0, false};

void led_toggle(pin_size_t ledPin) {
    if (digitalRead(ledPin) == HIGH)
        digitalWrite(ledPin, LOW);
    else
        digitalWrite(ledPin, HIGH);
}

void IRAM_ATTR isr1(void* arg) {
    Button* s = static_cast<Button*>(arg);
    s->numberKeyPresses += 1;
    s->pressed = true;
}

void IRAM_ATTR isr2() {
    button2.numberKeyPresses += 1;
    button2.pressed = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(button1.PIN, INPUT_PULLUP);
    attachInterruptArg(button1.PIN, isr1, &button1, FALLING);
    pinMode(button2.PIN, INPUT_PULLUP);
    attachInterrupt(button2.PIN, isr2, FALLING);
}

void loop() {
    if (button1.pressed) {
        Serial.printf("Button 1 has been pressed %lu times\n", button1.numberKeyPresses);
        button1.pressed = false;
    }
    if (button2.pressed) {
        led_toggle(LED_BUILTIN);
        Serial.printf("Button 2 has been pressed %lu times\n", button2.numberKeyPresses);
        button2.pressed = false;
    }
    static uint32_t lastMillis = 0;
    if (millis() - lastMillis > 10000) {
      lastMillis = millis();
      detachInterrupt(button1.PIN);
    }
}
