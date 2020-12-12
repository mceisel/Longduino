#ifndef TwoWire_h
#define TwoWire_h

#include "Arduino.h"
#include "Stream.h"
#include "RingBuffer.h"

#define I2C_BUFFER_LENGTH 128

#define I2C_CON_SPEED_STANDARD                        1 // <=100Kbit/s
#define I2C_CON_SPEED_FAST                            2 // <=400Kbit/s or <=1000Kbit/s
#define I2C_CON_SPEED_HIGH                            3 // <=3.4Mbit/s

#define I2C0_SDA PB7
#define I2C0_SCL PB6

typedef struct _i2c_dev_t
{
    uint32_t i2c_dev;
    rcu_periph_enum clk_id;
    uint8_t sda;
    uint8_t scl;
    uint8_t smba;
} i2c_dev_t;

typedef enum _i2c_device_number
{
    I2C_DEVICE_0,
    I2C_DEVICE_1,
    I2C_DEVICE_MAX,
} i2c_device_number_t;

typedef void(*user_onRequest)(void);
typedef void(*user_onReceive)(uint8_t*, int);

class TwoWire : public Stream
{
public:
    i2c_device_number_t _i2c_num;

    TwoWire(i2c_device_number_t i2c_device);
    ~TwoWire();
    void begin(uint8_t sda = I2C0_SDA, uint8_t scl = I2C0_SCL, uint32_t frequency = 400000);
    void begin(uint16_t slave_address, uint8_t sda = I2C0_SDA, uint8_t scl = I2C0_SCL);

    void setClock(uint32_t frequency); 
    uint32_t getClock();

    void setTimeOut(uint16_t timeOutMillis);
    uint16_t getTimeOut();

    int writeTransmission(uint16_t address, uint8_t* send_buf, size_t send_buf_len, bool sendStop);
    int readTransmission(uint16_t address, uint8_t* receive_buf, size_t receive_buf_len, bool sendStop);

    void beginTransmission(uint16_t address);
    void beginTransmission(uint8_t  address);
    void beginTransmission(int      address);

    uint8_t endTransmission(bool sendStop);
    uint8_t endTransmission(void);

    uint8_t requestFrom(uint16_t address, uint8_t size, bool sendStop);
    uint8_t requestFrom(uint16_t address, uint8_t size, uint8_t sendStop);
    uint8_t requestFrom(uint16_t address, uint8_t size);
    uint8_t requestFrom(uint8_t address, uint8_t size, uint8_t sendStop);
    uint8_t requestFrom(uint8_t address, uint8_t size);
    uint8_t requestFrom(int address, int size, int sendStop);
    uint8_t requestFrom(int address, int size);

    size_t write(uint8_t);
    size_t write(const uint8_t *, int);
    int available(void);
    int read(void);
    int peek(void);
    void flush(void);

    inline int write(const char * s)
    {
        return write((uint8_t*) s, strlen(s));
    }
    inline int write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline int write(long n)
    {
        return write((uint8_t)n);
    }
    inline int write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline int write(int n)
    {
        return write((uint8_t)n);
    }

    bool busy();

    void scan();

private:
    const i2c_dev_t *_dev;
    uint16_t i2c_slave_address;
    bool is_master_mode = false;
    uint32_t address_width = 7;
    uint8_t sda = VARIANT_GPIO_NUM;
    uint8_t scl = VARIANT_GPIO_NUM;
    uint32_t i2c_clk;

    RingBuffer *i2c_tx_buff;
    RingBuffer *i2c_rx_buff;

    uint16_t txAddress;
    uint8_t transmitting;

    uint16_t _timeOutMillis;

};



extern TwoWire Wire;
extern TwoWire Wire1;

#endif //TwoWire_h
