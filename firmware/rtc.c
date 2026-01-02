#include "rtc.h"
#include <stdbool.h>

// Simple software-based RTC for demonstration purposes.
// In a real embedded system, this would interface with a hardware RTC peripheral.
static volatile uint32_t s_current_time_seconds = 0;
static bool s_rtc_initialized = false;

/**
 * @brief Initializes the Real-Time Clock (RTC) module.
 *        Sets up the RTC for timekeeping.
 */
void RTC_Init(void)
{
    if (!s_rtc_initialized) {
        // In a real system, configure RTC hardware here.
        // For simulation, we just mark it as initialized.
        s_current_time_seconds = 0; // Start time from 0 or load from backup register
        s_rtc_initialized = true;
        // Potentially set up a timer interrupt to increment s_current_time_seconds every second.
        // For this example, we'll assume it's incremented externally or by a mock.
    }
}

/**
 * @brief Gets the current time in seconds since epoch (or device power-on).
 * @return Current time in seconds.
 */
uint32_t RTC_GetTimeSeconds(void)
{
    // In a real system, read from RTC hardware register.
    // For this example, we return our internal counter.
    return s_current_time_seconds;
}

/**
 * @brief Sets the current time of the RTC.
 * @param time_seconds The time to set in seconds.
 */
void RTC_SetTimeSeconds(uint32_t time_seconds)
{
    // In a real system, write to RTC hardware register.
    s_current_time_seconds = time_seconds;
}

// --- Mock function for incrementing time (for testing/simulation) ---
// In a real system, this would be handled by an RTC interrupt.
void RTC_MockIncrementTime(uint32_t seconds_to_add)
{
    s_current_time_seconds += seconds_to_add;
}
