#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>
#include "config.h"

// Define ADS1115 I2C address (can be configured via ADDR pin)
// Default is 0x48 (ADDR pin connected to GND)
// Other options: 0x49 (VCC), 0x4A (SDA), 0x4B (SCL)
#ifndef ADS1115_I2C_ADDRESS
#define ADS1115_I2C_ADDRESS     0x48
#endif

/**
 * @brief Enum for ADS1115 Programmable Gain Amplifier (PGA) settings.
 *        Corresponds to FSR (Full-Scale Range).
 */
enum ads1115_gain
{
    ADS1115_GAIN_PM6_144V = 0, ///< +/- 6.144V (PGA = 2/3)
    ADS1115_GAIN_PM4_096V = 1, ///< +/- 4.096V (PGA = 1)
    ADS1115_GAIN_PM2_048V = 2, ///< +/- 2.048V (PGA = 2) - Default
    ADS1115_GAIN_PM1_024V = 3, ///< +/- 1.024V (PGA = 4)
    ADS1115_GAIN_PM0_512V = 4, ///< +/- 0.512V (PGA = 8)
    ADS1115_GAIN_PM0_256V = 5  ///< +/- 0.256V (PGA = 16)
};

/**
 * @brief Enum for ADS1115 Data Rate (sampling rate) settings.
 *        Corresponds to SPS (Samples Per Second).
 */
enum ads1115_sampling_rate
{
    ADS1115_SPS_8   = 0, ///< 8 Samples per second
    ADS1115_SPS_16  = 1, ///< 16 Samples per second
    ADS1115_SPS_32  = 2, ///< 32 Samples per second
    ADS1115_SPS_64  = 3, ///< 64 Samples per second
    ADS1115_SPS_128 = 4, ///< 128 Samples per second - Default
    ADS1115_SPS_250 = 5, ///< 250 Samples per second
    ADS1115_SPS_475 = 6, ///< 475 Samples per second
    ADS1115_SPS_860 = 7  ///< 860 Samples per second
};

/**
 * @brief Enum for ADS1115 Input Multiplexer (MUX) configuration.
 *        Selects differential or single-ended inputs.
 */
enum ads1115_channel_config
{
    ADS1115_MUX_AIN0_AIN1 = 0, ///< Differential P = AIN0, N = AIN1
    ADS1115_MUX_AIN0_AIN3 = 1, ///< Differential P = AIN0, N = AIN3
    ADS1115_MUX_AIN1_AIN3 = 2, ///< Differential P = AIN1, N = AIN3
    ADS1115_MUX_AIN2_AIN3 = 3, ///< Differential P = AIN2, N = AIN3
    ADS1115_MUX_AIN0_GND  = 4, ///< Single-ended P = AIN0, N = GND
    ADS1115_MUX_AIN1_GND  = 5, ///< Single-ended P = AIN1, N = GND
    ADS1115_MUX_AIN2_GND  = 6, ///< Single-ended P = AIN2, N = GND
    ADS1115_MUX_AIN3_GND  = 7  ///< Single-ended P = AIN3, N = GND
};

/**
 * @brief Status codes for ADS1115 operations.
 */
typedef enum
{
    ADS1115_OK = 0,             ///< Operation successful
    ADS1115_ERROR_I2C_FAIL,     ///< I2C communication failed
    ADS1115_ERROR_INVALID_PARAM ///< Invalid parameter provided
} ads1115_status_t;

/**
 * @brief Initializes the ADS1115 with specified gain, sampling rate, and channel configuration.
 * 
 * This function translates the high-level parameters into the appropriate ADS1115
 * configuration register values and writes them via the I2C driver.
 * 
 * @param gain_setting Desired PGA gain setting.
 * @param rate_setting Desired sampling rate.
 * @param channel_cfg Desired input channel configuration.
 * @return ads1115_status_t ADS1115_OK if successful, otherwise an error code.
 */
ads1115_status_t ads1115_init(enum ads1115_gain gain_setting,
                             enum ads1115_sampling_rate rate_setting,
                             enum ads1115_channel_config channel_cfg);

// Add other ADS1115 function prototypes here (e.g., for reading conversion results)
// ads1115_status_t ads1115_read_conversion(int16_t *value);

#endif // ADS1115_H
