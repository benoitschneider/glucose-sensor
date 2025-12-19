#ifndef ADS1115_DRIVER_H
#define ADS1115_DRIVER_H

#include <stdint.h>
#include "i2c_driver.h"

// ADS1115 Register Addresses
#define ADS1115_REG_CONVERSION      0x00
#define ADS1115_REG_CONFIG          0x01
#define ADS1115_REG_LOWTHRESH       0x02
#define ADS1115_REG_HITHRESH        0x03

// ADS1115 Configuration Register Bit Definitions (MSB first)
// OS: Operational Status/Single-Shot Conversion Start
#define ADS1115_CONFIG_OS_NOOP      0x0000 // No effect
#define ADS1115_CONFIG_OS_START     0x8000 // Start a single conversion

// MUX: Input Multiplexer Configuration
typedef enum {
    ADS1115_MUX_P0_N1 = 0x0000, // AINP = AIN0, AINN = AIN1 (default)
    ADS1115_MUX_P0_N3 = 0x1000, // AINP = AIN0, AINN = AIN3
    ADS1115_MUX_P1_N3 = 0x2000, // AINP = AIN1, AINN = AIN3
    ADS1115_MUX_P2_N3 = 0x3000, // AINP = AIN2, AINN = AIN3
    ADS1115_MUX_P0_NG = 0x4000, // AINP = AIN0, AINN = GND
    ADS1115_MUX_P1_NG = 0x5000, // AINP = AIN1, AINN = GND
    ADS1115_MUX_P2_NG = 0x6000, // AINP = AIN2, AINN = GND
    ADS1115_MUX_P3_NG = 0x7000  // AINP = AIN3, AINN = GND
} ADS1115_Mux_t;

// PGA: Programmable Gain Amplifier Configuration
typedef enum {
    ADS1115_PGA_6_144V = 0x0000, // +/-6.144V (Gain 2/3)
    ADS1115_PGA_4_096V = 0x0200, // +/-4.096V (Gain 1)
    ADS1115_PGA_2_048V = 0x0400, // +/-2.048V (Gain 2) (default)
    ADS1115_PGA_1_024V = 0x0600, // +/-1.024V (Gain 4)
    ADS1115_PGA_0_512V = 0x0800, // +/-0.512V (Gain 8)
    ADS1115_PGA_0_256V = 0x0A00  // +/-0.256V (Gain 16)
} ADS1115_Pga_t;

// MODE: Device Operating Mode
#define ADS1115_MODE_CONTINUOUS     0x0000 // Continuous conversion mode
#define ADS1115_MODE_SINGLESHOT     0x0100 // Single-shot mode (default)

// DR: Data Rate (Samples Per Second)
typedef enum {
    ADS1115_DR_8SPS   = 0x0000,
    ADS1115_DR_16SPS  = 0x0020,
    ADS1115_DR_32SPS  = 0x0040,
    ADS1115_DR_64SPS  = 0x0060,
    ADS1115_DR_128SPS = 0x0080, // default
    ADS1115_DR_250SPS = 0x00A0,
    ADS1115_DR_475SPS = 0x00C0,
    ADS1115_DR_860SPS = 0x00E0
} ADS1115_DataRate_t;

// COMP_MODE: Comparator Mode
#define ADS1115_COMP_MODE_TRADITIONAL 0x0000 // Traditional comparator (default)
#define ADS1115_COMP_MODE_WINDOW      0x0010 // Window comparator

// COMP_POL: Comparator Polarity
#define ADS1115_COMP_POL_ACTIVE_LOW   0x0000 // Active low (default)
#define ADS1115_COMP_POL_ACTIVE_HIGH  0x0008 // Active high

// COMP_LAT: Latching Comparator
#define ADS1115_COMP_LAT_NONLATCHING  0x0000 // Non-latching comparator (default)
#define ADS1115_COMP_LAT_LATCHING     0x0004 // Latching comparator

// COMP_QUE: Comparator Queue and Disable
typedef enum {
    ADS1115_COMP_QUE_1CONV = 0x0000, // Assert after one conversion
    ADS1115_COMP_QUE_2CONV = 0x0001, // Assert after two conversions
    ADS1115_COMP_QUE_4CONV = 0x0002, // Assert after four conversions
    ADS1115_COMP_QUE_DISABLE = 0x0003  // Disable comparator (default)
} ADS1115_CompQue_t;

// ADS1115 Driver Status codes
typedef enum {
    ADS1115_OK = 0,
    ADS1115_I2C_ERROR,
    ADS1115_BUSY,
    ADS1115_INVALID_PARAM
} ADS1115_Status_t;

/**
 * @brief Initializes the ADS1115 driver and performs a basic device check.
 * @return ADS1115_Status_t status of the initialization.
 */
ADS1115_Status_t ADS1115_Init(void);

/**
 * @brief Configures the ADS1115 with the specified settings.
 * @param mux Input multiplexer configuration.
 * @param pga Programmable gain amplifier setting.
 * @param data_rate Samples per second data rate.
 * @param operating_mode ADS1115_MODE_SINGLESHOT or ADS1115_MODE_CONTINUOUS.
 * @return ADS1115_Status_t status of the configuration operation.
 */
ADS1115_Status_t ADS1115_Configure(
    ADS1115_Mux_t mux,
    ADS1115_Pga_t pga,
    ADS1115_DataRate_t data_rate,
    uint16_t operating_mode);

/**
 * @brief Starts a single-shot conversion on the ADS1115.
 *        This function only works if the device is configured in single-shot mode.
 * @return ADS1115_Status_t status of the start operation.
 */
ADS1115_Status_t ADS1115_StartConversion(void);

/**
 * @brief Checks if the ADS1115 conversion is complete.
 * @param conversion_complete Pointer to a boolean to store the status.
 * @return ADS1115_Status_t status of the check operation.
 */
ADS1115_Status_t ADS1115_IsConversionComplete(bool *conversion_complete);

/**
 * @brief Reads the raw 16-bit conversion result from the ADS1115.
 *        This function should be called after a conversion is complete.
 * @param raw_value Pointer to store the 16-bit raw ADC value.
 * @return ADS1115_Status_t status of the read operation.
 */
ADS1115_Status_t ADS1115_ReadConversionResult(int16_t *raw_value);

/**
 * @brief Reads the raw 16-bit conversion result and converts it to voltage.
 *        This function assumes the PGA setting used during configuration.
 * @param raw_value The 16-bit raw ADC value.
 * @param pga The PGA setting used for the conversion.
 * @return float The voltage in Volts.
 */
float ADS1115_RawToVoltage(int16_t raw_value, ADS1115_Pga_t pga);

#endif // ADS1115_DRIVER_H
