#include "i2c_driver.h"
#include "config.h"
#include <stddef.h>

// --- Placeholder for MCU-specific I2C peripheral access ---
// In a real application, these would be replaced by actual MCU HAL calls.
// For demonstration, we'll simulate success.

/**
 * @brief Initializes the I2C peripheral.
 * @param speed_hz The desired I2C bus speed in Hz.
 * @return I2C_OK on success, otherwise an error code.
 */
i2c_status_t i2c_init(uint32_t speed_hz)
{
    // Simulate MCU-specific I2C peripheral initialization
    // e.g., enable clock, configure GPIOs, set speed, enable peripheral
    (void)speed_hz; // Suppress unused parameter warning
    // printf("I2C initialized at %lu Hz\n", speed_hz); // For debugging
    return I2C_OK;
}

/**
 * @brief Writes data to an I2C slave device.
 * @param address The 7-bit I2C slave address.
 * @param reg The register address to write to.
 * @param data Pointer to the data buffer to write.
 * @param len The number of bytes to write.
 * @param timeout_ms Timeout in milliseconds.
 * @return I2C_OK on success, otherwise an error code.
 */
i2c_status_t i2c_write(uint8_t address, uint8_t reg, const uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    if (data == NULL || len == 0) {
        return I2C_ERROR; // Invalid parameters
    }

    // Simulate MCU-specific I2C write sequence:
    // 1. Start condition
    // 2. Send slave address + write bit
    // 3. Send register address
    // 4. Send data bytes
    // 5. Stop condition

    // For demonstration purposes, we just return success.
    // In a real system, this would involve checking ACKs and handling NACKs/timeouts.
    (void)address;    // Suppress unused parameter warning
    (void)reg;        // Suppress unused parameter warning
    (void)data;       // Suppress unused parameter warning
    (void)len;        // Suppress unused parameter warning
    (void)timeout_ms; // Suppress unused parameter warning

    // printf("I2C Write: Addr=0x%02X, Reg=0x%02X, Len=%u\n", address, reg, len); // For debugging

    // Simulate a delay for I2C transaction
    // For a real system, this would be handled by hardware peripheral and interrupts/polling
    for (volatile int i = 0; i < 100 * len; ++i); // Simple blocking delay

    return I2C_OK;
}

/**
 * @brief Reads data from an I2C slave device.
 * @param address The 7-bit I2C slave address.
 * @param reg The register address to read from.
 * @param data Pointer to the buffer to store read data.
 * @param len The number of bytes to read.
 * @param timeout_ms Timeout in milliseconds.
 * @return I2C_OK on success, otherwise an error code.
 */
i2c_status_t i2c_read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len, uint32_t timeout_ms)
{
    if (data == NULL || len == 0) {
        return I2C_ERROR; // Invalid parameters
    }

    // Simulate MCU-specific I2C read sequence:
    // 1. Start condition
    // 2. Send slave address + write bit
    // 3. Send register address
    // 4. Repeated start condition
    // 5. Send slave address + read bit
    // 6. Read data bytes (ACK for all but last, NACK for last)
    // 7. Stop condition

    // For demonstration purposes, we just return success and fill with dummy data.
    (void)address;    // Suppress unused parameter warning
    (void)reg;        // Suppress unused parameter warning
    (void)timeout_ms; // Suppress unused parameter warning

    // printf("I2C Read: Addr=0x%02X, Reg=0x%02X, Len=%u\n", address, reg, len); // For debugging

    // Simulate reading data (e.g., 0x12, 0x34 for a 2-byte read)
    for (uint16_t i = 0; i < len; ++i) {
        data[i] = (uint8_t)(0x12 + i);
    }

    // Simulate a delay for I2C transaction
    for (volatile int i = 0; i < 100 * len; ++i); // Simple blocking delay

    return I2C_OK;
}
