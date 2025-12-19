#ifndef TIMER_DRIVER_H
#define TIMER_DRIVER_H

#include <stdint.h>

// Define status codes for the timer driver module
typedef enum
{
    TIMER_DRV_OK = 0,
    TIMER_DRV_ERROR
} TIMER_DRV_Status_t;

/**
 * @brief Initializes the hardware timer to generate interrupts at a specified period.
 *        The ISR should call SENSOR_SAMPLING_Timer_ISR().
 * @param period_ms The period in milliseconds for the timer interrupt.
 * @return TIMER_DRV_OK if successful, error code otherwise.
 */
TIMER_DRV_Status_t TIMER_DRV_Init(uint32_t period_ms);

/**
 * @brief Gets the current system uptime in milliseconds.
 * @return Current uptime in milliseconds.
 */
uint32_t TIMER_DRV_Get_Uptime_ms(void);

/**
 * @brief Simulates a timer tick for testing purposes.
 *        In a real system, this would be handled by the hardware ISR.
 */
void TIMER_DRV_Simulate_Tick(void);

#endif // TIMER_DRIVER_H
