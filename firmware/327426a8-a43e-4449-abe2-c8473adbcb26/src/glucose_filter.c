#include "glucose_filter.h"
#include "nvm_storage.h"
#include <string.h>

// Static buffer for the circular buffer
static int16_t s_glucose_buffer[GLUCOSE_FILTER_MAX_WINDOW_SIZE];
static uint8_t s_window_size = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
static uint8_t s_buffer_idx = 0;
static uint8_t s_current_fill_count = 0; // Tracks how many valid samples are in the buffer

/**
 * @brief Initializes the glucose filter module.
 *        Loads the window size from non-volatile memory.
 */
void GlucoseFilter_Init(void)
{
    uint8_t stored_window_size;
    // Load window size from NVM
    if (NVM_Read(NVM_KEY_FILTER_WINDOW_SIZE, &stored_window_size, sizeof(stored_window_size)) == NVM_OK)
    {
        // Validate loaded size against min/max allowed
        if (stored_window_size >= GLUCOSE_FILTER_MIN_WINDOW_SIZE &&
            stored_window_size <= GLUCOSE_FILTER_MAX_WINDOW_SIZE)
        {
            s_window_size = stored_window_size;
        }
        else
        {
            // If NVM value is invalid, use default and store it
            s_window_size = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
            NVM_Write(NVM_KEY_FILTER_WINDOW_SIZE, &s_window_size, sizeof(s_window_size));
        }
    }
    else
    {
        // If NVM read fails, use default and attempt to store it
        s_window_size = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
        NVM_Write(NVM_KEY_FILTER_WINDOW_SIZE, &s_window_size, sizeof(s_window_size));
    }

    // Clear the buffer and reset indices
    memset(s_glucose_buffer, 0, sizeof(s_glucose_buffer));
    s_buffer_idx = 0;
    s_current_fill_count = 0;
}

/**
 * @brief Sets the moving average filter window size.
 *        The new size is stored in non-volatile memory.
 * @param window_size The desired window size (must be within min/max bounds).
 * @return GLUCOSE_FILTER_OK if successful, GLUCOSE_FILTER_INVALID_PARAM otherwise.
 */
GlucoseFilter_Status_t GlucoseFilter_SetMovingAverageWindow(uint8_t window_size)
{
    // Validate the input window size
    if (window_size < GLUCOSE_FILTER_MIN_WINDOW_SIZE ||
        window_size > GLUCOSE_FILTER_MAX_WINDOW_SIZE)
    {
        return GLUCOSE_FILTER_INVALID_PARAM;
    }

    // Update the static window size
    s_window_size = window_size;

    // Persist the new window size to NVM
    if (NVM_Write(NVM_KEY_FILTER_WINDOW_SIZE, &s_window_size, sizeof(s_window_size)) != NVM_OK)
    {
        // Log error if NVM write fails, but filter will continue with new value in RAM
        return GLUCOSE_FILTER_NVM_ERROR;
    }

    // Reset the filter state to avoid issues with changed window size
    memset(s_glucose_buffer, 0, sizeof(s_glucose_buffer));
    s_buffer_idx = 0;
    s_current_fill_count = 0;

    return GLUCOSE_FILTER_OK;
}

/**
 * @brief Gets the current moving average filter window size.
 * @return The current window size.
 */
uint8_t GlucoseFilter_GetMovingAverageWindow(void)
{
    return s_window_size;
}

/**
 * @brief Processes a new raw glucose value through the moving average filter.
 * @param raw_value The raw glucose value to filter.
 * @return The smoothed glucose value.
 */
int16_t GlucoseFilter_ProcessValue(int16_t raw_value)
{
    // Add the new value to the circular buffer
    s_glucose_buffer[s_buffer_idx] = raw_value;

    // Increment buffer index, wrap around if necessary
    s_buffer_idx = (s_buffer_idx + 1) % s_window_size;

    // Increment fill count, cap at window size
    if (s_current_fill_count < s_window_size)
    {
        s_current_fill_count++;
    }

    // Calculate the sum of values in the currently filled part of the buffer
    int32_t sum = 0;
    for (uint8_t i = 0; i < s_current_fill_count; ++i)
    {
        sum += s_glucose_buffer[i];
    }

    // Calculate the average. Handle division by zero if s_current_fill_count is 0 (should not happen after first sample)
    if (s_current_fill_count == 0)
    {
        return raw_value; // Return raw value if no samples yet (edge case, though s_current_fill_count starts at 0)
    }
    else
    {
        return (int16_t)(sum / s_current_fill_count);
    }
}

// Mock NVM functions for demonstration purposes
// In a real system, these would interface with actual flash memory or EEPROM.

// Simple in-memory NVM simulation
static uint8_t s_nvm_storage[NVM_STORAGE_SIZE];

void NVM_Init(void)
{
    // Simulate NVM being cleared or having default values on first boot
    // For this mock, we'll just ensure it's accessible.
    // In a real system, you might read a checksum or magic number here.
    if (s_nvm_storage[NVM_KEY_FILTER_WINDOW_SIZE] == 0xFF) // Simulate uninitialized NVM
    {
        s_nvm_storage[NVM_KEY_FILTER_WINDOW_SIZE] = GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE;
    }
}

NVM_Status_t NVM_Read(uint8_t key, void *data, size_t len)
{
    if (key >= NVM_STORAGE_SIZE || (key + len) > NVM_STORAGE_SIZE)
    {
        return NVM_INVALID_KEY;
    }
    memcpy(data, &s_nvm_storage[key], len);
    return NVM_OK;
}

NVM_Status_t NVM_Write(uint8_t key, const void *data, size_t len)
{
    if (key >= NVM_STORAGE_SIZE || (key + len) > NVM_STORAGE_SIZE)
    {
        return NVM_INVALID_KEY;
    }
    memcpy(&s_nvm_storage[key], data, len);
    return NVM_OK;
}
