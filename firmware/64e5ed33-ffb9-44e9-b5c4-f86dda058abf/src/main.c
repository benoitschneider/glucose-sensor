#include "config.h"
#include "i2c_driver.h"
#include "ads1115_driver.h"
#include <stdio.h> // For printf, replace with actual logging/debug output for embedded

// Simple delay function (blocking) for main loop
static void main_delay_ms(uint32_t ms)
{
    for (volatile uint32_t i = 0; i < ms * 10000; ++i);
}

int main(void)
{
    // 1. Initialize I2C peripheral
    if (i2c_init(I2C_BUS_SPEED_HZ) != I2C_OK)
    {
        printf("Error: I2C initialization failed!\n");
        // Handle error, e.g., infinite loop or reset
        while(1);
    }
    printf("I2C initialized successfully.\n");

    // 2. Initialize ADS1115 with desired settings
    // Example: Gain +/- 2.048V, Sampling Rate 128 SPS, AIN0 single-ended
    ads1115_gain_t adc_gain = ADS1115_PGA_2_048V;
    ads1115_sampling_rate_t adc_sps = ADS1115_DR_128SPS;
    ads1115_channel_t adc_channel = ADS1115_MUX_AIN0_GND; // Reading AIN0 relative to GND

    if (ads1115_init(adc_gain, adc_sps, adc_channel) != I2C_OK)
    {
        printf("Error: ADS1115 initialization failed!\n");
        // Handle error
        while(1);
    }
    printf("ADS1115 initialized successfully.\n");

    int16_t raw_adc_data;
    float voltage;

    while (1)
    {
        // 3. Read raw data from ADS1115 on selected channel
        // The ads1115_read_raw_data function handles triggering a single conversion
        // and waiting for its completion.
        if (ads1115_read_raw_data(adc_channel, &raw_adc_data) == I2C_OK)
        {
            printf("Raw ADC Data (AIN0): %d\n", raw_adc_data);

            // 4. Convert raw data to voltage
            voltage = ads1115_raw_to_voltage(raw_adc_data, adc_gain);
            printf("Voltage (AIN0): %.4f V\n", voltage);
        }
        else
        {
            printf("Error: Failed to read from ADS1115!\n");
        }

        // Example: Read from a different channel (AIN1_GND)
        ads1115_channel_t adc_channel_ain1 = ADS1115_MUX_AIN1_GND;
        if (ads1115_read_raw_data(adc_channel_ain1, &raw_adc_data) == I2C_OK)
        {
            printf("Raw ADC Data (AIN1): %d\n", raw_adc_data);
            voltage = ads1115_raw_to_voltage(raw_adc_data, adc_gain);
            printf("Voltage (AIN1): %.4f V\n", voltage);
        }
        else
        {
            printf("Error: Failed to read from ADS1115 AIN1!\n");
        }

        // Delay before next reading (e.g., 1 second)
        main_delay_ms(1000);
    }

    return 0;
}
