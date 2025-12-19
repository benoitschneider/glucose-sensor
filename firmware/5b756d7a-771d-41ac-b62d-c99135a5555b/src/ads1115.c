#include "ads1115.h"
#include "i2c_driver.h"
#include "config.h"

// ADS1115 Register Addresses
#define ADS1115_REG_CONVERSION      0x00
#define ADS1115_REG_CONFIG          0x01
#define ADS1115_REG_LO_THRESH       0x02
#define ADS1115_REG_HI_THRESH       0x03

// Configuration Register Bit Masks and Values
#define ADS1115_CONFIG_OS_MASK      (0x01 << 7) // Operational Status/Start Conversion
#define ADS1115_CONFIG_OS_SINGLE    (0x01 << 7) // Write 1 to start a single conversion

#define ADS1115_CONFIG_MUX_MASK     (0x07 << 4) // Input Multiplexer
// MUX values are defined in the enum mapping within ads1115_init

#define ADS1115_CONFIG_PGA_MASK     (0x07 << 1) // Programmable Gain Amplifier
// PGA values are defined in the enum mapping within ads1115_init

#define ADS1115_CONFIG_MODE_MASK    (0x01 << 0) // Device Operating Mode
#define ADS1115_CONFIG_MODE_SINGLE  (0x01 << 0) // Single-shot mode
#define ADS1115_CONFIG_MODE_CONTINUOUS (0x00 << 0) // Continuous conversion mode

#define ADS1115_CONFIG_DR_MASK      (0x07 << 5) // Data Rate
// DR values are defined in the enum mapping within ads1115_init

#define ADS1115_CONFIG_COMP_MODE_MASK (0x01 << 4) // Comparator Mode
#define ADS1115_CONFIG_COMP_POL_MASK  (0x01 << 3) // Comparator Polarity
#define ADS1115_CONFIG_COMP_LAT_MASK  (0x01 << 2) // Latching Comparator
#define ADS1115_CONFIG_COMP_QUE_MASK  (0x03 << 0) // Comparator Queue and Disable

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
                             enum ads1115_channel_config channel_cfg)
{
    uint16_t config_reg_value = 0;
    i2c_status_t i2c_status;

    // Start with default configuration (e.g., power-up defaults or known safe state)
    // Set to single-shot mode (bit 0 = 1)
    // Comparator disabled (bits 0-1 of COMP_QUE = 11)
    config_reg_value |= ADS1115_CONFIG_MODE_SINGLE; // Set to single-shot mode
    config_reg_value |= (0x03 << 0); // Disable comparator (COMP_QUE = 11)

    // Map Gain Setting (PGA bits 1-3)
    switch (gain_setting)
    {
        case ADS1115_GAIN_PM6_144V:
            config_reg_value |= (0x00 << 1); // PGA_6_144V
            break;
        case ADS1115_GAIN_PM4_096V:
            config_reg_value |= (0x01 << 1); // PGA_4_096V
            break;
        case ADS1115_GAIN_PM2_048V:
            config_reg_value |= (0x02 << 1); // PGA_2_048V (default)
            break;
        case ADS1115_GAIN_PM1_024V:
            config_reg_value |= (0x03 << 1); // PGA_1_024V
            break;
        case ADS1115_GAIN_PM0_512V:
            config_reg_value |= (0x04 << 1); // PGA_0_512V
            break;
        case ADS1115_GAIN_PM0_256V:
            config_reg_value |= (0x05 << 1); // PGA_0_256V
            break;
        default:
            return ADS1115_ERROR_INVALID_PARAM; // Invalid gain setting
    }

    // Map Sampling Rate (DR bits 5-7 of the second byte, but shifted for 16-bit word)
    switch (rate_setting)
    {
        case ADS1115_SPS_8:
            config_reg_value |= (0x00 << 5); // DR_8SPS
            break;
        case ADS1115_SPS_16:
            config_reg_value |= (0x01 << 5); // DR_16SPS
            break;
        case ADS1115_SPS_32:
            config_reg_value |= (0x02 << 5); // DR_32SPS
            break;
        case ADS1115_SPS_64:
            config_reg_value |= (0x03 << 5); // DR_64SPS
            break;
        case ADS1115_SPS_128:
            config_reg_value |= (0x04 << 5); // DR_128SPS (default)
            break;
        case ADS1115_SPS_250:
            config_reg_value |= (0x05 << 5); // DR_250SPS
            break;
        case ADS1115_SPS_475:
            config_reg_value |= (0x06 << 5); // DR_475SPS
            break;
        case ADS1115_SPS_860:
            config_reg_value |= (0x07 << 5); // DR_860SPS
            break;
        default:
            return ADS1115_ERROR_INVALID_PARAM; // Invalid sampling rate
    }

    // Map Channel Configuration (MUX bits 4-6 of the first byte, but shifted for 16-bit word)
    switch (channel_cfg)
    {
        case ADS1115_MUX_AIN0_AIN1:
            config_reg_value |= (0x00 << 12); // MUX_AIN0_AIN1
            break;
        case ADS1115_MUX_AIN0_AIN3:
            config_reg_value |= (0x01 << 12); // MUX_AIN0_AIN3
            break;
        case ADS1115_MUX_AIN1_AIN3:
            config_reg_value |= (0x02 << 12); // MUX_AIN1_AIN3
            break;
        case ADS1115_MUX_AIN2_AIN3:
            config_reg_value |= (0x03 << 12); // MUX_AIN2_AIN3
            break;
        case ADS1115_MUX_AIN0_GND:
            config_reg_value |= (0x04 << 12); // MUX_AIN0_GND
            break;
        case ADS1115_MUX_AIN1_GND:
            config_reg_value |= (0x05 << 12); // MUX_AIN1_GND
            break;
        case ADS1115_MUX_AIN2_GND:
            config_reg_value |= (0x06 << 12); // MUX_AIN2_GND
            break;
        case ADS1115_MUX_AIN3_GND:
            config_reg_value |= (0x07 << 12); // MUX_AIN3_GND
            break;
        default:
            return ADS1115_ERROR_INVALID_PARAM; // Invalid channel configuration
    }
    
    // Set operational status to start a single conversion (OS bit 15 = 1)
    // This bit is self-clearing. Writing '1' starts a conversion.
    // For initial configuration, we might not want to start immediately, 
    // but rather configure and then trigger a conversion later. 
    // However, the datasheet mentions OS bit can be set to 1 to begin a conversion
    // when in single-shot mode. For initialization, we'll just set it to 0 for now.
    // It will be set to 1 when a conversion is explicitly requested.
    // config_reg_value |= ADS1115_CONFIG_OS_SINGLE; // This would start a conversion immediately.

    // Write the 16-bit configuration register. The ADS1115 expects MSB first.
    // The I2C driver should handle byte ordering if needed, but typically
    // it sends bytes as provided. We need to split the 16-bit value into two 8-bit bytes.
    uint8_t write_buffer[3];
    write_buffer[0] = ADS1115_REG_CONFIG; // Pointer byte for configuration register
    write_buffer[1] = (uint8_t)((config_reg_value >> 8) & 0xFF); // MSB
    write_buffer[2] = (uint8_t)(config_reg_value & 0xFF);       // LSB

    i2c_status = i2c_driver_write(ADS1115_I2C_ADDRESS, write_buffer, 3);

    if (i2c_status != I2C_OK)
    {
        return ADS1115_ERROR_I2C_FAIL;
    }

    return ADS1115_OK;
}

// Placeholder for other ADS1115 functions (e.g., read_conversion)
// ads1115_status_t ads1115_read_conversion(int16_t *value)
// {
//     // Implementation for reading conversion results
//     return ADS1115_OK;
// }
