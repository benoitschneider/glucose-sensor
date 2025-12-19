#ifndef CONFIG_H
#define CONFIG_H

// Define the I2C peripheral base address or handle type based on your MCU HAL
// For STM32, this might be a pointer to an I2C_HandleTypeDef struct
// For other MCUs, it could be a base address or a driver instance pointer.
// This example assumes a generic 'I2C_Handle_t' type is defined by the HAL.
#define I2C_MASTER_HANDLE           (&hi2c1) // Example: replace with actual I2C handle

// I2C Timeout in milliseconds
#define I2C_TIMEOUT_MS              100

// ADS1115 I2C Slave Address
// ADDR pin connected to GND: 0x48 (1001000)
// ADDR pin connected to VDD: 0x49 (1001001)
// ADDR pin connected to SDA: 0x4A (1001010)
// ADDR pin connected to SCL: 0x4B (1001011)
#define ADS1115_I2C_ADDRESS         (0x48 << 1) // 7-bit address 0x48, left-shifted for 8-bit R/W byte

#endif // CONFIG_H
