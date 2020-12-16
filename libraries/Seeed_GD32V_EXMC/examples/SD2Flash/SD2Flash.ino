/*
 Seeed_GD32V_EXMC example: SD2Flash
 
 Stores file from SD card to SPI Flash.

 To run this, you need to put this files on SD card:
 https://github.com/sipeed/Longan_GD32VF_examples/tree/master/put_into_tf_card
 
 Convert your own images using ffmpeg:
 ffmpeg -i image.png -f rawvideo -pix_fmt rgb565be image.bin
*/

#include <SPI.h>
#include <SPIMemory.h>
#include <SD.h>
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
bool sdMounted = false;


void CopyFileToFlash(uint32_t addr, const char* path)
{
    uint32_t offset = 0;
    uint32_t dataSize = 0;
    uint32_t aligned_len = 0;
    size_t chunksize = sizeof(lcdBuffer);
    size_t bottomsize;
    File dataFile;

    if (!sdMounted) {
        lcd.setTextSize(1);
        
        lcd.setTextColor(TFT_BLACK);
        //lcd.setCursor(x+24,y+16);
        lcd.println("No card found!");

        lcd.setTextColor(TFT_BLUE);
        //lcd.setCursor(x+24,y+32);
        lcd.println("No card found!");

        lcd.setTextColor(TFT_RED);
        //lcd.setCursor(x+24,y+48);
        lcd.println("No card found!");

        return;
    }
    
    dataFile = SD.open(path, FILE_READ);
    if (dataFile)
    {
        dataSize = dataFile.size();
        offset = 0;

        if (flash.getCapacity() < dataSize)
            dataSize = flash.getCapacity();

        aligned_len = (dataSize + (0x10000-1)) & ~(0x10000-1);

        lcd.printf("Erasing %lu bytes...\n", aligned_len);
        while (offset < dataSize) {
          flash.eraseBlock64K(addr+offset);
          offset += 0x10000;
        }

        while (offset+chunksize < dataSize) {
                lcd.printf("Writing %d bytes...\n", chunksize);
                dataFile.read(lcdBuffer, (uint16_t)chunksize);
                flash.writeByteArray(addr+offset, lcdBuffer, (uint16_t)chunksize);
            
                offset += chunksize;
        }

        if (offset < dataSize) {
                bottomsize=dataSize-offset;
                lcd.printf("Writing %d bytes...\n", bottomsize);
                dataFile.read(lcdBuffer, (uint16_t)bottomsize);
                flash.writeByteArray(addr+offset, lcdBuffer, (uint16_t)bottomsize);
            
                offset += bottomsize;
        }

        lcd.printf("Total %lu bytes written.\n", offset);
        lcd.println("Done.");

        dataFile.close();
    }
    else
    {
        lcd.setTextSize(1);
        lcd.setTextColor(TFT_BLACK);
        //lcd.setCursor(x+24,y+16);
        lcd.println("File not found.");
        
        lcd.setTextSize(2);
        lcd.setTextColor(TFT_RED);
        //lcd.setCursor(x+0,y+32);
        lcd.printf("File %s not found.\n", path);
    }  
}

void setup()
{
    lcd.begin();
    lcd.setTextSize(2);
    lcd.setTextColor(TFT_BLACK);

    sdMounted = SD.begin();
    flash.begin();

    lcd.setRotation(3);
    // write at address 0
    CopyFileToFlash(0, "logo.bin");
}

void loop()
{
    //
}
