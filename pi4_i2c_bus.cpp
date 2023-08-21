#include "pi4_i2c_bus.h"

void delay(int64_t sleep_ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
}

void log_msg(const std::string &msg)
{
    std::cout << msg << std::endl;
}

uint64_t millis()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
    return ms.count();
}

I2CBusRaspberryPi4::I2CBusRaspberryPi4(const std::string &i2c_interface_name, const uint8_t &device_address) : i2c_interface_name_(i2c_interface_name),
                                                                                                               device_address_(device_address)
{
}

bool I2CBusRaspberryPi4::write(const uint8_t *buffer, size_t len)
{
    i2c_msg messages[1] = {
        {device_address_, 0, (typeof(i2c_msg().len))len, (typeof(i2c_msg().buf))buffer}};
    i2c_rdwr_ioctl_data ioctl_data = {messages, 1};

    int result = ioctl(file_descriptor_, I2C_RDWR, &ioctl_data);

    if (result != 1)
    {
        log_msg("Failed to write to " + std::to_string(device_address_));
        return false;
    }
    return true;
}
bool I2CBusRaspberryPi4::write_then_read(const uint8_t *write_buffer, size_t write_len,
                                         uint8_t *read_buffer, size_t read_len)
{
    i2c_msg messages[2] = {
        {device_address_, 0, (typeof(i2c_msg().len))write_len, (typeof(i2c_msg().buf))write_buffer},
        {device_address_, I2C_M_RD, (typeof(i2c_msg().len))read_len, (typeof(i2c_msg().buf))read_buffer},
    };
    i2c_rdwr_ioctl_data ioctl_data = {messages, 2};

    int result = ioctl(file_descriptor_, I2C_RDWR, &ioctl_data);

    if (result != 2)
    {
        log_msg("Failed to write then read to " + std::to_string(device_address_));
        return false;
    }
    return true;
}

bool I2CBusRaspberryPi4::begin()
{
    close();
    file_descriptor_ = ::open(i2c_interface_name_.c_str(), O_RDWR);
    if (file_descriptor_ == -1)
    {
        log_msg("Failed to open I2C device " + i2c_interface_name_);
        return false;
    }
    return true;
}

void I2CBusRaspberryPi4::close()
{
    if (file_descriptor_ != -1)
    {
        ::close(file_descriptor_);
        file_descriptor_ = -1;
    }
}