#include "usart_types.h"
#include "usart_com.h"

usart_buffer_t usart_rx_buffer[3];

void usart_com_init(uint32_t com, uint32_t wlen, unsigned long baud)
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
    }else if(com == USART2){
        /* enable GPIO clock */
        rcu_periph_clock_enable(RCU_GPIOB);
        /* enable USART clock */
        rcu_periph_clock_enable(RCU_USART2);
        /* connect port to USARTx_Tx */
        gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
        /* connect port to USARTx_Rx */
        gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    }else{
        return;
    }
    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, baud);
    usart_word_length_set(com, wlen);
    usart_stop_bit_set(com, USART_STB_1BIT);
    usart_parity_config(com, USART_PM_NONE);
    usart_hardware_flow_rts_config(com, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(com, USART_CTS_DISABLE);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);

    return;
}

int usart_readable(uint32_t com)
{
    return (usart_flag_get(com, USART_FLAG_RBNE) != RESET) ? 1 : 0;
}

int usart_writable(uint32_t com)
{
    return (usart_flag_get(com, USART_FLAG_TBE) != RESET) ? 1 : 0;
}

int usart_put_char(uint32_t com, uint32_t wlen, uint8_t ch)
{
    usart_data_transmit(com, (int)((ch) & BITS(0, 7 + (wlen >> 12))));
    while (!usart_writable(com));

    return ch;
}

int usart_put_buf(uint32_t com, uint32_t wlen, const uint8_t *buffer, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        usart_put_char(com, wlen, buffer[i]);
    }
    return size;
}

int usart_get_char(uint32_t com, uint32_t wlen)
{
    while (!usart_readable(com));
    return (int)(usart_data_receive(com) & BITS(0, 7 + (wlen >> 12)));
}

static int usart_irq_handler(uint8_t _usart_nr)
{
  usart_buffer_t* _rx_buffer = &usart_rx_buffer[_usart_nr];

  if (usart_readable(_rx_buffer->_com)) {
    unsigned char c = usart_get_char(_rx_buffer->_com, _rx_buffer->wlen);
    usart_buffer_index_t i = (unsigned int)(_rx_buffer->head + 1) % SERIAL_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer->tail) {
      _rx_buffer->data[_rx_buffer->head] = c;
      _rx_buffer->head = i;
      return c;
    }
  }

  return -1;
}

void USART0_IRQHandler(void)
{
    usart_irq_handler(0);
}

void USART1_IRQHandler(void)
{
    usart_irq_handler(1);
}

void USART2_IRQHandler(void)
{
    usart_irq_handler(2);
}
