#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// Define an enumeration for I2C driver status codes
typedef enum
{
    I2C_OK = 0,
    I2C_ERROR,
    I2C_NACK,
    I2C_TIMEOUT,
    I2C_BUS_BUSY,
    I2C_INVALID_PARAM
} I2C_Status_t;

/**
 * @brief Initializes the I2C peripheral.
 *        This function should typically be called once at system startup.
 *        Actual hardware initialization is assumed to be done by the MCU's HAL layer
 *        before this driver is used (e.g., MX_I2C1_Init() for STM32CubeMX).
 * @return I2C_Status_t status of the initialization.
 */
I2C_Status_t I2C_Driver_Init(void);

/**
 * @brief Writes a single byte to an I2C slave device.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param data The byte to write.
 * @return I2C_Status_t status of the write operation.
 */
I2C_Status_t I2C_WriteByte(uint8_t slave_address, uint8_t data);

/**
 * @brief Reads a single byte from an I2C slave device at a specific register address.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param register_addr The register address to read from.
 * @param data Pointer to store the read byte.
 * @return I2C_Status_t status of the read operation.
 */
I2C_Status_t I2C_ReadByte(uint8_t slave_address, uint8_t register_addr, uint8_t *data);

/**
 * @brief Writes multiple bytes to an I2C slave device.
 *        This function typically writes to a starting register address and increments.
 *        The first byte in data_buffer is often the register address.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param data_buffer Pointer to the buffer containing data to write.
 * @param length The number of bytes to write.
 * @return I2C_Status_t status of the write operation.
 */
I2C_Status_t I2C_WriteMulti(uint8_t slave_address, const uint8_t *data_buffer, uint16_t length);

/**
 * @brief Reads multiple bytes from an I2C slave device, starting from a specific register address.
 *        This typically involves a combined write (register address) and then a read.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param register_addr The starting register address to read from.
 * @param data_buffer Pointer to the buffer to store the read data.
 * @param length The number of bytes to read.
 * @return I2C_Status_t status of the read operation.
 */
I2C_Status_t I2C_ReadMulti(uint8_t slave_address, uint8_t register_addr, uint8_t *data_buffer, uint16_t length);

#endif // I2C_DRIVER_H
