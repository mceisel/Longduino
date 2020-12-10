/*
 Seeed_GD32V_EXMC example: Flash2TFT
 
 Shows 16-bit color raw bitmap images and animations on Seeed EXMC TFT.

 Before you can run this, you need to run SD2Flash example one time
 to store image in SPI flash memory.
*/

#include <SPI.h>
#include <SPIMemory.h>
#include "EXMC.h"
#include "TFT_eSPI.h"

SPIFlash flash;

// Use hardware SPI
TFT_eSPI lcd = TFT_eSPI();

#define LCD_W 320
#define LCD_H 240

#define LCD_IMG_W 160
#define LCD_IMG_H 80

#define LCD_BUF_H 10

static uint8_t lcdBuffer[LCD_IMG_W*LCD_BUF_H*2];


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

void DrawImageFromFlash(uint16_t x,uint16_t y,uint16_t width,uint16_t height, uint32_t addr)
{
    uint32_t offset = 0;
    uint16_t top = 0;
    uint16_t chunklines = LCD_BUF_H;
    uint16_t bottomlines = 0;
    uint32_t dataSize = width*height*2;
    size_t chunksize = sizeof(lcdBuffer);
    size_t bottomsize;

    chunklines=chunksize/(width*2);
    chunksize=width*2*chunklines;
    
    offset = 0;

    while (offset < dataSize) {
        top = 0;
            
        while (top+chunklines < height) {
            flash.readByteArray(addr+offset, lcdBuffer, (uint16_t)chunksize);
            lcd_drawImage(x,y+top,width,chunklines,(uint16_t*)&lcdBuffer);
            
            offset += chunksize;
            top += chunklines;
        }

        if (top < height) {
            bottomlines = height-top;
            bottomsize=width*2*bottomlines;
                
            flash.readByteArray(addr+offset, lcdBuffer, (uint16_t)bottomsize);
            lcd_drawImage(x,y+top,width,bottomlines,(uint16_t*)&lcdBuffer);
            
            offset += bottomsize;
            top += bottomlines;
        }
    }
}

void setup()
{
    lcd.begin();
    lcd.setTextSize(2);
    lcd.setTextColor(TFT_BLACK);

    flash.begin();

    lcd.setRotation(3);
    // read image from address 0 and draw image
    DrawImageFromFlash(40, 20, LCD_IMG_W, LCD_IMG_H, 0);
}

void loop()
{
    //
}
