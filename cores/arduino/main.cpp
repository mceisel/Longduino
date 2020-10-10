#define ARDUINO_MAIN
#include "Arduino.h"
#include "ArduinoAPI.h"
#include "Common.h"
#include "gd32vf103_libopt.h"


#define cmb() __asm__ __volatile__ ("" ::: "memory")
extern "C"{ void * __dso_handle = 0 ;}

/*
 * \brief Main entry point of Arduino application
 */
int main( void )
{
    Serial.begin(115200);

    setup();
  
    do {
        loop();
        //if (serialEventRun)
        //  serialEventRun();
    } while (1);

    return 0;
}
