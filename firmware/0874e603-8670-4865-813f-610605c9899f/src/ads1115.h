#ifndef ADS1115_H
#define ADS1115_H

#include <stdint.h>
#include <stdbool.h>

// ADS1115 Register Addresses
#define ADS1115_REG_CONVERSION  0x00
#define ADS1115_REG_CONFIG      0x01
#define ADS1115_REG_LOWTHRESH   0x02
#define ADS1115_REG_HITHRESH    0x03

// ADS1115 Configuration Register Bit Masks
// OS (Operational Status / Single-shot conversion start)
#define ADS1115_CONFIG_OS_MASK      0x8000
#define ADS1115_CONFIG_OS_NO_EFFECT 0x0000 // Write: No effect
#define ADS1115_CONFIG_OS_SINGLE    0x8000 // Write: Start a single conversion
#define ADS1115_CONFIG_OS_BUSY      0x0000 // Read: Device is busy
#define ADS1115_CONFIG_OS_NOT_BUSY  0x8000 // Read: Device is not busy

// MUX (Input Multiplexer Configuration)
typedef enum {
    ADS1115_MUX_P0_N1 = 0x0000, // AINP = AIN0, AINN = AIN1 (default)
    ADS1115_MUX_P0_N3 = 0x1000, // AINP = AIN0, AINN = AIN3
    ADS1115_MUX_P1_N3 = 0x2000, // AINP = AIN1, AINN = AIN3
    ADS1115_MUX_P2_N3 = 0x3000, // AINP = AIN2, AINN = AIN3
    ADS1115_MUX_P0_NG = 0x4000, // AINP = AIN0, AINN = GND
    ADS1115_MUX_P1_NG = 0x5000, // AINP = AIN1, AINN = GND
    ADS1115_MUX_P2_NG = 0x6000, // AINP = AIN2, AINN = GND
    ADS1115_MUX_P3_NG = 0x7000  // AINP = AIN3, AINN = GND
} ads1115_mux_t;

// PGA (Programmable Gain Amplifier Configuration)
typedef enum {
    ADS1115_PGA_6_144V = 0x0000, // +/-6.144V
    ADS1115_PGA_4_096V = 0x0200, // +/-4.096V
    ADS1115_PGA_2_048V = 0x0400, // +/-2.048V (default)
    ADS1115_PGA_1_024V = 0x0600, // +/-1.024V
    ADS1115_PGA_0_512V = 0x0800, // +/-0.512V
    ADS1115_PGA_0_256V = 0x0A00  // +/-0.256V
} ads1115_pga_t;

// MODE (Device Operating Mode)
#define ADS1115_CONFIG_MODE_MASK    0x0100
#define ADS1115_CONFIG_MODE_CONTINUOUS 0x0000 // Continuous conversion mode
#define ADS1115_CONFIG_MODE_SINGLE  0x0100 // Single-shot mode (default)

// DR (Data Rate)
typedef enum {
    ADS1115_DR_8SPS   = 0x0000, // 8 samples per second
    ADS1115_DR_16SPS  = 0x0020, // 16 samples per second
    ADS1115_DR_32SPS  = 0x0040, // 32 samples per second
    ADS1115_DR_64SPS  = 0x0060, // 64 samples per second
    ADS1115_DR_128SPS = 0x0080, // 128 samples per second (default)
    ADS1115_DR_250SPS = 0x00A0, // 250 samples per second
    ADS1115_DR_475SPS = 0x00C0, // 475 samples per second
    ADS1115_DR_860SPS = 0x00E0  // 860 samples per second
} ads1115_data_rate_t;

// COMP_MODE (Comparator Mode)
#define ADS1115_CONFIG_COMP_MODE_TRADITIONAL 0x0000 // Traditional comparator (default)
#define ADS1115_CONFIG_COMP_MODE_WINDOW      0x0010 // Window comparator

// COMP_POL (Comparator Polarity)
#define ADS1115_CONFIG_COMP_POL_ACTIVE_LOW 0x0000 // Active low (default)
#define ADS1115_CONFIG_COMP_POL_ACTIVE_HIGH 0x0008 // Active high

// COMP_LAT (Latching Comparator)
#define ADS1115_CONFIG_COMP_LAT_NONLATCHING 0x0000 // Non-latching comparator (default)
#define ADS1115_CONFIG_COMP_LAT_LATCHING    0x0004 // Latching comparator

// COMP_QUE (Comparator Queue and Disable)
#define ADS1115_CONFIG_COMP_QUE_1CONV   0x0000 // Assert after one conversion
#define ADS1115_CONFIG_COMP_QUE_2CONV   0x0001 // Assert after two conversions
#define ADS1115_CONFIG_COMP_QUE_4CONV   0x0002 // Assert after four conversions
#define ADS1115_CONFIG_COMP_QUE_DISABLE 0x0003 // Disable comparator (default)

/**
 * @brief Initializes the ADS1115 ADC with specified gain, sampling rate, and channel configuration.
 *        This function sets up the configuration register for single-shot mode.
 * 
 * @param gain The PGA gain setting (e.g., ADS1115_PGA_2_048V).
 * @param sampling_rate The data rate setting (e.g., ADS1115_DR_128SPS).
 * @param channel_config The MUX setting for the desired channel (e.g., ADS1115_MUX_P0_NG for AIN0 to GND).
 * @return true if initialization is successful, false otherwise.
 */
bool ads1115_init(ads1115_pga_t gain, ads1115_data_rate_t sampling_rate, ads1115_mux_t channel_config);

/**
 * @brief Triggers a single conversion on the specified channel and reads the 16-bit raw result.
 *        This function waits for the conversion to complete before reading the data.
 * 
 * @param channel_config The MUX setting for the desired channel (e.g., ADS1115_MUX_P0_NG for AIN0 to GND).
 * @param raw_data Pointer to a uint16_t where the raw conversion result will be stored.
 * @return true if the conversion and read are successful, false otherwise.
 */
bool ads1115_read_raw_data(ads1115_mux_t channel_config, int16_t *raw_data);

/**
 * @brief Converts a raw 16-bit ADC value to a voltage based on the current PGA setting.
 * 
 * @param raw_data The 16-bit raw ADC value.
 * @param pga The PGA setting used during the conversion.
 * @return The converted voltage in Volts (float).
 */
float ads1115_raw_to_voltage(int16_t raw_data, ads1115_pga_t pga);

#endif // ADS1115_H