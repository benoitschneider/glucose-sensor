#include "i2c_driver.h"
#include "config.h"

// Include your MCU's specific I2C HAL header here
// Example for STM32Cube HAL:
#include "stm32l4xx_hal.h" // Adjust based on your specific MCU family

// Extern declaration for the I2C handle, typically defined in main.c or a HAL file
// This assumes your HAL setup provides a global handle like 'hi2c1'
extern I2C_HandleTypeDef hi2c1; 

/**
 * @brief Initializes the I2C peripheral.
 *        This function assumes the HAL layer has already configured the I2C peripheral.
 *        It primarily serves as a placeholder for any driver-specific setup or checks.
 * @return I2C_Status_t status of the initialization.
 */
I2C_Status_t I2C_Driver_Init(void)
{
    // In a typical HAL environment (e.g., STM32CubeMX), the hardware I2C peripheral
    // is initialized by generated code (e.g., MX_I2C1_Init()).
    // This function can be used for any post-HAL initialization or checks.
    // For example, checking if the peripheral is ready.
    if (HAL_I2C_GetState(I2C_MASTER_HANDLE) == HAL_I2C_STATE_RESET ||
        HAL_I2C_GetState(I2C_MASTER_HANDLE) == HAL_I2C_STATE_ERROR)
    {
        // Attempt to re-initialize if in error or reset state, or just return error
        // This might require calling the HAL_I2C_Init() function again.
        // For simplicity, we assume it's correctly initialized by the system setup.
        return I2C_ERROR;
    }
    return I2C_OK;
}

/**
 * @brief Writes a single byte to an I2C slave device.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param data The byte to write.
 * @return I2C_Status_t status of the write operation.
 */
I2C_Status_t I2C_WriteByte(uint8_t slave_address, uint8_t data)
{
    HAL_StatusTypeDef hal_status;
    uint8_t tx_data[1] = {data};

    // Check for valid parameters
    if (slave_address == 0 || slave_address > 0x7F) return I2C_INVALID_PARAM;

    // Use HAL_I2C_Master_Transmit for writing data
    // slave_address is left-shifted by 1 to include R/W bit
    hal_status = HAL_I2C_Master_Transmit(I2C_MASTER_HANDLE, (uint16_t)(slave_address << 1), tx_data, 1, I2C_TIMEOUT_MS);

    if (hal_status == HAL_OK)
    {
        return I2C_OK;
    }
    else if (hal_status == HAL_ERROR)
    {
        // Check for specific I2C errors like NACK
        if (HAL_I2C_GetError(I2C_MASTER_HANDLE) & HAL_I2C_ERROR_AF) // Acknowledge Failure
        {
            return I2C_NACK;
        }
        return I2C_ERROR;
    }
    else if (hal_status == HAL_TIMEOUT)
    {
        return I2C_TIMEOUT;
    }
    else if (hal_status == HAL_BUSY)
    {
        return I2C_BUS_BUSY;
    }
    return I2C_ERROR; // Should not reach here
}

/**
 * @brief Reads a single byte from an I2C slave device at a specific register address.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param register_addr The register address to read from.
 * @param data Pointer to store the read byte.
 * @return I2C_Status_t status of the read operation.
 */
I2C_Status_t I2C_ReadByte(uint8_t slave_address, uint8_t register_addr, uint8_t *data)
{
    HAL_StatusTypeDef hal_status;

    // Check for valid parameters
    if (slave_address == 0 || slave_address > 0x7F || data == NULL) return I2C_INVALID_PARAM;

    // First, transmit the register address to read from
    // Use HAL_I2C_Master_Transmit for sending register address
    hal_status = HAL_I2C_Master_Transmit(I2C_MASTER_HANDLE, (uint16_t)(slave_address << 1), &register_addr, 1, I2C_TIMEOUT_MS);
    if (hal_status != HAL_OK)
    {
        if (hal_status == HAL_ERROR && (HAL_I2C_GetError(I2C_MASTER_HANDLE) & HAL_I2C_ERROR_AF)) return I2C_NACK;
        if (hal_status == HAL_TIMEOUT) return I2C_TIMEOUT;
        if (hal_status == HAL_BUSY) return I2C_BUS_BUSY;
        return I2C_ERROR;
    }

    // Then, receive the data from the slave
    // Use HAL_I2C_Master_Receive for reading data
    hal_status = HAL_I2C_Master_Receive(I2C_MASTER_HANDLE, (uint16_t)(slave_address << 1), data, 1, I2C_TIMEOUT_MS);

    if (hal_status == HAL_OK)
    {
        return I2C_OK;
    }
    else if (hal_status == HAL_ERROR)
    {
        if (HAL_I2C_GetError(I2C_MASTER_HANDLE) & HAL_I2C_ERROR_AF) return I2C_NACK;
        return I2C_ERROR;
    }
    else if (hal_status == HAL_TIMEOUT)
    {
        return I2C_TIMEOUT;
    }
    else if (hal_status == HAL_BUSY)
    {
        return I2C_BUS_BUSY;
    }
    return I2C_ERROR;
}

/**
 * @brief Writes multiple bytes to an I2C slave device.
 *        This function typically writes to a starting register address and increments.
 *        The first byte in data_buffer is often the register address, followed by data.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param data_buffer Pointer to the buffer containing data to write.
 * @param length The number of bytes to write.
 * @return I2C_Status_t status of the write operation.
 */
I2C_Status_t I2C_WriteMulti(uint8_t slave_address, const uint8_t *data_buffer, uint16_t length)
{
    HAL_StatusTypeDef hal_status;

    // Check for valid parameters
    if (slave_address == 0 || slave_address > 0x7F || data_buffer == NULL || length == 0) return I2C_INVALID_PARAM;

    // Use HAL_I2C_Master_Transmit for writing multiple bytes
    hal_status = HAL_I2C_Master_Transmit(I2C_MASTER_HANDLE, (uint16_t)(slave_address << 1), (uint8_t*)data_buffer, length, I2C_TIMEOUT_MS);

    if (hal_status == HAL_OK)
    {
        return I2C_OK;
    }
    else if (hal_status == HAL_ERROR)
    {
        if (HAL_I2C_GetError(I2C_MASTER_HANDLE) & HAL_I2C_ERROR_AF) return I2C_NACK;
        return I2C_ERROR;
    }
    else if (hal_status == HAL_TIMEOUT)
    {
        return I2C_TIMEOUT;
    }
    else if (hal_status == HAL_BUSY)
    {
        return I2C_BUS_BUSY;
    }
    return I2C_ERROR;
}

/**
 * @brief Reads multiple bytes from an I2C slave device, starting from a specific register address.
 *        This typically involves a combined write (register address) and then a read.
 * @param slave_address The 7-bit I2C slave address (not shifted).
 * @param register_addr The starting register address to read from.
 * @param data_buffer Pointer to the buffer to store the read data.
 * @param length The number of bytes to read.
 * @return I2C_Status_t status of the read operation.
 */
I2C_Status_t I2C_ReadMulti(uint8_t slave_address, uint8_t register_addr, uint8_t *data_buffer, uint16_t length)
{
    HAL_StatusTypeDef hal_status;

    // Check for valid parameters
    if (slave_address == 0 || slave_address > 0x7F || data_buffer == NULL || length == 0) return I2C_INVALID_PARAM;

    // First, transmit the register address to read from
    // Use HAL_I2C_Master_Transmit for sending register address
    hal_status = HAL_I2C_Master_Transmit(I2C_MASTER_HANDLE, (uint16_t)(slave_address << 1), &register_addr, 1, I2C_TIMEOUT_MS);
    if (hal_status != HAL_OK)
    {
        if (hal_status == HAL_ERROR && (HAL_I2C_GetError(I2C_MASTER_HANDLE) & HAL_I2C_ERROR_AF)) return I2C_NACK;
        if (hal_status == HAL_TIMEOUT) return I2C_TIMEOUT;
        if (hal_status == HAL_BUSY) return I2C_BUS_BUSY;
        return I2C_ERROR;
    }

    // Then, receive multiple bytes from the slave
    // Use HAL_I2C_Master_Receive for reading data
    hal_status = HAL_I2C_Master_Receive(I2C_MASTER_HANDLE, (uint16_t)(slave_address << 1), data_buffer, length, I2C_TIMEOUT_MS);

    if (hal_status == HAL_OK)
    {
        return I2C_OK;
    }
    else if (hal_status == HAL_ERROR)
    {
        if (HAL_I2C_GetError(I2C_MASTER_HANDLE) & HAL_I2C_ERROR_AF) return I2C_NACK;
        return I2C_ERROR;
    }
    else if (hal_status == HAL_TIMEOUT)
    {
        return I2C_TIMEOUT;
    }
    else if (hal_status == HAL_BUSY)
    {
        return I2C_BUS_BUSY;
    }
    return I2C_ERROR;
}
