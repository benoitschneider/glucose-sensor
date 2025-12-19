#ifndef CONFIG_H
#define CONFIG_H

// --- Glucose Filter Configuration ---

/**
 * @brief Defines the window size for the Moving Average filter.
 *        A larger window provides more smoothing but introduces more latency.
 *        Must be a positive integer.
 */
#define GLUCOSE_FILTER_WINDOW_SIZE 5

/**
 * @brief Configuration flag to switch between filtered and unfiltered output.
 *        Set to 1 to enable filtered output, 0 for raw output.
 *        This is primarily for testing and debugging.
 */
#define USE_FILTERED_GLUCOSE_OUTPUT 1

// --- Other System Configurations (placeholders) ---

// Define if using an RTOS for thread safety
// #define USE_RTOS_FOR_THREAD_SAFETY 1

#endif // CONFIG_H
