
#include <Wire.h>

/* I2C Test read/write AT24C02 */

void i2c_eeprom_read_page(int deviceaddress, unsigned int eeaddress, int8_t len) {
  uint8_t rdata = 0xFF;
  uint8_t eeoff = (eeaddress*len);

  eeaddress = eeoff;

  Wire.beginTransmission(deviceaddress);
  //Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  for (int i = 0; i < len; i++) {
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) {
      rdata = Wire.read();
      Serial.printf("%02x ",rdata);
    }
  }
    
  Serial.println ();
}

void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddress, int8_t len) {
  uint8_t rdata = 0;
  uint8_t eeoff = (eeaddress*len);

  eeaddress = eeoff;

  for (int i = 0; i < len; i++) {
    rdata = eeoff + i;
    //rdata = 255-rdata;
    Serial.printf("%02x ",rdata);
    
    Wire.beginTransmission(deviceaddress);
    //Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();

    eeaddress+=1;
  }
  
  Serial.println ();
}

void setup() {
  Serial.begin (115200);

  // wait for serial port to connect
  while (!Serial) 
    {
    }

  Serial.println ();
  Wire.begin();

  Serial.println ("I2C Test writing ...");
  for (int p = 0; p < 16; p++) {
    i2c_eeprom_write_page(0x50, p, 16);
  }
  
  Serial.println ("I2C Test reading ...");
  for (int p = 0; p < 16; p++) {
    i2c_eeprom_read_page(0x50, p, 16);
  }
  
  Serial.println ("Done.");

}  // end of setup

void loop() {}
