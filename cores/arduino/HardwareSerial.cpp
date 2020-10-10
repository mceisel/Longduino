#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "pins_arduino.h"
#include "HardwareSerial.h"

static void usart_com_init(uint32_t com, unsigned long baud)
{
    if(com == USART0){
        /* enable GPIO clock */
        rcu_periph_clock_enable(RCU_GPIOA);
        /* enable USART clock */
        rcu_periph_clock_enable(RCU_USART0);
        /* connect port to USARTx_Tx */
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
        /* connect port to USARTx_Rx */
        gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    }else if(com == USART1){
        /* enable GPIO clock */
        rcu_periph_clock_enable(RCU_GPIOA);
        /* enable USART clock */
        rcu_periph_clock_enable(RCU_USART1);
        /* connect port to USARTx_Tx */
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);

        /* connect port to USARTx_Rx */
        gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    }else{
        return;
    }
    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, baud);
    usart_word_length_set(com, USART_WL_8BIT);
    usart_stop_bit_set(com, USART_STB_1BIT);
    usart_parity_config(com, USART_PM_NONE);
    usart_hardware_flow_rts_config(com, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(com, USART_CTS_DISABLE);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
    return;
}

static int usart_put_char(uint32_t com, uint8_t ch)
{
    usart_data_transmit(com, ch);
    while ( usart_flag_get(com, USART_FLAG_TBE)== RESET){
    }

    return ch;
}

static int usart_put_buf(uint32_t com, const uint8_t *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        usart_put_char(com, buffer[i]);
    }
    return size;
}

static int usart_get_char(uint32_t com)
{
    return usart_data_receive(com);
}

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
        rxPin = 9;
        txPin = 10;
    }
    if(_uartNr == 1 && rxPin < 0 && txPin < 0) {
        rxPin = 2;
        txPin = 3;
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
    usart_com_init(_usartCom, _usartBaud);
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
    return (usart_flag_get(_usartCom, USART_FLAG_RBNE) != RESET) ? 1 : 0;
}
int HardwareSerial::availableForWrite(void)
{
    return (usart_flag_get(_usartCom, USART_FLAG_TBE) != RESET) ? 1 : 0;;
}

int HardwareSerial::peek(void)
{
/*
    if (available()) {
        return uartPeek(_usartCom);
    }
*/    
    return -1;
}

int HardwareSerial::read(void)
{
    if(available()) {
        return usart_get_char(_usartCom);
    }
    return -1;
}

void HardwareSerial::flush()
{
    while ( usart_flag_get(_usartCom, USART_FLAG_TBE)== RESET){
    }
}

size_t HardwareSerial::write(uint8_t c)
{
    usart_put_char(_usartCom, c);
    return 1;
}

size_t HardwareSerial::write(const uint8_t *buffer, size_t size)
{
    usart_put_buf(_usartCom, buffer, size);
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

