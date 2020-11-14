#include <gd32vf103.h>
#include <stdlib.h>
#include "pins_arduino.h"

typedef struct _handler_list_t {
    uint8_t pinId;
    void* handler;
    PinStatus mode;
    void* param;
    struct _handler_list_t* next;
} handler_list_t;

static handler_list_t* handler_list = 0;

#define EXTI_HANDLER_ATTR

#define digitalPinToIRQn(p) PIN_MAP[p].irqn
#define digitalPinToPortSource(p) (p >> 4)
#define digitalPinToPinSource(p) (p & 0xF)
#define digitalPinToExtiLine(p) BIT(p & 0xF)

static void attachInterruptInternal(handler_list_t* ptr, uint8_t pinId,
    void* callback, PinStatus mode, void* param) {
    /* enable and set key EXTI interrupt to the lowest priority */
    eclic_global_interrupt_enable();
    eclic_irq_enable(digitalPinToIRQn(pinId),1, 1);

    /* connect key EXTI line to key GPIO pin */
    gpio_exti_source_select(digitalPinToPortSource(pinId), digitalPinToPinSource(pinId));

    /* configure key EXTI line */
    if (mode == RISING) {
        exti_init(digitalPinToExtiLine(pinId), EXTI_INTERRUPT, EXTI_TRIG_RISING);
    }
    else if (mode == FALLING) {
        exti_init(digitalPinToExtiLine(pinId), EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    }
    else if (mode == CHANGE) {
        exti_init(digitalPinToExtiLine(pinId), EXTI_INTERRUPT, EXTI_TRIG_BOTH);
    }
    else if (mode == RISING) {
        exti_init(digitalPinToExtiLine(pinId), EXTI_INTERRUPT, EXTI_TRIG_NONE);
    }
    exti_interrupt_flag_clear(digitalPinToExtiLine(pinId));

    exti_interrupt_enable(digitalPinToExtiLine(pinId));

    ptr->handler = callback;
    ptr->pinId = pinId;
    ptr->mode = mode;
    ptr->param = param;
}

void attachInterrupt(pin_size_t pinId, voidFuncPtr callback, PinStatus mode) {
    attachInterruptParam(pinId, (voidFuncPtrParam)callback, mode, 0);
}

void attachInterruptParam(
    pin_size_t pinId, voidFuncPtrParam callback, PinStatus mode, void* param) {
    if (pinId >= VARIANT_GPIO_NUM) {
        return;
    }
    if (callback == 0) {
        return;
    }
    if (handler_list == 0) {
        handler_list = calloc(1, sizeof(handler_list_t));
        attachInterruptInternal(handler_list, pinId, callback, mode, param);
        handler_list->next = 0;
        return;
    }
    handler_list_t* ptr = handler_list;
    handler_list_t* previousPtr = handler_list;
    while (ptr != 0) {
        if (ptr->pinId == pinId) {
            attachInterruptInternal(ptr, pinId, callback, mode, param);
            return;
        }
        previousPtr = ptr;
        ptr = ptr->next;
    }
    previousPtr->next = calloc(1, sizeof(handler_list_t));
    ptr = previousPtr->next;
    attachInterruptInternal(ptr, pinId, callback, mode, param);
    ptr->next = 0;
    return;
}

void detachInterrupt(pin_size_t pinId) {
    if (handler_list == 0) {
        return;
    }
    if (handler_list->pinId == pinId) {
        exti_interrupt_disable(digitalPinToExtiLine(pinId));
        if (handler_list->next == 0) {
            free(handler_list);
            handler_list = 0;
            return;
        }
        else {
            handler_list_t* ptr = handler_list;
            handler_list = handler_list->next;
            free(ptr);
            return;
        }
    }
    handler_list_t* ptr = handler_list->next;
    handler_list_t* previousPtr = handler_list;
    while (ptr != 0) {
        if (ptr->pinId == pinId) {
            exti_interrupt_disable(digitalPinToExtiLine(pinId));
            previousPtr->next = ptr->next;
            free(ptr);
            return;
        }
        previousPtr = ptr;
        ptr = ptr->next;
    }
    return;
}

static void generic_IRQHandler(void) {
    handler_list_t* ptr = handler_list;
    while (ptr != 0) {
        if (exti_interrupt_flag_get(digitalPinToExtiLine(ptr->pinId)) != RESET) {
            if (ptr->param == 0) {
                ((voidFuncPtr)ptr->handler)();
            } else {
                ((voidFuncPtrParam)ptr->handler)(ptr->param);
            }
            exti_interrupt_flag_clear(digitalPinToExtiLine(ptr->pinId));
            break;
        }
        ptr = ptr->next;
    }
    return;
}

void EXTI_HANDLER_ATTR EXTI0_IRQHandler(void) { generic_IRQHandler(); }
void EXTI_HANDLER_ATTR EXTI1_IRQHandler(void) { generic_IRQHandler(); }
void EXTI_HANDLER_ATTR EXTI2_IRQHandler(void) { generic_IRQHandler(); }
void EXTI_HANDLER_ATTR EXTI3_IRQHandler(void) { generic_IRQHandler(); }
void EXTI_HANDLER_ATTR EXTI4_IRQHandler(void) { generic_IRQHandler(); }
void EXTI_HANDLER_ATTR EXTI5_9_IRQHandler(void) {
    generic_IRQHandler();
}
void EXTI_HANDLER_ATTR EXTI10_15_IRQHandler(void) {
    generic_IRQHandler();
}
