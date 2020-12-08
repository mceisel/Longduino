#include "SPI.h"
#include "EXMC.h"
#include "TFT_eSPI.h"

// Use hardware SPI
TFT_eSPI lcd = TFT_eSPI();

#define LCD_W 320
#define LCD_H 240

void func()
{
    lcd.fillScreen(TFT_RED);
    lcd.drawRect(20, 20, 50, 50, TFT_WHITE);
    lcd.fillCircle(100, 100, 40, TFT_WHITE);
    lcd.fillTriangle(10, 200, 300, 200, 300, 150, TFT_WHITE);
    lcd.setTextSize(2);
    lcd.setTextColor(TFT_WHITE);
    lcd.setCursor(100,30);
    lcd.println("hello Seeed GD32V");
}

void func2()
{
    lcd.fillScreen(TFT_RED);
    lcd.drawRect(20, 20, 50, 50, TFT_WHITE);
    lcd.fillCircle(180, 50, 40, TFT_WHITE);
    lcd.fillTriangle(10, 300, 200, 300, 200, 150, TFT_WHITE);
    lcd.setTextSize(2);
    lcd.setTextColor(TFT_WHITE);
    lcd.setCursor(1,100);
    lcd.println("hello Seeed GD32V");
}

void setup()
{
    lcd.begin();    
}

void loop()
{
    lcd.setRotation(1);
    func();
    delay(3000);
    lcd.invertDisplay(true);
    func();
    delay(3000);
    lcd.invertDisplay(false);
    lcd.setRotation(0);
    func2();
    delay(3000);
    lcd.setRotation(3);
    func();
    delay(3000);
    lcd.setRotation(2);
    func2();
    delay(3000);
}
