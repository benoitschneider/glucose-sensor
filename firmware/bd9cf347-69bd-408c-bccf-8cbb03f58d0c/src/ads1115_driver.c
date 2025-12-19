#include "ads1115_driver.h"
#include "config.h"

// Static variable to store the current configuration register value
static uint16_t current_config_reg = 0x0000; // Default power-up value is 0x8583, but we'll manage it dynamically

/**
 * @brief Writes a 16-bit value to an ADS1115 register.
 * @param reg_addr The 8-bit register address.
 * @param value The 16-bit value to write.
 * @return ADS1115_Status_t status of the write operation.
 */
static ADS1115_Status_t ADS1115_WriteRegister(uint8_t reg_addr, uint16_t value)
{
    uint8_t tx_buffer[3];
    tx_buffer[0] = reg_addr;        // Register address
    tx_buffer[1] = (uint8_t)(value >> 8); // MSB
    tx_buffer[2] = (uint8_t)(value & 0xFF); // LSB

    // I2C_WriteMulti expects the first byte of data_buffer to be the register address
    // and then the data bytes. Our I2C_WriteMulti function handles this pattern.
    I2C_Status_t i2c_status = I2C_WriteMulti(ADS1115_I2C_ADDRESS >> 1, tx_buffer, 3);

    if (i2c_status != I2C_OK)
    {
        // Map I2C errors to ADS1115 errors
        if (i2c_status == I2C_NACK) return ADS1115_I2C_ERROR; // NACK often indicates device not present
        return ADS1115_I2C_ERROR;
    }
    return ADS1115_OK;
}

/**
 * @brief Reads a 16-bit value from an ADS1115 register.
 * @param reg_addr The 8-bit register address.
 * @param value Pointer to store the 16-bit read value.
 * @return ADS1115_Status_t status of the read operation.
 */
static ADS1115_Status_t ADS1115_ReadRegister(uint8_t reg_addr, uint16_t *value)
{
    uint8_t rx_buffer[2];

    // I2C_ReadMulti sends the register address and then reads the specified number of bytes.
    I2C_Status_t i2c_status = I2C_ReadMulti(ADS1115_I2C_ADDRESS >> 1, reg_addr, rx_buffer, 2);

    if (i2c_status != I2C_OK)
    {
        if (i2c_status == I2C_NACK) return ADS1115_I2C_ERROR;
        return ADS1115_I2C_ERROR;
    }

    // Combine MSB and LSB to form the 16-bit value
    *value = ((uint16_t)rx_buffer[0] << 8) | rx_buffer[1];
    return ADS1115_OK;
}

/**
 * @brief Initializes the ADS1115 driver and performs a basic device check.
 * @return ADS1115_Status_t status of the initialization.
 */
ADS1115_Status_t ADS1115_Init(void)
{
    // Initialize the underlying I2C driver first
    if (I2C_Driver_Init() != I2C_OK)
    {
        return ADS1115_I2C_ERROR;
    }

    // Perform a dummy read to check if the device responds
    // Reading the config register is a good way to check for ACK
    uint16_t read_config_val;
    ADS1115_Status_t status = ADS1115_ReadRegister(ADS1115_REG_CONFIG, &read_config_val);
    if (status != ADS1115_OK)
    {
        return status; // Device not responding or I2C error
    }

    // Store a default configuration (e.g., single-shot, AIN0/GND, PGA +/-2.048V, 128SPS)
    // This is a common starting point. User can call ADS1115_Configure later.
    current_config_reg = ADS1115_CONFIG_OS_START | ADS1115_MUX_P0_NG | ADS1115_PGA_2_048V | 
                         ADS1115_MODE_SINGLESHOT | ADS1115_DR_128SPS | 
                         ADS1115_COMP_MODE_TRADITIONAL | ADS1115_COMP_POL_ACTIVE_LOW | 
                         ADS1115_COMP_LAT_NONLATCHING | ADS1115_COMP_QUE_DISABLE;
    
    // Write the initial configuration to the device
    status = ADS1115_WriteRegister(ADS1115_REG_CONFIG, current_config_reg);
    if (status != ADS1115_OK)
    {
        return status;
    }

    return ADS1115_OK;
}

/**
 * @brief Configures the ADS1115 with the specified settings.
 * @param mux Input multiplexer configuration.
 * @param pga Programmable gain amplifier setting.
 * @param data_rate Samples per second data rate.
 * @param operating_mode ADS1115_MODE_SINGLESHOT or ADS1115_MODE_CONTINUOUS.
 * @return ADS1115_Status_t status of the configuration operation.
 */
ADS1115_Status_t ADS1115_Configure(
    ADS1115_Mux_t mux,
    ADS1115_Pga_t pga,
    ADS1115_DataRate_t data_rate,
    uint16_t operating_mode)
{
    // Clear relevant bits in the current_config_reg and set new ones
    // OS bit (bit 15) is handled by StartConversion, so we don't touch it here.
    // Keep comparator settings as default for now or add parameters for them.
    current_config_reg = (current_config_reg & ~(0x7800 | 0x0E00 | 0x01E0 | 0x0100)) | 
                         mux | pga | data_rate | operating_mode;

    // Write the new configuration to the device
    ADS1115_Status_t status = ADS1115_WriteRegister(ADS1115_REG_CONFIG, current_config_reg);
    return status;
}

/**
 * @brief Starts a single-shot conversion on the ADS1115.
 *        This function only works if the device is configured in single-shot mode.
 * @return ADS1115_Status_t status of the start operation.
 */
ADS1115_Status_t ADS1115_StartConversion(void)
{
    // Ensure the device is in single-shot mode before attempting to start a conversion
    if ((current_config_reg & ADS1115_MODE_SINGLESHOT) == 0) {
        // Device is in continuous conversion mode, no explicit start needed
        return ADS1115_OK;
    }

    // Set the OS bit (bit 15) to start a single conversion
    uint16_t config_to_write = current_config_reg | ADS1115_CONFIG_OS_START;
    ADS1115_Status_t status = ADS1115_WriteRegister(ADS1115_REG_CONFIG, config_to_write);
    return status;
}

/**
 * @brief Checks if the ADS1115 conversion is complete.
 * @param conversion_complete Pointer to a boolean to store the status.
 * @return ADS1115_Status_t status of the check operation.
 */
ADS1115_Status_t ADS1115_IsConversionComplete(bool *conversion_complete)
{
    if (conversion_complete == NULL) return ADS1115_INVALID_PARAM;

    uint16_t config_read;
    ADS1115_Status_t status = ADS1115_ReadRegister(ADS1115_REG_CONFIG, &config_read);
    if (status != ADS1115_OK)
    {
        *conversion_complete = false;
        return status;
    }

    // The OS bit (bit 15) will be 0 when a conversion is complete
    *conversion_complete = !((config_read & ADS1115_CONFIG_OS_START) == ADS1115_CONFIG_OS_START);
    return ADS1115_OK;
}

/**
 * @brief Reads the raw 16-bit conversion result from the ADS1115.
 *        This function should be called after a conversion is complete.
 * @param raw_value Pointer to store the 16-bit raw ADC value.
 * @return ADS1115_Status_t status of the read operation.
 */
ADS1115_Status_t ADS1115_ReadConversionResult(int16_t *raw_value)
{
    if (raw_value == NULL) return ADS1115_INVALID_PARAM;

    uint16_t read_val;
    ADS1115_Status_t status = ADS1115_ReadRegister(ADS1115_REG_CONVERSION, &read_val);
    if (status != ADS1115_OK)
    {
        return status;
    }

    // The ADS1115 returns a signed 16-bit value
    *raw_value = (int16_t)read_val;
    return ADS1115_OK;
}

/**
 * @brief Reads the raw 16-bit conversion result and converts it to voltage.
 *        This function assumes the PGA setting used during configuration.
 * @param raw_value The 16-bit raw ADC value.
 * @param pga The PGA setting used for the conversion.
 * @return float The voltage in Volts.
 */
float ADS1115_RawToVoltage(int16_t raw_value, ADS1115_Pga_t pga)
{
    float fsr = 0.0f; // Full Scale Range

    switch (pga)
    {
        case ADS1115_PGA_6_144V: fsr = 6.144f; break;
        case ADS1115_PGA_4_096V: fsr = 4.096f; break;
        case ADS1115_PGA_2_048V: fsr = 2.048f; break;
        case ADS1115_PGA_1_024V: fsr = 1.024f; break;
        case ADS1115_PGA_0_512V: fsr = 0.512f; break;
        case ADS1115_PGA_0_256V: fsr = 0.256f; break;
        default: fsr = 2.048f; // Default to 2.048V if PGA is unknown
    }

    // The ADS1115 is a 16-bit ADC, but the sign bit means it's effectively 15-bit for positive values
    // and 16-bit for negative values (2's complement). The range is -32768 to +32767.
    // So, the maximum positive value is 2^15 - 1 = 32767.
    // The step size is FSR / 32768.
    return (float)raw_value * (fsr / 32768.0f);
}
