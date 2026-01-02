/**
 * @file utils.c
 * @brief Common utility functions for the Glucose Sensor Firmware.
 */

#include "utils.h"

/**
 * @brief Simple delay function (blocking).
 * @param[in] ms Milliseconds to delay.
 */
void common_delay_ms(uint32_t ms)
{
    // In a real application, this would use a timer or RTOS delay.
    // For nRF, nrf_delay_ms() from nrf_delay.h is available.
    // This is just a placeholder.
    volatile uint32_t i;
    for (i = 0; i < ms * 10000; i++) // Approximate cycles per ms
    {
        __NOP(); // No operation
    }
}

/**
 * @brief Example utility function: clamps a value within a range.
 * @param[in] value The value to clamp.
 * @param[in] min The minimum allowed value.
 * @param[in] max The maximum allowed value.
 * @return The clamped value.
 */
int32_t common_clamp_int32(int32_t value, int32_t min, int32_t max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
