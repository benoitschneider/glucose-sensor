#ifndef GLUCOSE_FILTER_H
#define GLUCOSE_FILTER_H

#include <stdint.h>
#include "config.h"

// Define status codes for filter operations
typedef enum {
    GLUCOSE_FILTER_OK = 0,
    GLUCOSE_FILTER_INVALID_PARAM,
    GLUCOSE_FILTER_NVM_ERROR
} GlucoseFilter_Status_t;

/**
 * @brief Initializes the glucose filter module.
 *        Loads the moving average window size from non-volatile memory.
 */
void GlucoseFilter_Init(void);

/**
 * @brief Sets the moving average filter window size.
 *        The new window size is stored in non-volatile memory.
 *        Resets the filter buffer upon successful update.
 * @param window_size The desired window size (between MIN and MAX defined in config.h).
 * @return GLUCOSE_FILTER_OK if successful, GLUCOSE_FILTER_INVALID_PARAM if window_size is out of range,
 *         GLUCOSE_FILTER_NVM_ERROR if NVM write fails.
 */
GlucoseFilter_Status_t GlucoseFilter_SetMovingAverageWindow(uint8_t window_size);

/**
 * @brief Gets the current moving average filter window size.
 * @return The current window size.
 */
uint8_t GlucoseFilter_GetMovingAverageWindow(void);

/**
 * @brief Processes a new raw glucose value and returns the smoothed value.
 *        Uses a circular buffer for efficient moving average calculation.
 * @param raw_glucose_value The new raw glucose value to add to the filter (e.g., in mg/dL).
 * @return The smoothed glucose value. If fewer than 'window_size' samples are available,
 *         it returns the average of the available samples.
 */
int16_t GlucoseFilter_ProcessValue(int16_t raw_glucose_value);

// --- Mock NVM API (for compilation) ---
// In a real system, this would be a separate NVM driver header.
// For this example, it's included here to make the example self-contained.

typedef enum {
    NVM_OK = 0,
    NVM_INVALID_PARAM,
    NVM_WRITE_ERROR,
    NVM_READ_ERROR
} NVM_Status_t;

void NVM_Init(void);
NVM_Status_t NVM_Read(uint16_t address, uint8_t *data, uint16_t length);
NVM_Status_t NVM_Write(uint16_t address, const uint8_t *data, uint16_t length);

#endif // GLUCOSE_FILTER_H
