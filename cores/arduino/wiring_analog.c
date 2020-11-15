#include "gd32vf103.h"
#include "pins_arduino.h"

uint8_t analog_reference = DEFAULT;
uint8_t b_adc_initialized[2] = { 0, 0 };

static void testInitAnalogRead(uint32_t adc_dev, rcu_periph_enum clk_id) {
    uint8_t idx = (adc_dev == ADC1) ? 1 : 0;
    if (b_adc_initialized[idx] == 0) {
        /* configure clock */
        rcu_periph_clock_enable(clk_id);
        rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV2);
        /* reset ADC0/1 */
        adc_deinit(adc_dev);
        /* configure ADC0/1 */
        adc_external_trigger_source_config(
            adc_dev, ADC_INSERTED_CHANNEL, ADC0_1_EXTTRIG_INSERTED_NONE);
        adc_external_trigger_config(adc_dev, ADC_INSERTED_CHANNEL, ENABLE);
        adc_channel_length_config(adc_dev, ADC_INSERTED_CHANNEL, 1);
        adc_enable(adc_dev);
        delayMicroseconds(1000);
        adc_calibration_enable(adc_dev);
        b_adc_initialized[idx] = 1;
    }
}

void analogReference(AnalogReferenceMode mode) {
#ifdef BOARD_SIPEED_LONGAN_NANO
    /* Longan nano does not have external vref pin, do nothing here */
#else
    analog_reference = mode;
#endif
}

int analogRead(pin_size_t pinNumber) {
    uint32_t adc_dev = ADC0;
    rcu_periph_enum clk_id = RCU_ADC0;
    if (pinNumber > VARIANT_GPIO_NUM) {
        return -1;
    }
#ifdef NO_ADC_PIN_MAP
    testInitAnalogRead(adc_dev, clk_id);
#else
    if (PIN_MAP[pinNumber].adc_device != 0
        && PIN_MAP[pinNumber].gpio_device != 0) {
        adc_dev = PIN_MAP[pinNumber].adc_device->adc_dev;
        clk_id = PIN_MAP[pinNumber].adc_device->clk_id;
        testInitAnalogRead(adc_dev, clk_id);
        adc_inserted_channel_config(
            adc_dev, 0, PIN_MAP[pinNumber].adc_channel, ADC_SAMPLETIME_1POINT5);
        adc_flag_clear(adc_dev, ADC_FLAG_EOIC);
        adc_software_trigger_enable(adc_dev, ADC_INSERTED_CHANNEL);
        while (adc_flag_get(adc_dev, ADC_FLAG_EOIC) != SET) {
            delayMicroseconds(1);
        }
        return ADC_IDATA0(adc_dev);
    }
    else {
        return -1;
    }
#endif
    return -1;
}

void analogWrite(pin_size_t pinNumber, int value) {
    if (pinNumber > VARIANT_GPIO_NUM || value < 0 || value > 255) {
        return;
    }
#ifndef NO_TIMER_PIN_MAP
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(PIN_MAP[pinNumber].timer_device->clk_id);
    timer_deinit(PIN_MAP[pinNumber].timer_device->timer_dev);
    timer_initpara.prescaler
        = rcu_clock_freq_get(CK_APB1) != rcu_clock_freq_get(CK_AHB)
        ? 2 * rcu_clock_freq_get(CK_APB1) / 1000000 - 1
        : rcu_clock_freq_get(CK_APB1) / 1000000 - 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 256;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(PIN_MAP[pinNumber].timer_device->timer_dev, &timer_initpara);

    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    if (PIN_MAP[pinNumber].timer_device->timer_dev == TIMER0) {
        timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
        timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
        timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
        timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    }
    timer_channel_output_config(PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, &timer_ocintpara);
    timer_channel_output_pulse_value_config(
        PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, value);
    timer_channel_output_mode_config(PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(
        PIN_MAP[pinNumber].timer_device->timer_dev,
        PIN_MAP[pinNumber].timer_channel, TIMER_OC_SHADOW_DISABLE);
    if (PIN_MAP[pinNumber].timer_device->timer_dev == TIMER0) {
        timer_primary_output_config(
            PIN_MAP[pinNumber].timer_device->timer_dev, ENABLE);
    }
    timer_auto_reload_shadow_enable(PIN_MAP[pinNumber].timer_device->timer_dev);
    timer_enable(PIN_MAP[pinNumber].timer_device->timer_dev);
#endif
}
