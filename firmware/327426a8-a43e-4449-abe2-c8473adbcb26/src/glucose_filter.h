#ifndef GLUCOSE_FILTER_H
#define GLUCOSE_FILTER_H

#include <stdint.h>
#include <stddef.h>

// Define filter configuration parameters
#define GLUCOSE_FILTER_MIN_WINDOW_SIZE      ((uint8_t)2)   // Minimum window size
#define GLUCOSE_FILTER_MAX_WINDOW_SIZE      ((uint8_t)10)  // Maximum window size
#define GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE  ((uint8_t)4)   // Default window size

/**
 * @brief Status codes for glucose filter operations.
 */
typedef enum
{
    GLUCOSE_FILTER_OK = 0,             /**< Operation successful */
    GLUCOSE_FILTER_INVALID_PARAM,      /**< Invalid parameter provided */
    GLUCOSE_FILTER_NVM_ERROR           /**< Non-volatile memory operation failed */
} GlucoseFilter_Status_t;

/**
 * @brief Initializes the glucose filter module.
 *        Loads the window size from non-volatile memory and resets the filter state.
 */
void GlucoseFilter_Init(void);

/**
 * @brief Sets the moving average filter window size.
 *        The new size is stored in non-volatile memory for persistence.
 *        If the window size is changed, the filter's internal buffer is reset.
 * @param window_size The desired window size. Must be between GLUCOSE_FILTER_MIN_WINDOW_SIZE
 *                    and GLUCOSE_FILTER_MAX_WINDOW_SIZE (inclusive).
 * @return GLUCOSE_FILTER_OK if successful,
 *         GLUCOSE_FILTER_INVALID_PARAM if the window_size is out of bounds,
 *         GLUCOSE_FILTER_NVM_ERROR if NVM write fails.
 */
GlucoseFilter_Status_t GlucoseFilter_SetMovingAverageWindow(uint8_t window_size);

/**
 * @brief Gets the current moving average filter window size.
 * @return The current window size.
 */
uint8_t GlucoseFilter_GetMovingAverageWindow(void);

/**
 * @brief Processes a new raw glucose value through the moving average filter.
 *        This function uses a circular buffer to efficiently calculate the moving average.
 *        Handles edge cases where fewer than 'window_size' samples are available by
 *        averaging only the available samples.
 * @param raw_value The raw glucose value (e.g., from an ADC) to be filtered.
 * @return The smoothed glucose value after applying the moving average filter.
 */
int16_t GlucoseFilter_ProcessValue(int16_t raw_value);

// --- Mock NVM Storage Interface (for demonstration) ---
// In a real system, this would be a separate NVM driver module.

/**
 * @brief Keys for NVM storage.
 */
typedef enum
{
    NVM_KEY_FILTER_WINDOW_SIZE = 0, // Key for storing filter window size
    // Add other NVM keys here
    NVM_STORAGE_SIZE                // Total size of NVM storage (for mock)
} NVM_Key_t;

/**
 * @brief Status codes for NVM operations.
 */
typedef enum
{
    NVM_OK = 0,
    NVM_INVALID_KEY,
    NVM_WRITE_ERROR,
    NVM_READ_ERROR
} NVM_Status_t;

/**
 * @brief Initializes the NVM storage module.
 */
void NVM_Init(void);

/**
 * @brief Reads data from NVM.
 * @param key The NVM key.
 * @param data Pointer to buffer to store read data.
 * @param len Length of data to read.
 * @return NVM_OK if successful, error code otherwise.
 */
NVM_Status_t NVM_Read(uint8_t key, void *data, size_t len);

/**
 * @brief Writes data to NVM.
 * @param key The NVM key.
 * @param data Pointer to data to write.
 * @param len Length of data to write.
 * @return NVM_OK if successful, error code otherwise.
 */
NVM_Status_t NVM_Write(uint8_t key, const void *data, size_t len);

#endif // GLUCOSE_FILTER_H
