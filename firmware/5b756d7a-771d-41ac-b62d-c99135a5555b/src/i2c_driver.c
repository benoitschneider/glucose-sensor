#include "i2c_driver.h"
#include "config.h"

// --- Mock I2C Driver Implementation ---
// This is a placeholder/mock implementation for the I2C driver.
// In a real project, this would be replaced by an MCU-specific HAL/LL driver.
// For demonstration, it simulates success and can be extended to simulate errors.

#include <stdio.h> // For printf in mock driver

// Static buffer to simulate I2C device memory for the ADS1115 config register
static uint8_t mock_ads1115_registers[4]; // Conversion, Config, Lo_thresh, Hi_thresh

/**
 * @brief Initializes the mock I2C peripheral.
 *        In a real system, this would configure GPIOs, I2C clock, etc.
 * @return i2c_status_t I2C_OK always for mock.
 */
i2c_status_t i2c_driver_init(void)
{
    // In a real system: configure I2C pins, clock, enable peripheral.
    // For mock, just print a message.
    printf("Mock I2C Driver: Initialized for peripheral instance %s at %lu Hz.\n", 
           "I2C1", (unsigned long)I2C_CLOCK_SPEED);
    
    // Initialize mock ADS1115 registers to default values (e.g., power-up defaults)
    mock_ads1115_registers[0] = 0x00; // Conversion register
    mock_ads1115_registers[1] = 0x85; // Config MSB (OS=1, MUX=000, PGA=010)
    mock_ads1115_registers[2] = 0x83; // Config LSB (MODE=1, DR=100, COMP_MODE=0, COMP_POL=0, COMP_LAT=0, COMP_QUE=11)
    mock_ads1115_registers[3] = 0x00; // Lo_thresh
    mock_ads1115_registers[4] = 0x00; // Hi_thresh

    return I2C_OK;
}

/**
 * @brief Writes data to a mock I2C slave device.
 * 
 * @param slave_addr 7-bit I2C slave address.
 * @param data Pointer to the buffer containing data to write.
 * @param len Number of bytes to write.
 * @return i2c_status_t I2C_OK always for mock, or I2C_ERROR_NACK if slave_addr is invalid.
 */
i2c_status_t i2c_driver_write(uint8_t slave_addr, const uint8_t *data, size_t len)
{
    if (slave_addr != ADS1115_I2C_ADDRESS)
    {
        printf("Mock I2C Driver: Write to unknown slave address 0x%02X.\n", slave_addr);
        return I2C_ERROR_NACK;
    }
    if (len < 1) return I2C_ERROR_GENERIC; // Need at least a register address

    uint8_t reg_ptr = data[0]; // First byte is the register pointer

    printf("Mock I2C Driver: Writing %zu bytes to ADS1115 (0x%02X), Reg 0x%02X. Data: ", len -1, slave_addr, reg_ptr);
    for (size_t i = 1; i < len; ++i)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\n");

    // Simulate writing to ADS1115 registers
    if (reg_ptr == 0x01 && len == 3) // Configuration register (0x01) expects 2 data bytes
    {
        mock_ads1115_registers[1] = data[1]; // MSB
        mock_ads1115_registers[2] = data[2]; // LSB
        printf("Mock I2C Driver: ADS1115 Config Register updated to 0x%02X%02X.\n", 
               mock_ads1115_registers[1], mock_ads1115_registers[2]);
    }
    else if (reg_ptr < sizeof(mock_ads1115_registers) && len == 1) {
        // Single byte write (e.g., just setting pointer for subsequent read)
        // No data written to register in this case, just pointer set
    }
    else if (reg_ptr < sizeof(mock_ads1115_registers) && (len - 1) == 2) {
        // For Lo_thresh (0x02) or Hi_thresh (0x03) registers
        mock_ads1115_registers[reg_ptr] = data[1]; // MSB
        mock_ads1115_registers[reg_ptr+1] = data[2]; // LSB (assuming 16-bit registers)
        printf("Mock I2C Driver: ADS1115 Reg 0x%02X updated to 0x%02X%02X.\n", 
               reg_ptr, mock_ads1115_registers[reg_ptr], mock_ads1115_registers[reg_ptr+1]);
    }
    else {
        printf("Mock I2C Driver: Unsupported ADS1115 register write or length. Reg: 0x%02X, Len: %zu.\n", reg_ptr, len);
    }

    return I2C_OK;
}

/**
 * @brief Reads data from a mock I2C slave device.
 * 
 * @param slave_addr 7-bit I2C slave address.
 * @param data Pointer to the buffer to store read data.
 * @param len Number of bytes to read.
 * @return i2c_status_t I2C_OK always for mock.
 */
i2c_status_t i2c_driver_read(uint8_t slave_addr, uint8_t *data, size_t len)
{
    if (slave_addr != ADS1115_I2C_ADDRESS)
    {
        printf("Mock I2C Driver: Read from unknown slave address 0x%02X.\n", slave_addr);
        return I2C_ERROR_NACK;
    }
    if (len == 0) return I2C_OK; // Nothing to read

    printf("Mock I2C Driver: Reading %zu bytes from ADS1115 (0x%02X). (Not implemented for specific register yet)\n", len, slave_addr);
    // For a full mock, you'd need to track the current register pointer
    // and return data from mock_ads1115_registers accordingly.
    // For now, return dummy data.
    for(size_t i=0; i<len; ++i) {
        data[i] = 0xAA; // Dummy data
    }

    return I2C_OK;
}

/**
 * @brief Writes a register address and then reads data from a mock I2C slave device.
 * 
 * @param slave_addr 7-bit I2C slave address.
 * @param reg_addr The register address to read from.
 * @param data Pointer to the buffer to store read data.
 * @param len Number of bytes to read.
 * @return i2c_status_t I2C_OK always for mock.
 */
i2c_status_t i2c_driver_write_read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, size_t len)
{
    if (slave_addr != ADS1115_I2C_ADDRESS)
    {
        printf("Mock I2C Driver: Write-Read to unknown slave address 0x%02X.\n", slave_addr);
        return I2C_ERROR_NACK;
    }
    if (len == 0) return I2C_OK; // Nothing to read

    printf("Mock I2C Driver: Write-Read ADS1115 (0x%02X), Reg 0x%02X, Reading %zu bytes. Data: ", slave_addr, reg_addr, len);

    // Simulate reading from ADS1115 registers
    if (reg_addr == 0x00 && len == 2) // Conversion register
    {
        // Simulate a conversion result (e.g., 0x1234)
        data[0] = 0x12; // MSB
        data[1] = 0x34; // LSB
        printf("0x%02X 0x%02X\n", data[0], data[1]);
    }
    else if (reg_addr == 0x01 && len == 2) // Configuration register
    {
        data[0] = mock_ads1115_registers[1]; // MSB
        data[1] = mock_ads1115_registers[2]; // LSB
        printf("0x%02X 0x%02X (Current Config)\n", data[0], data[1]);
    }
    else if (reg_addr == 0x02 && len == 2) // Lo_thresh register
    {
        data[0] = mock_ads1115_registers[3]; // MSB
        data[1] = mock_ads1115_registers[4]; // LSB
        printf("0x%02X 0x%02X (Lo_thresh)\n", data[0], data[1]);
    }
    else if (reg_addr == 0x03 && len == 2) // Hi_thresh register
    {
        data[0] = mock_ads1115_registers[5]; // MSB
        data[1] = mock_ads1115_registers[6]; // LSB
        printf("0x%02X 0x%02X (Hi_thresh)\n", data[0], data[1]);
    }
    else
    {
        // For other cases, return dummy data
        for(size_t i=0; i<len; ++i) {
            data[i] = 0xCC; // Dummy data
        }
        printf("Dummy data\n");
    }

    return I2C_OK;
}
