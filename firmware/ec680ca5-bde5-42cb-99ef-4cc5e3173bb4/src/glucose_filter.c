#include "glucose_filter.h"
#include "config.h"
#include <stddef.h> // For NULL
#include <string.h> // For memset

// Static buffer to hold raw glucose values for the moving average window
static float glucose_readings_buffer[GLUCOSE_FILTER_WINDOW_SIZE];
static size_t buffer_index = 0; // Current insertion point
static size_t readings_count = 0; // Number of valid readings currently in the buffer
static float current_filtered_value = 0.0f;

// Static flag for initialization status
static bool is_initialized = false;

// Mutex for thread safety if GlucoseFilter_ProcessNewReading and Glucose_GetFilteredValue
// are called from different threads/contexts. For a single-threaded embedded system
// or if protected by a global lock, this might be simplified.
// For demonstration, we'll assume a single-threaded context or external synchronization.
// If multi-threaded, uncomment and implement proper mutex (e.g., FreeRTOS mutex_t)
// #include "rtos_mutex.h" // Placeholder for RTOS mutex header
// static rtos_mutex_t glucose_filter_mutex;

/**
 * @brief Initializes the glucose filter module.
 *        Resets the buffer and internal state.
 */
void GlucoseFilter_Init(void)
{
    // Initialize buffer with zeros
    memset(glucose_readings_buffer, 0, sizeof(glucose_readings_buffer));
    buffer_index = 0;
    readings_count = 0;
    current_filtered_value = 0.0f;

    // If multi-threaded, initialize mutex here
    // rtos_mutex_init(&glucose_filter_mutex);

    is_initialized = true;
}

/**
 * @brief Processes a new raw glucose reading and updates the filter.
 *        This function implements a simple Moving Average filter.
 * @param new_raw_glucose The latest raw glucose value.
 */
void GlucoseFilter_ProcessNewReading(float new_raw_glucose)
{
    if (!is_initialized)
    {
        // Handle error: Filter not initialized. Perhaps log an error or assert.
        // For production, ensure initialization happens before this is called.
        return;
    }

    // Acquire mutex if multi-threaded
    // rtos_mutex_lock(&glucose_filter_mutex);

    // Store the new reading in the circular buffer
    glucose_readings_buffer[buffer_index] = new_raw_glucose;
    buffer_index = (buffer_index + 1) % GLUCOSE_FILTER_WINDOW_SIZE;

    // Increment count of valid readings until the buffer is full
    if (readings_count < GLUCOSE_FILTER_WINDOW_SIZE)
    {
        readings_count++;
    }

    // Calculate the moving average
    float sum = 0.0f;
    for (size_t i = 0; i < readings_count; i++)
    {
        sum += glucose_readings_buffer[i];
    }

    if (readings_count > 0)
    {
        current_filtered_value = sum / (float)readings_count;
    }
    else
    {
        current_filtered_value = new_raw_glucose; // Should not happen if readings_count > 0 check is correct
    }

    // Release mutex if multi-threaded
    // rtos_mutex_unlock(&glucose_filter_mutex);
}

/**
 * @brief Retrieves the current filtered glucose value.
 * @return The latest filtered glucose value.
 */
float Glucose_GetFilteredValue(void)
{
    if (!is_initialized)
    {
        // Handle error: Filter not initialized.
        return 0.0f; // Return a default/error value
    }

    // Acquire mutex if multi-threaded
    // rtos_mutex_lock(&glucose_filter_mutex);

    float value_to_return = current_filtered_value;

    // Release mutex if multi-threaded
    // rtos_mutex_unlock(&glucose_filter_mutex);

    return value_to_return;
}
