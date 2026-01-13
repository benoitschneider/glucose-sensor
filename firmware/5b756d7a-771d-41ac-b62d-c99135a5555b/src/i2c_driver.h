#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Status codes for I2C driver operations.
 */
typedef enum
{
    I2C_OK = 0,         ///< Operation successful
    I2C_ERROR_NACK,     ///< No Acknowledge received
    I2C_ERROR_BUS_BUSY, ///< I2C bus is busy
    I2C_ERROR_TIMEOUT,  ///< Operation timed out
    I2C_ERROR_GENERIC   ///< Generic I2C error
} i2c_status_t;

/**
 * @brief Initializes the I2C peripheral.
 *        This function should configure the I2C pins, clock, and enable the peripheral.
 * @return i2c_status_t I2C_OK if successful, otherwise an error code.
 */
i2c_status_t i2c_driver_init(void);

/**
 * @brief Writes data to an I2C slave device.
 * 
 * @param slave_addr 7-bit I2C slave address.
 * @param data Pointer to the buffer containing data to write.
 * @param len Number of bytes to write.
 * @return i2c_status_t I2C_OK if successful, otherwise an error code.
 */
i2c_status_t i2c_driver_write(uint8_t slave_addr, const uint8_t *data, size_t len);

/**
 * @brief Reads data from an I2C slave device.
 * 
 * @param slave_addr 7-bit I2C slave address.
 * @param data Pointer to the buffer to store read data.
 * @param len Number of bytes to read.
 * @return i2c_status_t I2C_OK if successful, otherwise an error code.
 */
i2c_status_t i2c_driver_read(uint8_t slave_addr, uint8_t *data, size_t len);

/**
 * @brief Writes a register address and then reads data from an I2C slave device.
 *        This is common for devices where you first send a register address and then read from it.
 * 
 * @param slave_addr 7-bit I2C slave address.
 * @param reg_addr The register address to read from.
 * @param data Pointer to the buffer to store read data.
 * @param len Number of bytes to read.
 * @return i2c_status_t I2C_OK if successful, otherwise an error code.
 */
i2c_status_t i2c_driver_write_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, size_t len);

#endif // I2C_DRIVER_H
