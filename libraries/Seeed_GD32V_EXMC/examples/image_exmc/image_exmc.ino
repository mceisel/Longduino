/*
 Seeed_GD32V_EXMC example: image_display
 
 Shows 16-bit color raw bitmap images and animations on Seeed EXMC TFT.

 To run this, you need to put this files on SD card:
 https://github.com/sipeed/Longan_GD32VF_examples/tree/master/put_into_tf_card
 
 Convert your own images using ffmpeg:
 ffmpeg -i image.png -f rawvideo -pix_fmt rgb565be image.bin
*/

#include <SPI.h>
#include <SD.h>
#include "EXMC.h"
#include "TFT_eSPI.h"

// Use hardware SPI
TFT_eSPI lcd = TFT_eSPI();

#define LCD_W 320
#define LCD_H 240

#define LCD_IMG_W 160
#define LCD_IMG_H 80

#define LCD_BUF_H 10

static uint8_t lcdBuffer[LCD_IMG_W*LCD_BUF_H*2];
bool sdMounted = false;


void lcd_drawImage(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t* img)
{
    uint16_t curcolor = 0;
    lcd.setAddrWindow(x1, y1, width, height);
    lcd.startWrite();
    for (int i = 0; i< (width*height); i++) {
      curcolor = ((img[i] & 0xFF) << 8) | ((img[i] & 0xFF00) >> 8);
      lcd.pushColor(curcolor, 1);
    }
    lcd.endWrite();
}

void DrawImageFromFile(uint16_t x,uint16_t y,uint16_t width,uint16_t height, const char* path)
{
    uint32_t offset = 0;
    uint16_t top = 0;
    uint16_t chunklines = LCD_BUF_H;
    uint16_t bottomlines = 0;
    uint32_t dataSize = 0;
    size_t chunksize = sizeof(lcdBuffer);
    size_t bottomsize;
    File dataFile;

    if (!sdMounted) {
        lcd.setTextSize(1);
        
        lcd.setTextColor(TFT_BLACK);
        lcd.setCursor(x+24,y+16);
        lcd.println("No card found!");

        lcd.setTextColor(TFT_BLUE);
        lcd.setCursor(x+24,y+32);
        lcd.println("No card found!");

        lcd.setTextColor(TFT_RED);
        lcd.setCursor(x+24,y+48);
        lcd.println("No card found!");

        return;
    }

    chunklines=chunksize/(width*2);
    chunksize=width*2*chunklines;
    
    dataFile = SD.open(path, FILE_READ);
    if (dataFile)
    {
        dataSize = dataFile.size();
        offset = 0;

        while (offset < dataSize) {
            top = 0;
            
            while (top+chunklines < height) {
                dataFile.read(lcdBuffer, (uint16_t)chunksize);
                lcd_drawImage(x,y+top,width,chunklines,(uint16_t*)&lcdBuffer);
            
                offset += chunksize;
                top += chunklines;
            }

            if (top < height) {
                bottomlines = height-top;
                bottomsize=width*2*bottomlines;
                
                dataFile.read(lcdBuffer, (uint16_t)bottomsize);
                lcd_drawImage(x,y+top,width,bottomlines,(uint16_t*)&lcdBuffer);
            
                offset += bottomsize;
                top += bottomlines;
            }
        }

        dataFile.close();
    }
    else
    {
        lcd.setTextSize(1);
        lcd.setTextColor(TFT_BLACK);
        lcd.setCursor(x+24,y+16);
        lcd.println("File not found.");
        
        lcd.setTextSize(2);
        lcd.setTextColor(TFT_RED);
        lcd.setCursor(x+0,y+32);
        lcd.printf("File %s not found.", path);
    }
}

void setup()
{
    lcd.begin();
    lcd.setTextSize(2);
    lcd.setTextColor(TFT_BLACK);

    sdMounted = SD.begin();
}

void loop()
{
    lcd.setRotation(1);
    DrawImageFromFile(40, 20, LCD_IMG_W, LCD_IMG_H, "logo.bin");
    delay(1500);
    lcd.fillScreen(TFT_WHITE);
    DrawImageFromFile(40, 20, LCD_IMG_W, LCD_IMG_H, "bmp.bin");
    delay(1500);
    lcd.fillScreen(TFT_WHITE);
}
