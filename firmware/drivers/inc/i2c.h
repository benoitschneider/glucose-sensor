#ifndef I2C_H
#define I2C_H

#include <stdint.h>

// Define I2C return codes
typedef enum {
    I2C_SUCCESS = 0,
    I2C_ERROR_BUS_BUSY,
    I2C_ERROR_NACK,
    I2C_ERROR_TIMEOUT,
    I2C_ERROR_INVALID_PARAM,
    I2C_ERROR_OTHER
} i2c_ret_code_t;

/**
 * @brief Initializes the I2C peripheral.
 * @param sda_pin The GPIO pin for I2C data.
 * @param scl_pin The GPIO pin for I2C clock.
 * @param frequency The I2C bus frequency in Hz (e.g., 100000 for 100kHz).
 * @return I2C_SUCCESS on success, otherwise an error code.
 */
i2c_ret_code_t i2c_init(uint32_t sda_pin, uint32_t scl_pin, uint32_t frequency);

/**
 * @brief Writes data to an I2C slave device.
 * @param address The 7-bit I2C slave address.
 * @param data Pointer to the data buffer to write.
 * @param len The number of bytes to write.
 * @param no_stop If true, a STOP condition is not generated after writing (for repeated start).
 * @return I2C_SUCCESS on success, otherwise an error code.
 */
i2c_ret_code_t i2c_write(uint8_t address, const uint8_t *data, uint8_t len, bool no_stop);

/**
 * @brief Reads data from an I2C slave device.
 * @param address The 7-bit I2C slave address.
 * @param data Pointer to the buffer to store the read data.
 * @param len The number of bytes to read.
 * @return I2C_SUCCESS on success, otherwise an error code.
 */
i2c_ret_code_t i2c_read(uint8_t address, uint8_t *data, uint8_t len);

#endif // I2C_H
