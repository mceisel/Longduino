#include "Arduino.h"

static void buzzBegin(pin_size_t pinNumber)
{
    pinMode(pinNumber, OUTPUT);
}

void buzz(pin_size_t pinNumber, unsigned int frequency, unsigned long duration) {
  long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * duration / 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to
  //// get the total number of cycles to produce
  buzzBegin(pinNumber);
  for (long i=0; i < numCycles; i++) { // for the calculated length of time...
    digitalWrite(pinNumber, HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(pinNumber, LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
}

#ifndef NO_TIMER_PIN_MAP
static void toneBegin(pin_size_t pinNumber)
{
    pinMode(pinNumber, OUTPUT_ANALOG);
}
#endif

void tone(pin_size_t pinNumber, unsigned int frequency, unsigned long duration)
{
    if (pinNumber > VARIANT_GPIO_NUM || frequency <= 0 || frequency > 1000000) {
        return;
    }
#ifdef NO_TIMER_PIN_MAP
    buzz(pinNumber, frequency,  duration);
#else
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;
    uint64_t _start_time;
    int32_t period = 255;
    int32_t value = 127;
    int32_t prescaler = (int32_t)(1000000 / frequency / 2);
    uint16_t clockdiv = TIMER_CKDIV_DIV1;

    if (!PIN_MAP[pinNumber].timer_device || !PIN_MAP[pinNumber].adc_device) {
      /* fallback to buzz for pins without PWM */
      buzz(pinNumber, frequency,  duration);
      return;
    }
    toneBegin(pinNumber);

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

    /* channel configuration in PWM mode, duty cycle 50% */
    timer_channel_output_pulse_value_config(
        PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, value);
    timer_channel_output_mode_config(PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(
        PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(PIN_MAP[pinNumber].timer_device->timer_dev);
    _start_time = millis();
    /* TIMER counter enable */
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
