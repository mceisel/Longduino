#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "usart_com.h"
#include "HardwareSerial.h"

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
HardwareSerial Serial(0);
HardwareSerial Serial1(1);
HardwareSerial Serial2(2);
#endif

HardwareSerial::HardwareSerial(int uart_nr) : _uartNr(uart_nr), _usartCom(0) {}

void HardwareSerial::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms)
{
    if(0 > _uartNr || _uartNr > 2) {
        //log_e("Serial number is invalid, please use 0, 1 or 2");
        return;
    }
    if(_usartCom) {
        end();
    }
    if(_uartNr == 0 && rxPin < 0 && txPin < 0) {
        txPin = 9;
        rxPin = 10;
    }
    if(_uartNr == 1 && rxPin < 0 && txPin < 0) {
        txPin = 2;
        rxPin = 3;
    }
    if(_uartNr == 2 && rxPin < 0 && txPin < 0) {
        txPin = 26;
        rxPin = 27;
    }

    if(_uartNr == 0) {
        _usartCom = USART0;
    }
    if(_uartNr == 1) {
        _usartCom = USART1;
    }
    if(_uartNr == 2) {
        _usartCom = USART2;
    }

    _usartBaud = baud ? baud : 115200U;
    _usartWlen = USART_WL_8BIT;
    usart_com_init(_usartCom, _usartWlen, _usartBaud);
}

void HardwareSerial::updateBaudRate(unsigned long baud)
{
    _usartBaud = baud;
    usart_baudrate_set(_usartCom, _usartBaud);
}

void HardwareSerial::end()
{
    usart_deinit(_usartCom);
    _usartCom = 0;
}

int HardwareSerial::available(void)
{
    int c = ((unsigned int)(SERIAL_RX_BUFFER_SIZE + _rx_buffer_head - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
    if (c > 0) {
      return c;
    } else {
      return usart_readable(_usartCom);
    }
}

int HardwareSerial::availableForWrite(void)
{
    return usart_writable(_usartCom);
}

int HardwareSerial::hwRead(void)
{
  if (usart_readable(_usartCom)) {
    unsigned char c = usart_get_char(_usartCom, _usartWlen);
    rx_buffer_index_t i = (unsigned int)(_rx_buffer_head + 1) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
      _rx_buffer[_rx_buffer_head] = c;
      _rx_buffer_head = i;
      return c;
    }
  }

  return -1;
}

int HardwareSerial::peek(void)
{
  if (_rx_buffer_head == _rx_buffer_tail) {
    return hwRead();
  } else {
    return _rx_buffer[_rx_buffer_tail];
  }
}

int HardwareSerial::read(void)
{
  int c;

  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer_head == _rx_buffer_tail) {
    c = hwRead();
  } else {
    c = _rx_buffer[_rx_buffer_tail];
  }
  if (c > -1)
    _rx_buffer_tail = (rx_buffer_index_t)(_rx_buffer_tail + 1) % SERIAL_RX_BUFFER_SIZE;
  return c;
}

void HardwareSerial::flush()
{
    while ( usart_flag_get(_usartCom, USART_FLAG_TBE)== RESET){
    }
}

size_t HardwareSerial::write(uint8_t c)
{
    usart_put_char(_usartCom, _usartWlen, c);
    return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
    usart_put_buf(_usartCom, _usartWlen, buffer, size);
    return size;
}
uint32_t  HardwareSerial::baudRate()

{
    return _usartBaud;
}
HardwareSerial::operator bool() const
{
    return true;
}

#ifdef GD32V_SERIAL_LIGHT
size_t HardwareSerial::print(const char str[])
{
  return write(str);
}

size_t HardwareSerial::print(char c)
{
  return write(c);
}

size_t HardwareSerial::println(void)
{
  return write("\r\n");
}

size_t HardwareSerial::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t HardwareSerial::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}
#endif
