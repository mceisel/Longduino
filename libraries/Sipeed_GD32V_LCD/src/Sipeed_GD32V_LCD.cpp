
#include "Sipeed_GD32V_LCD.h"

#define SPI_FREQUENCY SPI_PSC_8

#define TFT_DC PB0
#define TFT_RST PB1
#define TFT_CS PB2

#define DC_C digitalWrite(TFT_DC, LOW);
#define DC_D digitalWrite(TFT_DC, HIGH);

#define CS_L digitalWrite(TFT_CS, LOW);
#define CS_H digitalWrite(TFT_CS, HIGH);

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

SPIClass SPI(PA7, PA6, PA5);

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
    SPI.begin();
    SPI.beginTransaction(SPISettings(freq, MSBFIRST, SPI_MODE3));

    pinMode(TFT_DC, OUTPUT);
    pinMode(TFT_RST, OUTPUT);
    pinMode(TFT_CS, OUTPUT);

    digitalWrite(TFT_RST, LOW);
    delay(200);
    digitalWrite(TFT_RST, HIGH);
    delay(20);

    writecommand(0x11);    // turn off sleep mode
    delay(100);

    writecommand(0x21);    // display inversion mode

    writecommand(0xB1);    // Set the frame frequency of the full colors normal mode
                        // Frame rate=fosc/((RTNA x 2 + 40) x (LINE + FPA + BPA +2))
                        // fosc = 850kHz
    writedata(0x05);    // RTNA
    writedata(0x3A);    // FPA
    writedata(0x3A);    // BPA

    writecommand(0xB2);    // Set the frame frequency of the Idle mode
                        // Frame rate=fosc/((RTNB x 2 + 40) x (LINE + FPB + BPB +2))
                        // fosc = 850kHz
    writedata(0x05);    // RTNB
    writedata(0x3A);    // FPB
    writedata(0x3A);    // BPB

    writecommand(0xB3);    // Set the frame frequency of the Partial mode/ full colors
    writedata(0x05);
    writedata(0x3A);
    writedata(0x3A);
    writedata(0x05);
    writedata(0x3A);
    writedata(0x3A);

    writecommand(0xB4);
    writedata(0x03);

    writecommand(0xC0);
    writedata(0x62);
    writedata(0x02);
    writedata(0x04);

    writecommand(0xC1);
    writedata(0xC0);

    writecommand(0xC2);
    writedata(0x0D);
    writedata(0x00);

    writecommand(0xC3);
    writedata(0x8D);
    writedata(0x6A);

    writecommand(0xC4);
    writedata(0x8D);
    writedata(0xEE);

    writecommand(0xC5);  /*VCOM*/
    writedata(0x0E);

    writecommand(0xE0);
    writedata(0x10);
    writedata(0x0E);
    writedata(0x02);
    writedata(0x03);
    writedata(0x0E);
    writedata(0x07);
    writedata(0x02);
    writedata(0x07);
    writedata(0x0A);
    writedata(0x12);
    writedata(0x27);
    writedata(0x37);
    writedata(0x00);
    writedata(0x0D);
    writedata(0x0E);
    writedata(0x10);

    writecommand(0xE1);
    writedata(0x10);
    writedata(0x0E);
    writedata(0x03);
    writedata(0x03);
    writedata(0x0F);
    writedata(0x06);
    writedata(0x02);
    writedata(0x08);
    writedata(0x0A);
    writedata(0x13);
    writedata(0x26);
    writedata(0x36);
    writedata(0x00);
    writedata(0x0D);
    writedata(0x0E);
    writedata(0x10);

    writecommand(0x3A);    // define the format of RGB picture data
    writedata(0x05);    // 16-bit/pixel

    writeRotation();

    writecommand(0x29);	// Display On

    fillScreen(color);

    return true;
}

/***************************************************************************************
** Function name:           writecommand
** Description:             Send an 8 bit command to the TFT
***************************************************************************************/
void Sipeed_GD32V_LCD::writecommand(uint8_t c) {
    DC_C;
    CS_L;
    SPI.transfer(c);
    CS_H;
    DC_D;
}

/***************************************************************************************
** Function name:           writedata
** Description:             Send a 8 bit data value to the TFT
***************************************************************************************/
void Sipeed_GD32V_LCD::writedata(uint8_t c) {
    CS_L;
    SPI.transfer(c);
    CS_H;
}

/***************************************************************************************
** Function name:           writedata16
** Description:             Send a 16 bit data value to the TFT
***************************************************************************************/
void Sipeed_GD32V_LCD::writedata16(uint16_t c) {
    CS_L;
    SPI.transfer(c>>8);
    SPI.transfer(c);
    CS_H;
}

void Sipeed_GD32V_LCD::drawImage(int16_t x1, int16_t y1, int16_t width, int16_t height, uint16_t* img)
{
    int i;
    setAddrWindow(x1, y1, x1+width-1, y1+height-1);
    CS_L;
    for(i=0;i<(width*height);i++)
    {
        SPI.transfer(img[i]);
        SPI.transfer(img[i]>>8);
    }
    CS_H;
}

void Sipeed_GD32V_LCD::drawWindow(int16_t x1, int16_t y1, int16_t x2, int16_t y2,uint16_t color, int16_t lineWidth)
{
    fillWindow(x1,y1,x2,y1+lineWidth-1,color);
    fillWindow(x1,y1,x1+lineWidth-1,y2,color);
    fillWindow(x1,y2-lineWidth+1,x2,y2,color);
    fillWindow(x2-lineWidth+1,y1,x2,y2,color);
}

void Sipeed_GD32V_LCD::fillWindow(int16_t xsta,int16_t ysta,int16_t xend,int16_t yend,uint16_t color)
{
    uint16_t i,j;
    if(xsta<0)xsta=0;
    if(ysta<0)ysta=0;
    if(xend<0)return;
    if(yend<0)return;
    if(_width<xsta)return;
    if(_height<ysta)return;
    if(_width<xend)xend=_width-1;
    if(_height<yend)yend=_height-1;
    setAddrWindow(xsta,ysta,xend,yend);      //设置光标位置
    for(i=ysta;i<=yend;i++)
    {
        for(j=xsta;j<=xend;j++)writedata16(color);//设置光标位置
    }
}

/******************************************************************************
           Function description: Set start and end addresses
       Entry data: x1, x2 set the start and end addresses of the column
                   y1, y2 set the start and end addresses of the line
       Return value: None
******************************************************************************/
void Sipeed_GD32V_LCD::setAddrWindow(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
    if(_screenDir==0)
    {
        writecommand(0x2a);//Column address settings
        writedata16(x1+26);
        writedata16(x2+26);
        writecommand(0x2b);//Row address setting
        writedata16(y1+1);
        writedata16(y2+1);
        writecommand(0x2c);//Memory write
    }
    else if(_screenDir==1)
    {
        writecommand(0x2a);//Column address settings
        writedata16(x1+26);
        writedata16(x2+26);
        writecommand(0x2b);//Row address setting
        writedata16(y1+1);
        writedata16(y2+1);
        writecommand(0x2c);//Memory write
    }
    else if(_screenDir==2)
    {
        writecommand(0x2a);//Column address settings
        writedata16(x1+1);
        writedata16(x2+1);
        writecommand(0x2b);//Row address setting
        writedata16(y1+26);
        writedata16(y2+26);
        writecommand(0x2c);//Memory write
    }
    else
    {
        writecommand(0x2a);//Column address settings
        writedata16(x1+1);
        writedata16(x2+1);
        writecommand(0x2b);//Row address setting
        writedata16(y1+26);
        writedata16(y2+26);
        writecommand(0x2c);//Memory write
    }
}

void Sipeed_GD32V_LCD::writeRotation(void)
{
    writecommand(0x36);

    if(_screenDir==0)
        writedata(0x08);
    else if(_screenDir==1)
        writedata(0xC8);
    else if(_screenDir==2)
        writedata(0x78);
    else
        writedata(0xA8);
}

void Sipeed_GD32V_LCD::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    setAddrWindow(x,y,x,y);
    writedata16(color);

}
void Sipeed_GD32V_LCD::writePixel(int16_t x, int16_t y, uint16_t color)
{
    drawPixel(x, y, color);
}

void Sipeed_GD32V_LCD::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    fillWindow(x, y , x+w-1, y+h-1, color);
}

void Sipeed_GD32V_LCD::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    fillWindow(x, y, x, y+h-1, color);
}

void Sipeed_GD32V_LCD::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    fillWindow(x, y, x+w-1, y, color);
}

void Sipeed_GD32V_LCD::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    fillWindow(x, y, x, y+h-1, color);
}

void Sipeed_GD32V_LCD::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    fillWindow(x, y, x+w-1, y, color);
}

void Sipeed_GD32V_LCD::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    fillWindow(x, y , x+w-1, y+h-1, color);
}

void Sipeed_GD32V_LCD::fillScreen(uint16_t color)
{
    fillWindow(0, 0, _width-1, _height-1, color);
}

void Sipeed_GD32V_LCD::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    drawWindow(x, y, x+w-1, y+h-1, color, 1);
}

void Sipeed_GD32V_LCD::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t lineWidth)
{
    drawWindow(x, y, x+w-1, y+h-1, color, lineWidth);
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
    writeRotation();
}

void Sipeed_GD32V_LCD::invertDisplay(boolean invert) {
    _screenDir = getValueByRotation(rotation);
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
    writeRotation();
}

/***************************************************************************************
** Function name:           color565
** Description:             convert three 8 bit RGB levels to a 16 bit colour
*value
***************************************************************************************/
uint16_t Sipeed_GD32V_LCD::color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
