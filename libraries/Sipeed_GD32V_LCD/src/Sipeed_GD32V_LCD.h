#ifndef _SIPEED_GD32V_LCD_H
#define _SIPEED_GD32V_LCD_H


#include <SPI.h>
#include <Adafruit_GFX.h>


#define COLOR_BLACK       0x0000
#define COLOR_NAVY        0x000F
#define COLOR_DARKGREEN   0x03E0
#define COLOR_DARKCYAN    0x03EF
#define COLOR_MAROON      0x7800
#define COLOR_PURPLE      0x780F
#define COLOR_OLIVE       0x7BE0
#define COLOR_LIGHTGREY   0xC618
#define COLOR_DARKGREY    0x7BEF
#define COLOR_BLUE        0x001F
#define COLOR_GREEN       0x07E0
#define COLOR_CYAN        0x07FF
#define COLOR_RED         0xF800
#define COLOR_MAGENTA     0xF81F
#define COLOR_YELLOW      0xFFE0
#define COLOR_WHITE       0xFFFF
#define COLOR_ORANGE      0xFD20
#define COLOR_GREENYELLOW 0xAFE5
#define COLOR_PINK        0xF81F


class Sipeed_GD32V_LCD : public Adafruit_GFX{
public:
    /**
     * @param rst_pin -1: not use reset pin
     */
    Sipeed_GD32V_LCD(uint16_t w, uint16_t h);

    ~Sipeed_GD32V_LCD(void);

    boolean      begin( uint32_t freq = 15000000, uint16_t color = 0xffff );

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void writePixel(int16_t x, int16_t y, uint16_t color);
    virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

    virtual void
    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    fillScreen(uint16_t color),
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t lineWidth);

    virtual void setRotation(uint8_t r);
    virtual void invertDisplay(boolean invert);

    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);

    void drawImage(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t* img);

private:
    void writecommand(uint8_t c);
    void writedata(uint8_t c);
    void writedata16(uint16_t c);
    void drawWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color, uint16_t lineWidth);
    void fillWindow(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
    void setAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
    void writeRotation(void);

    uint32_t  _freq;
    uint16_t  _screenDir;


};

#endif

