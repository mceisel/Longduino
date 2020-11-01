/*
  SD card file dump

 This example shows how to read a file from the SD card using the
 SD library and send it over the serial port.

 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin PB15
 ** MISO - pin PB14
 ** CLK  - pin PB13
 ** CS   - pin PB12 (Longan Nano) or PB1 (Wio Lite RISC-V)

 created  22 December 2010
 by Limor Fried
 modified 9 Apr 2012
 by Tom Igoe
 modified for gd32v 31 Oct 2020
 by SCP
 
 This example code is in the public domain.

 */

#include <SPI.h>
#include <SD.h>

const int chipSelect = SDCARD_SS_PIN;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loop() {
}

