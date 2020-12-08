// Spiro
// Rainbow patern generator

#include <Sipeed_GD32V_LCD.h>

#define LCD_W 160
#define LCD_H 80

Sipeed_GD32V_LCD tft(LCD_W, LCD_H);

#define DEG2RAD 0.0174532925 // Convert angles in degrees to radians

unsigned long runTime = 0;

float sx = 0, sy = 0;
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;

void setup() {
    //randomSeed(analogRead(A0));

    // Setup the LCD
    tft.begin();
    tft.setRotation(2);
}

void loop() {
    runTime = millis();

    tft.fillScreen(COLOR_BLACK);
    int n = random(2, 23), r = random(20, (LCD_H/2)-20), colour = 0; //rainbow();

    for (long i = 0; i < (360 * n); i++) {
        sx = cos((i / n - 90) * DEG2RAD);
        sy = sin((i / n - 90) * DEG2RAD);
        x0 = sx * ((LCD_H/2) - r) + (LCD_W/2)-1;
        yy0 = sy * ((LCD_H/2) - r) + (LCD_H/2)-1;


        sy = cos(((i % 360) - 90) * DEG2RAD);
        sx = sin(((i % 360) - 90) * DEG2RAD);
        x1 = sx * r + x0;
        yy1 = sy * r + yy0;
        tft.drawPixel(x1, yy1, rainbow(map(i % 360, 0, 360, 0, 127))); //colour);
    }

    r = random(20, (LCD_H/2)-20);//r = r / random(2,4);
    for (long i = 0; i < (360 * n); i++) {
        sx = cos((i / n - 90) * DEG2RAD);
        sy = sin((i / n - 90) * DEG2RAD);
        x0 = sx * ((LCD_H/2) - r) + (LCD_W/2)-1;
        yy0 = sy * ((LCD_H/2) - r) + (LCD_H/2)-1;


        sy = cos(((i % 360) - 90) * DEG2RAD);
        sx = sin(((i % 360) - 90) * DEG2RAD);
        x1 = sx * r + x0;
        yy1 = sy * r + yy0;
        tft.drawPixel(x1, yy1, rainbow(map(i % 360, 0, 360, 0, 127))); //colour);
    }


    delay(2000);
}

unsigned int rainbow(int value) {
    // Value is expected to be in range 0-127
    // The value is converted to a spectrum colour from 0 = blue through to red = blue
    //int value = random (128);
    byte red = 0; // Red is the top 5 bits of a 16 bit colour value
    byte green = 0;// Green is the middle 6 bits
    byte blue = 0; // Blue is the bottom 5 bits

    byte quadrant = value / 32;

    if (quadrant == 0) {
        blue = 31;
        green = 2 * (value % 32);
        red = 0;
    }
    if (quadrant == 1) {
        blue = 31 - (value % 32);
        green = 63;
        red = 0;
    }
    if (quadrant == 2) {
        blue = 0;
        green = 63;
        red = value % 32;
    }
    if (quadrant == 3) {
        blue = 0;
        green = 63 - 2 * (value % 32);
        red = 31;
    }
    return (red << 11) + (green << 5) + blue;
}
