
#include "Sipeed_GD32V_LCD.h"
#include "lcd.h"

#define DIR_INVERT 0

typedef enum _lcd_dir
{
    DIR_XY_RLUD = 0,
    DIR_YX_RLUD = 2 | DIR_INVERT,
    DIR_XY_LRUD = 0 | DIR_INVERT,
    DIR_YX_LRUD = 3,
    DIR_XY_RLDU = 1 | DIR_INVERT,
    DIR_YX_RLDU = 2,
    DIR_XY_LRDU = 1,
    DIR_YX_LRDU = 3 | DIR_INVERT,
} lcd_dir_t;

Sipeed_GD32V_LCD::Sipeed_GD32V_LCD(uint16_t w, uint16_t h )
:Adafruit_GFX(w,h),
 _screenDir(DIR_YX_RLDU)
{

}

Sipeed_GD32V_LCD::~Sipeed_GD32V_LCD(void)
{

}

boolean Sipeed_GD32V_LCD::begin( uint32_t freq, uint16_t color )
{
    Lcd_Init();			// init OLED
    LCD_Clear(color);

    return true;
}


void Sipeed_GD32V_LCD::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    LCD_DrawPoint(x, y, color);
}
void Sipeed_GD32V_LCD::writePixel(int16_t x, int16_t y, uint16_t color)
{
    LCD_DrawPoint(x, y, color);
}

void Sipeed_GD32V_LCD::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    LCD_Fill(x, y , x+w-1, y+h-1, color);
}

void Sipeed_GD32V_LCD::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    LCD_Fill(x, y, x, y+h-1, color);
}

void Sipeed_GD32V_LCD::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    LCD_Fill(x, y, x+w-1, y, color);
}

void Sipeed_GD32V_LCD::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    LCD_Fill(x, y, x, y+h-1, color);
}

void Sipeed_GD32V_LCD::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    LCD_Fill(x, y, x+w-1, y, color);
}

void Sipeed_GD32V_LCD::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    LCD_Fill(x, y , x+w-1, y+h-1, color);
}

void Sipeed_GD32V_LCD::fillScreen(uint16_t color)
{
    LCD_Clear(color);
}

void Sipeed_GD32V_LCD::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    //LCD_DrawRectangle(x, y, x+w-1, y+h-1, 1, color);
    LCD_DrawRectangle(x, y, x+w-1, y+h-1, color);
}

void Sipeed_GD32V_LCD::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t lineWidth)
{
    //LCD_DrawRectangle(x, y, x+w-1, y+h-1, lineWidth, color);
    LCD_DrawRectangle(x, y, x+w-1, y+h-1, color);
}


uint16_t getValueByRotation(uint8_t rotation)
{
    uint16_t v = DIR_YX_RLDU;
    switch(rotation) {
        case 0:
            v = DIR_YX_RLDU;
            break;
        case 1:
            v = DIR_XY_RLUD;
            break;
        case 2:
            v = DIR_YX_LRUD;
            break;
        case 3:
            v = DIR_XY_LRDU;
            break;
    }
    return v;
}

/**************************************************************************/
/*!
    @brief      Set rotation setting for display
    @param  x   0 thru 3 corresponding to 4 cardinal rotations
*/
/**************************************************************************/
void Sipeed_GD32V_LCD::setRotation(uint8_t x) {
    rotation = (x & 3);
    if (rotation > 3)
        return;

    switch(rotation) {
        case 0:
        case 2:
            _width  = WIDTH;
            _height = HEIGHT;
            break;
        case 1:
        case 3:
            _width  = HEIGHT;
            _height = WIDTH;
            break;
    }
    _screenDir = getValueByRotation(rotation);
    LCD_SetRotation(_screenDir);
}

void Sipeed_GD32V_LCD::invertDisplay(boolean invert) {
    uint16_t _screenDir = getValueByRotation(rotation);
    if( invert )
    {
        switch(rotation) {
            case 0:
                _screenDir = DIR_YX_RLUD;
                break;
            case 1:
                _screenDir = DIR_XY_LRUD;
                break;
            case 2:
                _screenDir = DIR_YX_LRDU;
                break;
            case 3:
                _screenDir = DIR_XY_RLDU;
                break;
        }
    }
    LCD_SetRotation(_screenDir);
}

void Sipeed_GD32V_LCD::drawImage(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t* img)
{
    if (img)
        memcpy(image, img, sizeof(image));

    LCD_ShowPicture(x1, y1, width, height);
}

