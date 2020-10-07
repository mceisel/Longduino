#include <Sipeed_GD32V_LCD.h>

Sipeed_GD32V_LCD lcd(160, 80);


void func()
{
    lcd.fillScreen(COLOR_RED);
    lcd.drawRect(5, 5, 15, 15, COLOR_WHITE);
    lcd.fillCircle(135, 60, 15, COLOR_WHITE);
    lcd.fillTriangle(40, 76, 100, 76, 100, 50, COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setTextColor(COLOR_WHITE);
    lcd.setCursor(5,25);
    lcd.println("hello Sipeed GD32V");
}

void func2()
{
    lcd.fillScreen(COLOR_RED);
    lcd.drawRect(5, 5, 15, 15, COLOR_WHITE);
    lcd.fillCircle(50, 135, 15, COLOR_WHITE);
    lcd.fillTriangle(10, 110, 60, 110, 60, 75, COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setTextColor(COLOR_WHITE);
    lcd.setCursor(1,25);
    lcd.println("hello Sipeed GD32V");
}

void setup()
{
    lcd.begin(15000000, COLOR_RED);    
}

void loop()
{
    lcd.setRotation(0);
    func();
    delay(3000);
    lcd.invertDisplay(true);
    func();
    delay(3000);
    lcd.setRotation(1);
    func2();
    delay(3000);
    lcd.setRotation(2);
    func();
    delay(3000);
    lcd.setRotation(3);
    func2();
    delay(3000);
}
