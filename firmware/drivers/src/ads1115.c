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
    uint16_t config = ADS1115_CONFIG_OS_NO_EFFECT | // Don't start conversion yet
                      channel_cfg |
                      gain_setting |
                      ADS1115_CONFIG_MODE_SINGLE |
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
    current_config &= ~((uint16_t)0x07 << 12); // Clear MUX bits (bits 12-14)
    current_config |= mux;

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

    // 1. Read current config to preserve settings and get current MUX
    err_code = ads1115_read_register(i2c_address, ADS1115_REG_POINTER_CONFIG, &config_reg);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    // 2. Set OS bit to start a single conversion
    config_reg |= ADS1115_CONFIG_OS_SINGLE_START;
    err_code = ads1115_write_register(i2c_address, ADS1115_REG_POINTER_CONFIG, config_reg);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    // 3. Wait for conversion to complete
    // The 'OS' bit will be cleared by the ADS1115 when conversion is complete.
    // Max conversion time for 8 SPS is ~125ms, for 860 SPS is ~1.16ms.
    // We need to know the configured data rate to set an appropriate timeout.
    // For simplicity, we'll poll the OS bit.
    uint32_t timeout_count = 0;
    const uint32_t max_timeout_polls = 1000; // Arbitrary max polls

    do {
        ads1115_delay_ms(1); // Wait a short period
        err_code = ads1115_read_register(i2c_address, ADS1115_REG_POINTER_CONFIG, &config_reg);
        if (err_code != ADS1115_OK) {
            return err_code;
        }
        timeout_count++;
    } while ((config_reg & ADS1115_CONFIG_OS_SINGLE_START) && (timeout_count < max_timeout_polls));

    if (timeout_count >= max_timeout_polls) {
        return ADS1115_ERR_TIMEOUT;
    }

    // 4. Read the conversion result
    uint16_t conversion_value;
    err_code = ads1115_read_register(i2c_address, ADS1115_REG_POINTER_CONVERSION, &conversion_value);
    if (err_code != ADS1115_OK) {
        return err_code;
    }

    *raw_data = (int16_t)conversion_value; // Conversion result is signed 16-bit

    return ADS1115_OK;
}
