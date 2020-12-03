#include "Ticker.h"
#include "TInterrupts.h"

static void timer_callback(void* ctx);

Ticker::Ticker(timer_id_t id)
:timer_id(id)
{
    /* */
}

Ticker::~Ticker()
{
    detach();
}

void
Ticker::_attach_ms(uint32_t milliseconds, bool repeat, callback_with_arg_t callback, size_t arg)
{
    user_callback = callback;
    _arg = arg;
    _repeat = repeat; 
    attachTickerParam(timer_id, timer_callback, milliseconds * 1000, this);
}

void
Ticker::_attach_us(uint32_t microseconds, bool repeat, callback_with_arg_t callback, size_t arg)
{
    user_callback = callback;
    _arg = arg;
    _repeat = repeat;
    attachTickerParam(timer_id, timer_callback, microseconds, this);
}

void 
Ticker::detach()
{
    detachTicker(timer_id);
}

static void timer_callback(void* ctx)
{
    auto &driver = *reinterpret_cast<Ticker *>(ctx);
    driver.user_callback((void *)driver._arg);
    if (!driver._repeat)
        driver.detach();
}
