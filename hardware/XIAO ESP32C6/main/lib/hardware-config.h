#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// Pin definitions based on XIAO ESP32C6 documentation
#define ANTENNA_SELECT_PIN 14
#define I2C_SDA D4
#define I2C_SCL D5

// Device configuration
#define I2C_SLAVE_ADDR 0x08
#define API_CHECK_INTERVAL 30000 // 30 seconds

// Error codes
enum class HardwareStatus {
    OK,
    WIFI_ERROR,
    I2C_ERROR,
    ANTENNA_ERROR,
    COMMAND_ERROR
};

// Function declarations only - implementations should be in hardware-impl.cpp
void internal();
void external();
void longRange();

#endif // HARDWARE_CONFIG_H
