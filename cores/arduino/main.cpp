#define ARDUINO_MAIN
#include "Arduino.h"
#include "ArduinoAPI.h"
#include "Common.h"
#include "gd32vf103_libopt.h"
#include "usart_com.h"


#define cmb() __asm__ __volatile__ ("" ::: "memory")
extern "C"{ void * __dso_handle = 0 ;}

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
    usart_com_init(USART0, USART_WL_8BIT, 115200U);

    setup();
  
    do {
        loop();
        //if (serialEventRun)
        //  serialEventRun();
    } while (1);

    return 0;
}
