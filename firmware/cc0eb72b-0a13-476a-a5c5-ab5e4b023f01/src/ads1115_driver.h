#ifndef ADS1115_DRIVER_H
#define ADS1115_DRIVER_H

#include <stdint.h>

// Define status codes for the ADS1115 driver module
typedef enum
{
    ADS1115_OK = 0,
    ADS1115_ERROR,
    ADS1115_BUSY,
    // Add more specific error codes (e.g., I2C_ERROR, CONFIG_ERROR)
} ADS1115_Status_t;

/**
 * @brief Initializes the ADS1115 ADC.
 *        This function should configure the I2C peripheral and the ADS1115
 *        registers (e.g., gain, data rate, operating mode).
 * @return ADS1115_OK if successful, error code otherwise.
 */
ADS1115_Status_t ADS1115_Init(void);

/**
 * @brief Reads a raw 16-bit conversion result from the ADS1115.
 *        This function is expected to be fast and non-blocking if called from an ISR.
 *        It assumes the ADS1115 is already configured for continuous conversion or
 *        a single-shot conversion was triggered prior and is now complete.
 * @param[out] data Pointer to store the 16-bit raw ADC value.
 * @return ADS1115_OK if data was read successfully, error code otherwise.
 */
ADS1115_Status_t ADS1115_ReadRawData(int16_t *data);

#endif // ADS1115_DRIVER_H
