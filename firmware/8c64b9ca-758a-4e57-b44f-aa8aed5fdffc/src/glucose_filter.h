#ifndef GLUCOSE_FILTER_H
#define GLUCOSE_FILTER_H

#include <stdint.h>

// --- Public Defines --- //

// Status codes for filter operations
typedef enum {
    GLUCOSE_FILTER_OK = 0,
    GLUCOSE_FILTER_ERROR_INVALID_PARAM,
    GLUCOSE_FILTER_ERROR_NVM_WRITE_FAIL,
    GLUCOSE_FILTER_ERROR_NVM_READ_FAIL
} GlucoseFilter_Status_t;

// --- Public API Functions --- //

/**
 * @brief Initializes the glucose filter module.
 *        Loads the filter parameter from NVM and calculates coefficients.
 *        Must be called once at system startup after NVM_Init().
 */
void GlucoseFilter_Init(void);

/**
 * @brief Sets the filter parameter (e.g., alpha for EMA, cutoff frequency for Butterworth).
 *        The parameter is saved to non-volatile memory and filter state is reset.
 * @param parameter The new filter parameter.
 * @return GLUCOSE_FILTER_OK if successful, or an error code.
 */
GlucoseFilter_Status_t GlucoseFilter_SetIIRParameter(float parameter);

/**
 * @brief Gets the current filter parameter.
 * @return The current filter parameter.
 */
float GlucoseFilter_GetIIRParameter(void);

/**
 * @brief Processes a raw glucose value through the configured IIR filter.
 * @param raw_glucose The current raw glucose reading.
 * @return The smoothed glucose reading.
 */
float GlucoseFilter_Process(float raw_glucose);

#endif // GLUCOSE_FILTER_H
