#ifndef ADS1115_DRIVER_H
#define ADS1115_DRIVER_H

#include <stdint.h>
#include "i2c_driver.h"

// ADS1115 Register Addresses
#define ADS1115_REG_CONVERSION  0x00
#define ADS1115_REG_CONFIG      0x01
#define ADS1115_REG_LOW_THRESH  0x02
#define ADS1115_REG_HIGH_THRESH 0x03

// ADS1115 Configuration Register Bits
// OS: Operational Status/Start Conversion (bit 15)
#define ADS1115_CONFIG_OS_NO_EFFECT     (0x00 << 15) // No effect
#define ADS1115_CONFIG_OS_SINGLE_START  (0x01 << 15) // Start a single conversion

// MUX: Input Multiplexer Configuration (bits 14-12)
typedef enum {
    ADS1115_MUX_AIN0_AIN1 = (0x00 << 12), // AIN0-AIN1 (default)
    ADS1115_MUX_AIN0_AIN3 = (0x01 << 12),
    ADS1115_MUX_AIN1_AIN3 = (0x02 << 12),
    ADS1115_MUX_AIN2_AIN3 = (0x03 << 12),
    ADS1115_MUX_AIN0_GND  = (0x04 << 12), // AIN0 single-ended
    ADS1115_MUX_AIN1_GND  = (0x05 << 12), // AIN1 single-ended
    ADS1115_MUX_AIN2_GND  = (0x06 << 12), // AIN2 single-ended
    ADS1115_MUX_AIN3_GND  = (0x07 << 12)  // AIN3 single-ended
} ads1115_channel_t;

// PGA: Programmable Gain Amplifier Configuration (bits 11-9)
typedef enum {
    ADS1115_PGA_6_144V = (0x00 << 9), // +/- 6.144V
    ADS1115_PGA_4_096V = (0x01 << 9), // +/- 4.096V
    ADS1115_PGA_2_048V = (0x02 << 9), // +/- 2.048V (default)
    ADS1115_PGA_1_024V = (0x03 << 9), // +/- 1.024V
    ADS1115_PGA_0_512V = (0x04 << 9), // +/- 0.512V
    ADS1115_PGA_0_256V = (0x05 << 9)  // +/- 0.256V (can also be 0x06, 0x07)
} ads1115_gain_t;

// MODE: Device Operating Mode (bit 8)
#define ADS1115_CONFIG_MODE_CONTINUOUS  (0x00 << 8)
#define ADS1115_CONFIG_MODE_SINGLE_SHOT (0x01 << 8) // Default

// DR: Data Rate (bits 7-5)
typedef enum {
    ADS1115_DR_8SPS   = (0x00 << 5),
    ADS1115_DR_16SPS  = (0x01 << 5),
    ADS1115_DR_32SPS  = (0x02 << 5),
    ADS1115_DR_64SPS  = (0x03 << 5),
    ADS1115_DR_128SPS = (0x04 << 5), // Default
    ADS1115_DR_250SPS = (0x05 << 5),
    ADS1115_DR_475SPS = (0x06 << 5),
    ADS1115_DR_860SPS = (0x07 << 5)
} ads1115_sampling_rate_t;

// COMP_MODE: Comparator Mode (bit 4)
#define ADS1115_CONFIG_COMP_MODE_TRADITIONAL (0x00 << 4)
#define ADS1115_CONFIG_COMP_MODE_WINDOW      (0x01 << 4)

// COMP_POL: Comparator Polarity (bit 3)
#define ADS1115_CONFIG_COMP_POL_LOW  (0x00 << 3)
#define ADS1115_CONFIG_COMP_POL_HIGH (0x01 << 3)

// COMP_LAT: Latching Comparator (bit 2)
#define ADS1115_CONFIG_COMP_LAT_NON_LATCHING (0x00 << 2)
#define ADS1115_CONFIG_COMP_LAT_LATCHING     (0x01 << 2)

// COMP_QUE: Comparator Queue and Disable (bits 1-0)
#define ADS1115_CONFIG_COMP_QUE_ONE    (0x00 << 0)
#define ADS1115_CONFIG_COMP_QUE_TWO    (0x01 << 0)
#define ADS1115_CONFIG_COMP_QUE_FOUR   (0x02 << 0)
#define ADS1115_CONFIG_COMP_QUE_DISABLE (0x03 << 0) // Default

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
);

/**
 * @brief Triggers a single conversion and reads the 16-bit raw conversion result from the ADS1115.
 *        This function blocks until the conversion is complete or a timeout occurs.
 * @param channel The desired input multiplexer configuration for this conversion (e.g., AIN0_GND).
 * @param raw_data Pointer to store the 16-bit raw ADC data.
 * @return I2C_OK on success, otherwise an I2C error code.
 */
i2c_status_t ads1115_read_raw_data(ads1115_channel_t channel, int16_t *raw_data);

/**
 * @brief Converts raw ADC data to voltage based on the current gain setting.
 * @param raw_data The 16-bit raw ADC value.
 * @param gain The gain setting used for the conversion.
 * @return The voltage in Volts.
 */
float ads1115_raw_to_voltage(int16_t raw_data, ads1115_gain_t gain);

#endif // ADS1115_DRIVER_H
