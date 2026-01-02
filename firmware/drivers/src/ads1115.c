#include "ads1115.h"
#include "i2c.h"
#include <stdbool.h>

// A simple delay function (replace with actual delay from your HAL/OS)
static void ads1115_delay_ms(uint32_t ms)
{
    // Placeholder: In a real system, use a timer-based delay or OS delay.
    // For now, a busy-wait loop for simulation.
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 10000; j++); // Adjust for your clock speed
    }
}

/**
 * @brief Writes a 16-bit value to an ADS1115 register.
 * @param i2c_address The 7-bit I2C address of the ADS1115.
 * @param reg The register pointer (e.g., ADS1115_REG_POINTER_CONFIG).
 * @param value The 16-bit value to write.
 * @return ADS1115_OK on success, otherwise an error code.
 */
static ads1115_ret_code_t ads1115_write_register(uint8_t i2c_address, uint8_t reg, uint16_t value)
{
    uint8_t tx_buf[3];
    tx_buf[0] = reg;            // Register pointer
    tx_buf[1] = (uint8_t)(value >> 8); // MSB
    tx_buf[2] = (uint8_t)(value & 0xFF); // LSB

    if (i2c_write(i2c_address, tx_buf, 3, false) != I2C_SUCCESS) {
        return ADS1115_ERR_I2C;
    }
    return ADS1115_OK;
}

/**
 * @brief Reads a 16-bit value from an ADS1115 register.
 * @param i2c_address The 7-bit I2C address of the ADS1115.
 * @param reg The register pointer (e.g., ADS1115_REG_POINTER_CONVERSION).
 * @param value Pointer to store the read 16-bit value.
 * @return ADS1115_OK on success, otherwise an error code.
 */
static ads1115_ret_code_t ads1115_read_register(uint8_t i2c_address, uint8_t reg, uint16_t *value)
{
    uint8_t tx_buf[1] = {reg};
    uint8_t rx_buf[2];

    // Send register pointer
    if (i2c_write(i2c_address, tx_buf, 1, true) != I2C_SUCCESS) { // Repeated start
        return ADS1115_ERR_I2C;
    }
    // Read 2 bytes from the register
    if (i2c_read(i2c_address, rx_buf, 2) != I2C_SUCCESS) {
        return ADS1115_ERR_I2C;
    }

    *value = ((uint16_t)rx_buf[0] << 8) | rx_buf[1];
    return ADS1115_OK;
}

ads1115_ret_code_t ads1115_init(
    uint8_t i2c_address,
    ads1115_gain_t gain_setting,
    ads1115_sampling_rate_t rate_setting,
    ads1115_mux_t channel_cfg) 
{
    // Default configuration: Single-shot, Power-down mode, Comparator disabled
    // The OS bit is not set here, as init should not trigger a conversion.
    // Conversions are triggered by ads1115_read_raw_data.
    uint16_t config = ADS1115_CONFIG_OS_NO_EFFECT | // Don't start conversion yet
                      channel_cfg |
                      gain_setting |
                      ADS1115_CONFIG_MODE_SINGLE | // Set to single-shot mode
                      rate_setting |
                      ADS1115_CONFIG_COMP_QUE_DISABLE; // Disable comparator

    return ads1115_write_register(i2c_address, ADS1115_REG_POINTER_CONFIG, config);
}

ads1115_ret_code_t ads1115_set_mux(uint8_t i2c_address, ads1115_mux_t mux)
{
    uint16_t current_config;
    ads1115_ret_code_t err_code = ads1115_read_register(i2c_address, ADS1115_REG_POINTER_CONFIG, &current_config);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    // Clear existing MUX bits and set new ones
    // MUX bits are 12-14. Mask is 0b1110000000000000 >> 12 = 0x07
    current_config &= ~((uint16_t)0x07 << 12); 
    current_config |= mux;

    // Write the updated configuration back
    return ads1115_write_register(i2c_address, ADS1115_REG_POINTER_CONFIG, current_config);
}

ads1115_ret_code_t ads1115_read_raw_data(
    uint8_t i2c_address,
    int16_t *raw_data)
{
    if (raw_data == NULL) {
        return ADS1115_ERR_INVALID_PARAM;
    }

    uint16_t config_reg;
    ads1115_ret_code_t err_code;

    // 1. Read current config to preserve settings (gain, rate, etc.)
    err_code = ads1115_read_read_register(i2c_address, ADS1115_REG_POINTER_CONFIG, &config_reg);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    // Check if a conversion is already in progress (OS bit is 0)
    if (!(config_reg & ADS1115_CONFIG_OS_SINGLE_START)) {
        // If OS bit is 0, a conversion is in progress or completed. If completed, we can read it.
        // If we want to start a *new* conversion, we need to ensure it's not busy.
        // For single-shot mode, OS=0 means conversion is complete and data is ready.
        // If we want to force a new conversion, we should set OS=1.
        // The current implementation sets OS=1 to start a new conversion.
        // If OS is already 0, it implies a previous conversion finished. We'll proceed to start a new one.
    }

    // Ensure it's in single-shot mode (MODE bit 8 set to 1)
    // This check is important because ads1115_read_raw_data is designed for single-shot conversions.
    if (! (config_reg & ADS1115_CONFIG_MODE_SINGLE)) {
        // If not in single-shot mode, the behavior of setting OS_SINGLE_START might be different
        // (e.g., continuous mode doesn't use the OS bit to start conversions).
        // Returning an error here makes the driver's usage explicit.
        return ADS1115_ERR_NOT_INITIALIZED; // Or a more specific error like ADS1115_ERR_MODE_MISMATCH
    }

    // 2. Set OS bit to start a single conversion
    config_reg |= ADS1115_CONFIG_OS_SINGLE_START; // Set bit 15 to 1
    err_code = ads1115_write_register(i2c_address, ADS1115_REG_POINTER_CONFIG, config_reg);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    // 3. Wait for conversion to complete
    // The 'OS' bit (bit 15) will be cleared by the ADS1115 when conversion is complete.
    // Max conversion time depends on the configured data rate (DR).
    // For simplicity, we'll poll the OS bit with a generous timeout.
    uint32_t timeout_count = 0;
    // A typical max conversion time for 8 SPS is ~125ms. For 860 SPS is ~1.16ms.
    // 1000 polls with 1ms delay each gives 1 second timeout, which is more than enough.
    const uint32_t max_timeout_polls = 1000; 

    do {
        ads1115_delay_ms(1); // Wait a short period (e.g., 1ms)
        err_code = ads1115_read_register(i2c_address, ADS1115_REG_POINTER_CONFIG, &config_reg);
        if (err_code != ADS1115_OK) {
            return err_code;
        }
        timeout_count++;
    } while ((config_reg & ADS1115_CONFIG_OS_SINGLE_START) && (timeout_count < max_timeout_polls));

    if (timeout_count >= max_timeout_polls) {
        return ADS1115_ERR_TIMEOUT;
    }

    // 4. Read the conversion result from the Conversion Register
    uint16_t conversion_value;
    err_code = ads1115_read_register(i2c_address, ADS1115_REG_POINTER_CONVERSION, &conversion_value);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    *raw_data = (int16_t)conversion_value; // Conversion result is signed 16-bit

    return ADS1115_OK;
}
