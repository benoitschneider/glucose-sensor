#include "glucose_filter.h"
#include "nvm.h"
#include "config.h"
#include <stddef.h>

// Static buffer for glucose readings (circular buffer)
static int16_t glucose_buffer[GLUCOSE_FILTER_MAX_WINDOW_SIZE];
// Current index in the circular buffer
static uint8_t buffer_idx = 0;
// Number of valid samples currently in the buffer
static uint8_t current_sample_count = 0;
// Configured moving average window size
static uint8_t moving_average_window_size = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;

/**
 * @brief Initializes the glucose filter module.
 *        Loads the moving average window size from non-volatile memory.
 */
void GlucoseFilter_Init(void) {
    uint8_t stored_window_size;
    // Attempt to load window size from NVM
    if (NVM_Read(NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE, &stored_window_size, sizeof(stored_window_size)) == NVM_OK) {
        // Validate loaded window size
        if (stored_window_size >= GLUCOSE_FILTER_MIN_WINDOW_SIZE &&
            stored_window_size <= GLUCOSE_FILTER_MAX_WINDOW_SIZE) {
            moving_average_window_size = stored_window_size;
        } else {
            // If NVM value is invalid, use default and store it
            moving_average_window_size = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
            (void)NVM_Write(NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE, &moving_average_window_size, sizeof(moving_average_window_size));
        }
    } else {
        // If NVM read fails, use default and store it
        moving_average_window_size = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
        (void)NVM_Write(NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE, &moving_average_window_size, sizeof(moving_average_window_size));
    }

    // Clear the buffer on initialization
    for (uint8_t i = 0; i < GLUCOSE_FILTER_MAX_WINDOW_SIZE; i++) {
        glucose_buffer[i] = 0;
    }
    buffer_idx = 0;
    current_sample_count = 0;
}

/**
 * @brief Sets the moving average filter window size.
 *        The new window size is stored in non-volatile memory.
 * @param window_size The desired window size (between MIN and MAX).
 * @return GLUCOSE_FILTER_OK if successful, GLUCOSE_FILTER_INVALID_PARAM otherwise.
 */
GlucoseFilter_Status_t GlucoseFilter_SetMovingAverageWindow(uint8_t window_size) {
    if (window_size < GLUCOSE_FILTER_MIN_WINDOW_SIZE ||
        window_size > GLUCOSE_FILTER_MAX_WINDOW_SIZE) {
        return GLUCOSE_FILTER_INVALID_PARAM;
    }

    // Update the internal window size
    moving_average_window_size = window_size;

    // Persist the new window size to NVM
    if (NVM_Write(NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE, &moving_average_window_size, sizeof(moving_average_window_size)) != NVM_OK) {
        // Handle NVM write error if necessary (e.g., log, return an error status)
        return GLUCOSE_FILTER_NVM_ERROR;
    }

    // When window size changes, reset the buffer to avoid averaging with old data from a different window size
    for (uint8_t i = 0; i < GLUCOSE_FILTER_MAX_WINDOW_SIZE; i++) {
        glucose_buffer[i] = 0;
    }
    buffer_idx = 0;
    current_sample_count = 0;

    return GLUCOSE_FILTER_OK;
}

/**
 * @brief Gets the current moving average filter window size.
 * @return The current window size.
 */
uint8_t GlucoseFilter_GetMovingAverageWindow(void) {
    return moving_average_window_size;
}

/**
 * @brief Processes a new raw glucose value and returns the smoothed value.
 *        Uses a circular buffer for efficient moving average calculation.
 * @param raw_glucose_value The new raw glucose value to add to the filter.
 * @return The smoothed glucose value. Returns the raw value if fewer than window_size samples are available.
 */
int16_t GlucoseFilter_ProcessValue(int16_t raw_glucose_value) {
    // Store the new value in the circular buffer
    glucose_buffer[buffer_idx] = raw_glucose_value;

    // Increment buffer index and wrap around if necessary
    buffer_idx = (buffer_idx + 1) % moving_average_window_size;

    // Increment sample count, up to the window size
    if (current_sample_count < moving_average_window_size) {
        current_sample_count++;
    }

    // Calculate the sum of values in the current window
    int32_t sum = 0;
    for (uint8_t i = 0; i < current_sample_count; i++) {
        sum += glucose_buffer[i];
    }

    // Handle edge case: fewer than 'window_size' samples available
    if (current_sample_count < moving_average_window_size) {
        // Return the simple average of available samples, or raw value if only one
        if (current_sample_count == 0) return 0; // Should not happen if a value is processed
        return (int16_t)(sum / current_sample_count);
    } else {
        // Return the moving average
        return (int16_t)(sum / moving_average_window_size);
    }
}

// --- Mock NVM Module (for compilation) ---
// In a real system, this would be replaced by actual NVM driver (EEPROM, Flash, etc.)
static uint8_t mock_nvm_storage[NVM_TOTAL_SIZE];

void NVM_Init(void) {
    // Simulate NVM initialization, e.g., checking for first boot, default values
    // For this mock, we just ensure the mock storage is cleared or initialized
    for (uint16_t i = 0; i < NVM_TOTAL_SIZE; i++) {
        mock_nvm_storage[i] = 0xFF; // Simulate erased flash
    }
    // Set a default window size in mock NVM if it's 'erased'
    if (mock_nvm_storage[NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE] == 0xFF) {
        mock_nvm_storage[NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE] = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
    }
}

NVM_Status_t NVM_Read(uint16_t address, uint8_t *data, uint16_t length) {
    if (address + length > NVM_TOTAL_SIZE || data == NULL) {
        return NVM_INVALID_PARAM;
    }
    for (uint16_t i = 0; i < length; i++) {
        data[i] = mock_nvm_storage[address + i];
    }
    return NVM_OK;
}

NVM_Status_t NVM_Write(uint16_t address, const uint8_t *data, uint16_t length) {
    if (address + length > NVM_TOTAL_SIZE || data == NULL) {
        return NVM_INVALID_PARAM;
    }
    for (uint16_t i = 0; i < length; i++) {
        mock_nvm_storage[address + i] = data[i];
    }
    return NVM_OK;
}
