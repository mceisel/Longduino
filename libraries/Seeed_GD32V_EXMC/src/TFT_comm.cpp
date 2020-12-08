#include "Arduino.h"
#include "EXMC.h"

#define	LCD_CMD_ADDR	0x60000000
#define	LCD_DAT_ADDR	(0x60000000+(0x03FFFFFE))

EXMCClass exmc(LCD_CMD_ADDR, LCD_DAT_ADDR);

void interface_begin()
{
    exmc.begin();
}

uint8_t interface_transfer(uint8_t data)
{
   //LOG(data);
  return exmc.transfer(data);
}

uint16_t interface_transfer16(uint16_t data)
{
  return exmc.transfer16(data);
}

void interface_transfer(void *data, uint32_t count)
{ 
  exmc.transfer(data, count);
}

void interface_end()
{
	return;
}

void interface_writeCommand(uint8_t c)
{
	exmc.command(c);
}

void interface_writeData(uint8_t d)
{
     exmc.transfer(d);
}

