#include "ads1115.h"
#include "config.h"
#include <stdio.h>
#include <stdbool.h>

// --- Mock I2C Driver Initialization (if using mock) ---
// In a real application, your I2C driver would be initialized here.
static bool your_i2c_init_function(void) {
    printf("I2C Master Initialized (Mock)\n");
    return true;
}
// --- End Mock I2C Driver Initialization ---

// Define I2C driver functions if they are not already defined in config.h
// This allows main.c to compile with the mock functions if needed.
#ifndef I2C_MASTER_WRITE
#define I2C_MASTER_WRITE(address, data, len)  mock_i2c_write(address, data, len)
#endif
#ifndef I2C_MASTER_READ
#define I2C_MASTER_READ(address, data, len)   mock_i2c_read(address, data, len)
#endif
#ifndef I2C_MASTER_WRITE_READ
#define I2C_MASTER_WRITE_READ(address, write_data, write_len, read_data, read_len) mock_i2c_write_read(address, write_data, write_len, read_data, read_len)
#endif
#ifndef DELAY_MS
#define DELAY_MS(ms)                          mock_delay_ms(ms)
#endif

// Forward declarations for mock I2C functions if they are not in a separate header
extern bool mock_i2c_write(uint8_t address, uint8_t *data, uint16_t len);
extern bool mock_i2c_read(uint8_t address, uint8_t *data, uint16_t len);
extern bool mock_i2c_write_read(uint8_t address, uint8_t *write_data, uint16_t write_len, uint8_t *read_data, uint16_t read_len);
extern void mock_delay_ms(uint32_t ms);

int main(void) {
    printf("Starting ADS1115 Driver Test...\n");

    // 1. Initialize I2C Master (replace with your actual I2C init function)
    if (!your_i2c_init_function()) {
        printf("Error: I2C Master initialization failed!\n");
        return -1;
    }

    // 2. Initialize ADS1115 with desired settings
    // Example: PGA = +/-2.048V, Data Rate = 128SPS, Channel AIN0 to GND
    ads1115_pga_t pga_setting = ADS1115_PGA_2_048V;
    ads1115_data_rate_t dr_setting = ADS1115_DR_128SPS;
    ads1115_mux_t channel_to_read = ADS1115_MUX_P0_NG; // AIN0 to GND

    if (!ads1115_init(pga_setting, dr_setting, channel_to_read)) {
        printf("Error: ADS1115 initialization failed!\n");
        return -1;
    }
    printf("ADS1115 initialized successfully.\n");

    int16_t raw_adc_value;
    float voltage;

    // 3. Read raw data from AIN0 (single-shot conversion)
    printf("Reading from AIN0...\n");
    if (ads1115_read_raw_data(ADS1115_MUX_P0_NG, &raw_adc_value)) {
        voltage = ads1115_raw_to_voltage(raw_adc_value, pga_setting);
        printf("AIN0 Raw ADC: %d, Voltage: %.4f V\n", raw_adc_value, voltage);
    } else {
        printf("Error: Failed to read from AIN0.\n");
    }

    // 4. Read raw data from AIN1 (single-shot conversion) with same settings
    printf("Reading from AIN1...\n");
    if (ads1115_read_raw_data(ADS1115_MUX_P1_NG, &raw_adc_value)) {
        voltage = ads1115_raw_to_voltage(raw_adc_value, pga_setting);
        printf("AIN1 Raw ADC: %d, Voltage: %.4f V\n", raw_adc_value, voltage);
    } else {
        printf("Error: Failed to read from AIN1.\n");
    }

    // 5. Change PGA setting and read again (e.g., to +/-4.096V)
    printf("Changing PGA to +/-4.096V and reading AIN0...\n");
    pga_setting = ADS1115_PGA_4_096V;
    if (!ads1115_init(pga_setting, dr_setting, ADS1115_MUX_P0_NG)) { // Re-init to update PGA
        printf("Error: ADS1115 re-initialization failed!\n");
        return -1;
    }

    if (ads1115_read_raw_data(ADS1115_MUX_P0_NG, &raw_adc_value)) {
        voltage = ads1115_raw_to_voltage(raw_adc_value, pga_setting);
        printf("AIN0 Raw ADC (new PGA): %d, Voltage: %.4f V\n", raw_adc_value, voltage);
    } else {
        printf("Error: Failed to read from AIN0 with new PGA.\n
");
    }

    printf("ADS1115 Driver Test Complete.\n");

    return 0;
}
