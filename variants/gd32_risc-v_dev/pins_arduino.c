#include "pins_arduino.h"

#include "gd32vf103_libopt.h"



gpio_dev_t gpioa = {
    .gpio_port = GPIOA,
    .clk_id = RCU_GPIOA,
};

gpio_dev_t * const GPIO_A = &gpioa;

gpio_dev_t gpiob = {
    .gpio_port = GPIOB,
    .clk_id = RCU_GPIOB,
};

gpio_dev_t * const GPIO_B = &gpiob;

gpio_dev_t gpioc = {
    .gpio_port = GPIOC,
    .clk_id = RCU_GPIOC,
};

gpio_dev_t * const GPIO_C = &gpioc;

gpio_dev_t gpiod = {
    .gpio_port = GPIOD,
    .clk_id = RCU_GPIOD,
};

gpio_dev_t * const GPIO_D = &gpiod;

gpio_dev_t gpioe = {
    .gpio_port = GPIOE,
    .clk_id = RCU_GPIOE,
};
gpio_dev_t * const GPIO_E = &gpioe;

adc_dev_t adc0 = {
    .adc_dev = ADC0,
    .clk_id = RCU_ADC0,
};
adc_dev_t * const ADC_0 = &adc0;

adc_dev_t adc1 = {
    .adc_dev = ADC1,
    .clk_id = RCU_ADC1,
};
adc_dev_t * const ADC_1 = &adc1;

// TODO: add timer remap descriptor

timer_dev_t timer0 = {
    .timer_dev = TIMER0,
    .clk_id = RCU_TIMER0,
};
timer_dev_t * const TIMER_0 = &timer0;

timer_dev_t timer1 = {
    .timer_dev = TIMER1,
    .clk_id = RCU_TIMER1,
};
timer_dev_t * const TIMER_1 = &timer1;

timer_dev_t timer2 = {
    .timer_dev = TIMER2,
    .clk_id = RCU_TIMER2,
};
timer_dev_t * const TIMER_2 = &timer2;

timer_dev_t timer3 = {
    .timer_dev = TIMER3,
    .clk_id = RCU_TIMER3,
};
timer_dev_t * const TIMER_3 = &timer3;

timer_dev_t timer4 = {
    .timer_dev = TIMER4,
    .clk_id = RCU_TIMER4,
};
timer_dev_t * const TIMER_4 = &timer4;

timer_dev_t timer5 = {
    .timer_dev = TIMER5,
    .clk_id = RCU_TIMER5,
};
timer_dev_t * const TIMER_5 = &timer5;

timer_dev_t timer6 = {
    .timer_dev = TIMER6,
    .clk_id = RCU_TIMER6,
};
timer_dev_t * const TIMER_6 = &timer6;

spi_dev_t spi0 = {
    .spi_dev = SPI0,
    .clk_id = RCU_SPI0,
};
spi_dev_t * const SPI_0 = &spi0;

spi_dev_t spi1 = {
    .spi_dev = SPI1,
    .clk_id = RCU_SPI1,
};
spi_dev_t * const SPI_1 = &spi1;

spi_dev_t spi2 = {
    .spi_dev = SPI2,
    .clk_id = RCU_SPI2,
};
spi_dev_t * const SPI_2 = &spi2;

const gpio_dev_t * PORT_MAP[VARIANT_GPIO_PORT_NUM] = {
    &gpioa,
    &gpiob,
    &gpioc,
#if VARIANT_GPIO_NUM > 48
    &gpiod,
#endif
#if VARIANT_GPIO_NUM > 64
    &gpioe,
#endif
};

const gd32v_pin_info_t PIN_MAP[VARIANT_AF_NUM] = {
/*
    timer_dev *timer_device;    Pin's timer device, if any.
    const adc_dev *adc_device;  ADC device, if any. 
    uint8 timer_channel;        Timer channel, or 0 if none. 
    uint8 adc_channel;          Pin ADC channel, or ADCx if none. 
*/
    {&timer1, &adc0, 0, 0}, /* PA0 */
    {&timer1, &adc0, 1, 1}, /* PA1 */
    {&timer1, &adc0, 2, 2}, /* PA2 */
    {&timer1, &adc0, 3, 3}, /* PA3 */
    {0, &adc1, 0, 4}, /* PA4 */
    {0, &adc1, 0, 5}, /* PA5 */
    {&timer2, &adc1, 0, 6}, /* PA6 */
    {&timer2, &adc1, 1, 7}, /* PA7 */
    {&timer0, 0, 0, 0}, /* PA8 */
    {&timer0, 0, 1, 0}, /* PA9 */
    {&timer0, 0, 2, 0}, /* PA10 */
    {&timer0, 0, 3, 0}, /* PA11 */
    {0, 0, 0, 0}, /* PA12 */
    {0, 0, 0, 0}, /* PA13 */
    {0, 0, 0, 0}, /* PA14 */
    {0, 0, 0, 0}, /* PA15 */

    {&timer2, &adc0, 2, 8}, /* PB0 */
    {&timer2, &adc0, 3, 9}, /* PB1 */
    {0, 0, 0, 0}, /* PB2 */
    {0, 0, 0, 0}, /* PB3 */
    {0, 0, 0, 0}, /* PB4 */
    {0, 0, 0, 0}, /* PB5 */
    {&timer3, 0, 0, 0}, /* PB6 */
    {&timer3, 0, 1, 0}, /* PB7 */
    {&timer3, 0, 2, 0}, /* PB8 */
    {&timer3, 0, 3, 0}, /* PB9 */
    {0, 0, 0, 0}, /* PB10 */
    {0, 0, 0, 0}, /* PB11 */
    {0, 0, 0, 0}, /* PB12 */
    {0, 0, 0, 0}, /* PB13 */
    {0, 0, 0, 0}, /* PB14 */
    {0, 0, 0, 0}, /* PB15 */

    {0, &adc0, 0, 10}, /* PC0 */
    {0, &adc0, 0, 11}, /* PC1 */
    {0, &adc1, 0, 12}, /* PC2 */
    {0, &adc1, 0, 13}, /* PC3 */
    {0, &adc1, 0, 14}, /* PC4 */
    {0, &adc1, 0, 15}, /* PC5 */
    {0, 0, 0, 0}, /* PC6 */
    {0, 0, 0, 0}, /* PC7 */
};

const spi_map_t SPI_MAP[VARIANT_SPI_NUM] = {
    {&spi0, PA4, PA5, PA6, PA7},
    {&spi2, PA15, PB3, PB4, PB5},
    {&spi1, PB12, PB13, PB14, PB15},
};

const timer_dev_t * TIMER_MAP[VARIANT_TIMER_NUM] = {
  TIMER_0,
  TIMER_1,
  TIMER_2,
  TIMER_3,
  TIMER_4,
  TIMER_5,
  TIMER_6,
};
