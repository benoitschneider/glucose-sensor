/**
 * @file utils.h
 * @brief Header for common utility functions.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/**
 * @brief Simple delay function (blocking).
 * @param[in] ms Milliseconds to delay.
 */
void common_delay_ms(uint32_t ms);

/**
 * @brief Example utility function: clamps a value within a range.
 * @param[in] value The value to clamp.
 * @param[in] min The minimum allowed value.
 * @param[in] max The maximum allowed value.
 * @return The clamped value.
 */
int32_t common_clamp_int32(int32_t value, int32_t min, int32_t max);

#endif // UTILS_H
