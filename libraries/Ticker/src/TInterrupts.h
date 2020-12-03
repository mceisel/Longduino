#ifdef __cplusplus
extern "C"{
#endif

#include "pins_arduino.h"

void attachTicker(uint8_t interruptNumber, voidFuncPtr callback, uint32_t microseconds);
void attachTickerParam(uint8_t interruptNumber, voidFuncPtrParam callback, uint32_t microseconds, void* param);
void detachTicker(uint8_t interruptNumber);

#define attachTickerArg(p,c,a,m) attachTickerParam(p,c,m,a)

#ifdef __cplusplus
} // extern "C"
#endif
