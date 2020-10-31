/*
 Sipeed_GD32V_LCD example: image_display
 
 Shows 16-bit color raw bitmap images and animations on Longan Nano LCD.

 To run this, you need to put this files on SD card:
 https://github.com/sipeed/Longan_GD32VF_examples/tree/master/put_into_tf_card
 
 Convert your own images using ffmpeg:
 ffmpeg -i image.png -f rawvideo -pix_fmt rgb565 image.bin
*/

#include <SD.h>
#include <Sipeed_GD32V_LCD.h>

#define LCD_W 160
#define LCD_H 80

#define LCD_BUF_H 10

Sipeed_GD32V_LCD lcd(LCD_W, LCD_H);

static uint8_t lcdBuffer[LCD_W*LCD_BUF_H*2];
bool sdMounted = false;


void DrawImageFromFile(char* path)
{
    int offset = 0;
    uint16_t top = 0;
    uint32_t dataSize = 0;
    File dataFile;

    if (!sdMounted) {
        lcd.setTextSize(1);
        
        lcd.setTextColor(COLOR_BLACK);
        lcd.setCursor(24,16);
        lcd.println("No card found!");

        lcd.setTextColor(COLOR_BLUE);
        lcd.setCursor(24,32);
        lcd.println("No card found!");

        lcd.setTextColor(COLOR_RED);
        lcd.setCursor(24,48);
        lcd.println("No card found!");

        return;
    }
    
    dataFile = SD.open(path, FILE_READ);
    if (dataFile)
    {
        dataSize = dataFile.size();
        offset = 0;
        while (offset < dataSize) {
            top = 0;
            
            while (top < LCD_H) {
                dataFile.seek(offset);
                dataFile.read(lcdBuffer, (uint16_t)sizeof(lcdBuffer));
                lcd.drawImage(0,top,LCD_W,LCD_BUF_H,(uint16_t*)&lcdBuffer);
            
                offset += sizeof(lcdBuffer);
                top += LCD_BUF_H;
            }
        }

        dataFile.close();
    }
    else
    {
        lcd.setTextSize(1);
        lcd.setTextColor(COLOR_BLACK);
        lcd.setCursor(24,16);
        lcd.println("File not found.");
        
        lcd.setTextSize(2);
        lcd.setTextColor(COLOR_RED);
        lcd.setCursor(0,32);
        lcd.print("File "); lcd.print(path); lcd.println(" not found.");
    }
}

void setup()
{
    lcd.begin(13500000, COLOR_WHITE);
    lcd.setTextSize(2);
    lcd.setTextColor(COLOR_BLACK);

    sdMounted = SD.begin();
}

void loop()
{
    DrawImageFromFile("logo.bin");
    delay(1500);
    lcd.fillScreen(COLOR_WHITE);
    DrawImageFromFile("bmp.bin");
    delay(1500);
    lcd.fillScreen(COLOR_WHITE);
}
