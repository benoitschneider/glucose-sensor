#ifndef RTC_H
#define RTC_H

#include <stdint.h>

/**
 * @brief Initializes the Real-Time Clock (RTC) module.
 *        Sets up the RTC for timekeeping.
 */
void RTC_Init(void);

/**
 * @brief Gets the current time in seconds since epoch (or device power-on).
 * @return Current time in seconds.
 */
uint32_t RTC_GetTimeSeconds(void);

/**
 * @brief Sets the current time of the RTC.
 * @param time_seconds The time to set in seconds.
 */
void RTC_SetTimeSeconds(uint32_t time_seconds);

#endif // RTC_H
