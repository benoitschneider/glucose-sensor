#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <stdint.h>

typedef enum
{
    POWER_MODE_ACTIVE,
    POWER_MODE_DEEP_SLEEP
} power_mode_t;

/**@brief Function to set the current power mode of the device.
 * @param[in] new_state The desired power mode.
 */
void power_manager_set_state(power_mode_t new_state);

/**@brief Function to get the current power mode of the device.
 * @return The current power mode.
 */
power_mode_t power_manager_get_state(void);

/**@brief Function to set the next RTC wake-up time.
 * @param[in] seconds_from_now The number of seconds from now to wake up.
 */
void power_manager_set_next_wakeup_time(uint32_t seconds_from_now);

/**@brief Function to be called in the main loop to manage power states.
 */
void power_manager_run(void);

#endif // POWER_MANAGER_H
