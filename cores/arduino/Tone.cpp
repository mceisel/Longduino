#include "Arduino.h"

static void toneBegin(pin_size_t pinNumber)
{
    pinMode(pinNumber, OUTPUT_ANALOG);
}

void tone(pin_size_t pinNumber, unsigned int frequency, unsigned long duration)
{
    if (pinNumber > VARIANT_GPIO_NUM || frequency <= 0 || frequency > 1000000) {
        return;
    }
    toneBegin(pinNumber);
#ifndef NO_TIMER_PIN_MAP
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;
    uint64_t _start_time;
    int32_t period = 255;
    int32_t value = 127;
    int32_t prescaler = (int32_t)(1000000 / frequency / 2);
    uint16_t clockdiv = TIMER_CKDIV_DIV1;

    if (prescaler > 65535)
    {
      prescaler/=2;
      clockdiv = TIMER_CKDIV_DIV2;
    }
    if (prescaler > 65535)
    {
      prescaler/=2;
      clockdiv = TIMER_CKDIV_DIV4;
    }

    rcu_periph_clock_enable(PIN_MAP[pinNumber].timer_device->clk_id);
    timer_deinit(PIN_MAP[pinNumber].timer_device->timer_dev);

    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER configuration */
    timer_initpara.prescaler = prescaler - 1;

    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = period;
    timer_initpara.clockdivision = clockdiv;
    timer_initpara.repetitioncounter = 0;
    timer_init(PIN_MAP[pinNumber].timer_device->timer_dev, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* channel configuration in PWM mode */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, &timer_ocinitpara);

    timer_channel_output_pulse_value_config(
        PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, value);

    timer_channel_output_mode_config(PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(
        PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, TIMER_OC_SHADOW_DISABLE);

    timer_auto_reload_shadow_enable(PIN_MAP[pinNumber].timer_device->timer_dev);
    _start_time = millis();
    timer_enable(PIN_MAP[pinNumber].timer_device->timer_dev);
    if (duration > 0) {
        while ( millis() - _start_time < duration );
        timer_deinit(PIN_MAP[pinNumber].timer_device->timer_dev);
    }
#endif
}

void noTone(pin_size_t pinNumber) 
{
    if (pinNumber > VARIANT_GPIO_NUM) {
        return;
    }
#ifndef NO_TIMER_PIN_MAP
    timer_deinit(PIN_MAP[pinNumber].timer_device->timer_dev);
#endif
}
