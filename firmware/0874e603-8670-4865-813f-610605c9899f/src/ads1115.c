#include "ads1115.h"
#include "config.h"
#include <stdint.h>
#include <stdbool.h>

// --- Mock I2C Driver (Replace with your actual I2C driver) ---
// These functions are placeholders and need to be replaced with your 
// platform-specific I2C driver implementations.

// Dummy function for I2C write
static bool mock_i2c_write(uint8_t address, uint8_t *data, uint16_t len) {
    // In a real system, this would interact with I2C hardware
    // For simulation, we just print and return success.
    // printf("I2C Write to 0x%02X: ", address);
    // for (int i = 0; i < len; i++) { printf("0x%02X ", data[i]); }
    // printf("\n");
    return true; 
}

// Dummy function for I2C read
static bool mock_i2c_read(uint8_t address, uint8_t *data, uint16_t len) {
    // In a real system, this would interact with I2C hardware
    // For simulation, we just fill with dummy data and return success.
    // printf("I2C Read from 0x%02X, len %d\n", address, len);
    if (len >= 2) {
        // Simulate a conversion result (e.g., mid-range value)
        data[0] = 0x7F; // Most significant byte
        data[1] = 0xFF; // Least significant byte
    }
    return true;
}

// Dummy function for I2C write then read
static bool mock_i2c_write_read(uint8_t address, uint8_t *write_data, uint16_t write_len, uint8_t *read_data, uint16_t read_len) {
    if (!mock_i2c_write(address, write_data, write_len)) return false;
    // A small delay might be needed in real I2C transactions between write and read for some devices
    // For the ADS1115, reading the conversion register typically follows a pointer write.
    return mock_i2c_read(address, read_data, read_len);
}

// Dummy delay function
static void mock_delay_ms(uint32_t ms) {
    // In a real system, this would be a non-blocking delay or a timer-based wait.
    // For simulation, we do nothing.
    (void)ms; // Suppress unused parameter warning
}

// Use these mock functions if your project doesn't have an I2C driver yet
// #define I2C_MASTER_WRITE(address, data, len)  mock_i2c_write(address, data, len)
// #define I2C_MASTER_READ(address, data, len)   mock_i2c_read(address, data, len)
// #define I2C_MASTER_WRITE_READ(address, write_data, write_len, read_data, read_len) mock_i2c_write_read(address, write_data, write_len, read_data, read_len)
// #define DELAY_MS(ms)                          mock_delay_ms(ms)

// If you have a real I2C driver, uncomment and adapt these:
// #include "your_i2c_driver.h" // Include your actual I2C driver header
// #define I2C_MASTER_WRITE(address, data, len)  your_i2c_driver_write(address, data, len)
// #define I2C_MASTER_READ(address, data, len)   your_i2c_driver_read(address, data, len)
// #define I2C_MASTER_WRITE_READ(address, write_data, write_len, read_data, read_len) your_i2c_driver_write_read(address, write_data, write_len, read_data, read_len)
// #define DELAY_MS(ms)                          your_delay_function_ms(ms)

// --- End Mock I2C Driver ---

// Static variable to store the current PGA setting for voltage conversion
static ads1115_pga_t current_pga_setting = ADS1115_PGA_2_048V; // Default to 2.048V

/**
 * @brief Writes a 16-bit value to an ADS1115 register.
 * @param reg_address The address of the register to write to.
 * @param value The 16-bit value to write.
 * @return true on success, false on I2C error.
 */
static bool ads1115_write_register(uint8_t reg_address, uint16_t value) {
    uint8_t tx_buffer[3];
    tx_buffer[0] = reg_address;         // Register pointer
    tx_buffer[1] = (uint8_t)(value >> 8); // MSB
    tx_buffer[2] = (uint8_t)(value & 0xFF); // LSB
    return I2C_MASTER_WRITE(ADS1115_I2C_ADDRESS, tx_buffer, sizeof(tx_buffer));
}

/**
 * @brief Reads a 16-bit value from an ADS1115 register.
 * @param reg_address The address of the register to read from.
 * @param value Pointer to store the read 16-bit value.
 * @return true on success, false on I2C error.
 */
static bool ads1115_read_register(uint8_t reg_address, uint16_t *value) {
    uint8_t rx_buffer[2];
    uint8_t tx_buffer[1] = {reg_address};

    // First, send the register address to set the pointer
    // Then, read the two bytes from that register
    if (!I2C_MASTER_WRITE_READ(ADS1115_I2C_ADDRESS, tx_buffer, sizeof(tx_buffer), rx_buffer, sizeof(rx_buffer))) {
        return false;
    }

    *value = ((uint16_t)rx_buffer[0] << 8) | rx_buffer[1];
    return true;
}

bool ads1115_init(ads1115_pga_t gain, ads1115_data_rate_t sampling_rate, ads1115_mux_t channel_config) {
    // Store the PGA setting for later voltage conversion
    current_pga_setting = gain;

    // Initial configuration for single-shot mode, power-down, and comparator disabled
    // We set the MUX and PGA here, but OS bit is not set to start conversion yet.
    // The OS bit will be set when ads1115_read_raw_data is called.
    uint16_t config_reg_value = 
        ADS1115_CONFIG_OS_NO_EFFECT |   // No effect on write (will be set to single-shot start later)
        channel_config |                // MUX setting
        gain |                          // PGA gain
        ADS1115_CONFIG_MODE_SINGLE |    // Single-shot mode
        sampling_rate |                 // Data rate
        ADS1115_CONFIG_COMP_MODE_TRADITIONAL | // Comparator disabled/traditional
        ADS1115_CONFIG_COMP_POL_ACTIVE_LOW | // Comparator polarity (irrelevant if disabled)
        ADS1115_CONFIG_COMP_LAT_NONLATCHING | // Latching comparator (irrelevant if disabled)
        ADS1115_CONFIG_COMP_QUE_DISABLE;    // Disable comparator

    // Write the initial configuration. This doesn't start a conversion.
    return ads1115_write_register(ADS1115_REG_CONFIG, config_reg_value);
}

bool ads1115_read_raw_data(ads1115_mux_t channel_config, int16_t *raw_data) {
    if (raw_data == NULL) {
        return false;
    }

    uint16_t config_reg_value;

    // 1. Read current configuration to preserve settings like PGA and Data Rate
    if (!ads1115_read_register(ADS1115_REG_CONFIG, &config_reg_value)) {
        return false;
    }

    // 2. Modify configuration for the desired channel and to start a single conversion
    // Clear previous MUX setting and set new one
    config_reg_value &= ~0x7000; // Clear MUX bits (bits 14:12)
    config_reg_value |= channel_config; // Set new MUX bits
    
    // Set OS bit to start a single conversion
    config_reg_value |= ADS1115_CONFIG_OS_SINGLE;

    // 3. Write the modified configuration register to start conversion
    if (!ads1115_write_register(ADS1115_REG_CONFIG, config_reg_value)) {
        return false;
    }

    // 4. Wait for conversion to complete (Poll the OS bit)
    uint32_t timeout_counter = 0;
    do {
        if (!ads1115_read_register(ADS1115_REG_CONFIG, &config_reg_value)) {
            return false;
        }
        // Check if OS bit (bit 15) is 1, indicating conversion is complete
        if ((config_reg_value & ADS1115_CONFIG_OS_MASK) == ADS1115_CONFIG_OS_NOT_BUSY) {
            break; // Conversion complete
        }
        DELAY_MS(1); // Wait 1ms before polling again
        timeout_counter++;
    } while (timeout_counter < ADS1115_CONVERSION_TIMEOUT_MS);

    if (timeout_counter >= ADS1115_CONVERSION_TIMEOUT_MS) {
        // Conversion timed out
        return false;
    }

    // 5. Read the conversion result from the Conversion Register
    uint16_t raw_val_u16;
    if (!ads1115_read_register(ADS1115_REG_CONVERSION, &raw_val_u16)) {
        return false;
    }
    
    // The ADS1115 returns a 16-bit signed integer (two's complement).
    // Cast to int16_t to correctly interpret the signed value.
    *raw_data = (int16_t)raw_val_u16;

    return true;
}

float ads1115_raw_to_voltage(int16_t raw_data, ads1115_pga_t pga) {
    float fsr_volts = 0.0f;

    // Determine the Full Scale Range (FSR) based on the PGA setting
    switch (pga) {
        case ADS1115_PGA_6_144V: fsr_volts = 6.144f; break;
        case ADS1115_PGA_4_096V: fsr_volts = 4.096f; break;
        case ADS1115_PGA_2_048V: fsr_volts = 2.048f; break;
        case ADS1115_PGA_1_024V: fsr_volts = 1.024f; break;
        case ADS1115_PGA_0_512V: fsr_volts = 0.512f; break;
        case ADS1115_PGA_0_256V: fsr_volts = 0.256f; break;
        default: 
            // Should not happen with valid PGA enum values
            // Use a default or error out
            fsr_volts = 2.048f; // Default to 2.048V if unknown
            break;
    }

    // The ADS1115 is a 16-bit ADC, but the range is -32768 to +32767.
    // The maximum positive value is 2^15 - 1 = 32767.
    // The resolution is FSR / 2^15 (for positive values) or FSR / (2^15 - 1) for full range.
    // A common way to calculate is (raw_data * FSR) / 32768.0f
    return ((float)raw_data * fsr_volts) / 32768.0f;
}
