#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

// Define I2C status codes
typedef enum
{
    I2C_OK       = 0,
    I2C_ERROR    = 1,
    I2C_TIMEOUT  = 2,
    I2C_NACK     = 3
} i2c_status_t;

/**
 * @brief Initializes the I2C peripheral.
 * @param speed_hz The desired I2C bus speed in Hz.
 * @return I2C_OK on success, otherwise an error code.
 */
i2c_status_t i2c_init(uint32_t speed_hz);

/**
 * @brief Writes data to an I2C slave device.
 * @param address The 7-bit I2C slave address.
 * @param reg The register address to write to.
 * @param data Pointer to the data buffer to write.
 * @param len The number of bytes to write.
 * @param timeout_ms Timeout in milliseconds.
 * @return I2C_OK on success, otherwise an error code.
 */
i2c_status_t i2c_write(uint8_t address, uint8_t reg, const uint8_t *data, uint16_t len, uint32_t timeout_ms);

/**
 * @brief Reads data from an I2C slave device.
 * @param address The 7-bit I2C slave address.
 * @param reg The register address to read from.
 * @param data Pointer to the buffer to store read data.
 * @param len The number of bytes to read.
 * @param timeout_ms Timeout in milliseconds.
 * @return I2C_OK on success, otherwise an error code.
 */
i2c_status_t i2c_read(uint8_t address, uint8_t reg, uint8_t *data, uint16_t len, uint32_t timeout_ms);

#endif // I2C_DRIVER_H
