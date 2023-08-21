#ifndef __PI4_I2C_BUS__
#define __PI4_I2C_BUS__

#include <chrono>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <thread>
#include <sys/ioctl.h>
#include <string>
#include <unistd.h>

void delay(int64_t sleep_ms);

void log_msg(const std::string &msg);

uint64_t millis();

class I2CBusRaspberryPi4
{
public:
    I2CBusRaspberryPi4(const std::string &i2c_interface_name, const uint8_t &device_address);
    bool write(const uint8_t *buffer, size_t len);
    bool write_then_read(const uint8_t *write_buffer, size_t write_len,
                         uint8_t *read_buffer, size_t read_len);
    bool begin();
    void close();

private:
    int16_t file_descriptor_;
    uint8_t device_address_;
    std::string i2c_interface_name_;
};

#endif