#include "ads1115_driver.h"
#include "config.h"
#include <stddef.h>

// Forward declaration for internal delay function
static void ads1115_delay_ms(uint32_t ms);

// Global variable to store the current configuration, useful for subsequent reads or voltage conversion
static uint16_t current_config_reg = 0;
static ads1115_gain_t current_gain_setting = ADS1115_PGA_2_048V; // Default gain

/**
 * @brief Internal delay function (blocking). Replace with non-blocking timer in real RTOS.
 * @param ms Milliseconds to delay.
 */
static void ads1115_delay_ms(uint32_t ms)
{
    // This is a blocking delay and should be replaced by a non-blocking
    // timer-based delay or RTOS delay in a production environment.
    for (volatile uint32_t i = 0; i < ms * 10000; ++i);
}

/**
 * @brief Initializes the ADS1115 with specified gain, sampling rate, and channel configuration.
 *        Sets up the configuration register for single-shot mode.
 * @param gain The desired PGA gain setting.
 * @param sampling_rate The desired data sampling rate.
 * @param channel_config The desired input multiplexer configuration (e.g., AIN0_GND).
 * @return I2C_OK on success, otherwise an I2C error code.
 */
i2c_status_t ads1115_init(
    ads1115_gain_t gain,
    ads1115_sampling_rate_t sampling_rate,
    ads1115_channel_t channel_config
)
{
    // Store the current gain setting for later voltage conversion
    current_gain_setting = gain;

    // Construct the configuration register value
    // Start with default settings for non-specified bits
    // OS: No effect (bit 15 is 0 for write)
    // MODE: Single-shot mode (bit 8)
    // COMP_QUE: Disable comparator (bits 1-0)
    // COMP_LAT: Non-latching (bit 2)
    // COMP_POL: Active low (bit 3)
    // COMP_MODE: Traditional comparator (bit 4)
    uint16_t config_word = 0;
    config_word |= channel_config;      // MUX bits (14-12)
    config_word |= gain;                // PGA bits (11-9)
    config_word |= ADS1115_CONFIG_MODE_SINGLE_SHOT; // MODE bit (8)
    config_word |= sampling_rate;       // DR bits (7-5)
    config_word |= ADS1115_CONFIG_COMP_QUE_DISABLE; // COMP_QUE bits (1-0)
    config_word |= ADS1115_CONFIG_COMP_LAT_NON_LATCHING; // COMP_LAT bit (2)
    config_word |= ADS1115_CONFIG_COMP_POL_LOW; // COMP_POL bit (3)
    config_word |= ADS1115_CONFIG_COMP_MODE_TRADITIONAL; // COMP_MODE bit (4)

    // Store the base configuration for future read operations
    current_config_reg = config_word;

    // Send the configuration to the ADS1115
    // The ADS1115 expects 16-bit words to be sent MSB first
uint8_t write_buffer[2];
    write_buffer[0] = (uint8_t)(config_word & 0xFF);         // LSB
    write_buffer[1] = (uint8_t)((config_word >> 8) & 0xFF); // MSB

    i2c_status_t status = i2c_write(ADS1115_I2C_ADDRESS, ADS1115_REG_CONFIG, write_buffer, 2, I2C_TIMEOUT_MS);

    return status;
}

/**
 * @brief Triggers a single conversion and reads the 16-bit raw conversion result from the ADS1115.
 *        This function blocks until the conversion is complete or a timeout occurs.
 * @param channel The desired input multiplexer configuration for this conversion (e.g., AIN0_GND).
 * @param raw_data Pointer to store the 16-bit raw ADC data.
 * @return I2C_OK on success, otherwise an I2C error code.
 */
i2c_status_t ads1115_read_raw_data(ads1115_channel_t channel, int16_t *raw_data)
{
    if (raw_data == NULL) {
        return I2C_ERROR; // Null pointer check
    }

    // Update the configuration register for the specific channel and trigger a conversion
    // The OS bit (bit 15) must be set to '1' to start a single conversion.
    uint16_t config_word_with_os = current_config_reg; // Start with the base config
    config_word_with_os &= ~ADS1115_MUX_AIN0_AIN1; // Clear previous MUX settings
    config_word_with_os |= channel;                // Set new MUX settings
    config_word_with_os |= ADS1115_CONFIG_OS_SINGLE_START; // Set OS bit to start conversion

    uint8_t write_buffer[2];
    write_buffer[0] = (uint8_t)((config_word_with_os >> 8) & 0xFF); // MSB
    write_buffer[1] = (uint8_t)(config_word_with_os & 0xFF);         // LSB

    i2c_status_t status = i2c_write(ADS1115_I2C_ADDRESS, ADS1115_REG_CONFIG, write_buffer, 2, I2C_TIMEOUT_MS);
    if (status != I2C_OK) {
        return status;
    }

    // Wait for the conversion to complete.
    // The OS bit (bit 15) will return to '0' when conversion is complete.
    uint38_t timeout_counter = 0;
    const uint32_t max_timeout_ms = 500; // Max reasonable time for conversion (e.g., 8 SPS = 125ms)
    uint8_t read_buffer[2];

    do {
        ads1115_delay_ms(1); // Wait 1ms before checking again
        timeout_counter++;

        // Read the config register to check the OS bit
        status = i2c_read(ADS1115_I2C_ADDRESS, ADS1115_REG_CONFIG, read_buffer, 2, I2C_TIMEOUT_MS);
        if (status != I2C_OK) {
            return status;
        }

        uint16_t current_status_config = ((uint16_t)read_buffer[0] << 8) | read_buffer[1];

        // Check if the OS bit (bit 15) is 0, indicating conversion is complete
        if (!((current_status_config >> 15) & 0x01)) {
            break; // Conversion complete
        }

    } while (timeout_counter < max_timeout_ms);

    if (timeout_counter >= max_timeout_ms) {
        return I2C_TIMEOUT; // Conversion timed out
    }

    // Read the conversion result from the Conversion Register
    status = i2c_read(ADS1115_I2C_ADDRESS, ADS1115_REG_CONVERSION, read_buffer, 2, I2C_TIMEOUT_MS);
    if (status != I2C_OK) {
        return status;
    }

    // Combine the two bytes into a 16-bit signed integer
    // The ADS1115 provides data in two's complement format.
    *raw_data = (int116_t)(((uint16_t)read_buffer[0] << 8) | read_buffer[1]);

    return I2C_OK;
}

/**
 * @brief Converts raw ADC data to voltage based on the current gain setting.
 * @param raw_data The 16-bit raw ADC value.
 * @param gain The gain setting used for the conversion.
 * @return The voltage in Volts.
 */
float ads1115_raw_to_voltage(int16_t raw_data, ads1115_gain_t gain)
{
    float fsr = 0.0f; // Full Scale Range

    // Determine the FSR based on the gain setting
    switch (gain)
    {
        case ADS1115_PGA_6_144V: fsr = 6.144f; break;
        case ADS1115_PGA_4_096V: fsr = 4.096f; break;
        case ADS1115_PGA_2_048V: fsr = 2.048f; break;
        case ADS1115_PGA_1_024V: fsr = 1.024f; break;
        case ADS1115_PGA_0_512V: fsr = 0.512f; break;
        case ADS1115_PGA_0_256V: fsr = 0.256f; break;
        default: fsr = 2.048f; break; // Default to 2.048V if an unknown gain is passed
    }

    // The ADS1115 is a 16-bit ADC, so it has 2^15 - 1 positive steps (0x7FFF) and 2^15 negative steps (0x8000)
    // The maximum positive value is 0x7FFF (32767).
    // The voltage is calculated as (raw_data / 32767.0) * FSR.
    return ((float)raw_data / 32767.0f) * fsr;
}
