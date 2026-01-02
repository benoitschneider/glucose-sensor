#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>
#include <stdbool.h>

// ADS1115 I2C address
#define ADS1115_ADDRESS_GND     0x48 // ADDR pin connected to GND
#define ADS1115_ADDRESS_VCC     0x49 // ADDR pin connected to VCC
#define ADS1115_ADDRESS_SDA     0x4A // ADDR pin connected to SDA
#define ADS1115_ADDRESS_SCL     0x4B // ADDR pin connected to SCL

// ADS1115 Register Pointer Map
#define ADS1115_REG_POINTER_CONVERSION  0x00
#define ADS1115_REG_POINTER_CONFIG      0x01
#define ADS1115_REG_POINTER_LOWTHRESH   0x02
#define ADS1115_REG_POINTER_HITHRESH    0x03

// ADS1115 Configuration Register Bits
// OS: Operational Status or Single-Shot Conversion Start
#define ADS1115_CONFIG_OS_NO_EFFECT     (0 << 15)
#define ADS1115_CONFIG_OS_SINGLE_START  (1 << 15)

// MUX: Input Multiplexer Configuration
typedef enum {
    ADS1115_MUX_P0_N1 = (0 << 12), // AINP = AIN0, AINN = AIN1 (default)
    ADS1115_MUX_P0_N3 = (1 << 12), // AINP = AIN0, AINN = AIN3
    ADS1115_MUX_P1_N3 = (2 << 12), // AINP = AIN1, AINN = AIN3
    ADS1115_MUX_P2_N3 = (3 << 12), // AINP = AIN2, AINN = AIN3
    ADS1115_MUX_P0_NG = (4 << 12), // AINP = AIN0, AINN = GND
    ADS1115_MUX_P1_NG = (5 << 12), // AINP = AIN1, AINN = GND
    ADS1115_MUX_P2_NG = (6 << 12), // AINP = AIN2, AINN = GND
    ADS1115_MUX_P3_NG = (7 << 12)  // AINP = AIN3, AINN = GND
} ads1115_mux_t;

// PGA: Programmable Gain Amplifier Configuration
typedef enum {
    ADS1115_PGA_6_144V = (0 << 9), // +/- 6.144V
    ADS1115_PGA_4_096V = (1 << 9), // +/- 4.096V
    ADS1115_PGA_2_048V = (2 << 9), // +/- 2.048V (default)
    ADS1115_PGA_1_024V = (3 << 9), // +/- 1.024V
    ADS1115_PGA_0_512V = (4 << 9), // +/- 0.512V
    ADS1115_PGA_0_256V = (5 << 9)  // +/- 0.256V
} ads1115_gain_t;

// MODE: Device Operating Mode
#define ADS1115_CONFIG_MODE_CONTINUOUS  (0 << 8)
#define ADS1115_CONFIG_MODE_SINGLE      (1 << 8) // (default)

// DR: Data Rate
typedef enum {
    ADS1115_DR_8SPS   = (0 << 5),
    ADS1115_DR_16SPS  = (1 << 5),
    ADS1115_DR_32SPS  = (2 << 5),
    ADS1115_DR_64SPS  = (3 << 5),
    ADS1115_DR_128SPS = (4 << 5), // (default)
    ADS1115_DR_250SPS = (5 << 5),
    ADS1115_DR_475SPS = (6 << 5),
    ADS1115_DR_860SPS = (7 << 5)
} ads1115_sampling_rate_t;

// COMP_MODE: Comparator Mode
#define ADS1115_CONFIG_COMP_MODE_TRADITIONAL (0 << 4)
#define ADS1115_CONFIG_COMP_MODE_WINDOW      (1 << 4)

// COMP_POL: Comparator Polarity
#define ADS1115_CONFIG_COMP_POL_ACTIVE_LOW   (0 << 3)
#define ADS1115_CONFIG_COMP_POL_ACTIVE_HIGH  (1 << 3)

// COMP_LAT: Latching Comparator
#define ADS1115_CONFIG_COMP_LAT_NON_LATCHING (0 << 2)
#define ADS1115_CONFIG_COMP_LAT_LATCHING     (1 << 2)

// COMP_QUE: Comparator Queue and Disable
#define ADS1115_CONFIG_COMP_QUE_1CONV        (0 << 0)
#define ADS1115_CONFIG_COMP_QUE_2CONV        (1 << 0)
#define ADS1115_CONFIG_COMP_QUE_4CONV        (2 << 0)
#define ADS1115_CONFIG_COMP_QUE_DISABLE      (3 << 0) // (default)

// ADS1115 return codes
typedef enum {
    ADS1115_OK = 0,
    ADS1115_ERR_I2C,
    ADS1115_ERR_TIMEOUT,
    ADS1115_ERR_INVALID_PARAM,
    ADS1115_ERR_NOT_INITIALIZED,
    ADS1115_ERR_BUSY
} ads1115_ret_code_t;

/**
 * @brief Initializes the ADS1115 ADC with specified gain, sampling rate, and channel configuration.
 *        This function writes the configuration register.
 * @param i2c_address The 7-bit I2C address of the ADS1115 (e.g., ADS1115_ADDRESS_GND).
 * @param gain_setting The Programmable Gain Amplifier setting.
 * @param rate_setting The data rate (samples per second).
 * @param channel_cfg The input multiplexer configuration (channel selection).
 * @return ADS1115_OK on success, otherwise an error code.
 */
ads1115_ret_code_t ads1115_init(
    uint8_t i2c_address,
    ads1115_gain_t gain_setting,
    ads1115_sampling_rate_t rate_setting,
    ads1115_mux_t channel_cfg
);

/**
 * @brief Triggers a single-shot conversion and reads the 16-bit raw conversion result.
 *        This function waits for the conversion to complete.
 * @param i2c_address The 7-bit I2C address of the ADS1115.
 * @param raw_data Pointer to store the 16-bit raw ADC value.
 * @return ADS1115_OK on success, otherwise an error code.
 */
ads1115_ret_code_t ads1115_read_raw_data(
    uint8_t i2c_address,
    int16_t *raw_data
);

/**
 * @brief Sets the input multiplexer configuration for the next conversion.
 * @param i2c_address The 7-bit I2C address of the ADS1115.
 * @param mux The new input multiplexer configuration.
 * @return ADS1115_OK on success, otherwise an error code.
 */
ads1115_ret_code_t ads1115_set_mux(uint8_t i2c_address, ads1115_mux_t mux);

#endif // ADS1115_H
