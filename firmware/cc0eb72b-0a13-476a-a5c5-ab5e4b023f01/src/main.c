#include "config.h"
#include "ads1115_driver.h"
#include "sensor_sampling.h"
#include "timer_driver.h"
#include <stdio.h>

// Dummy main function for compilation. In a real system, this would initialize peripherals
// and start the main application loop.
int main(void)
{
    // Initialize the ADS1115 driver
    if (ADS1115_Init() != ADS1115_OK)
    {
        // Handle ADS1115 initialization error
        while (1);
    }

    // Initialize the sensor sampling module
    if (SENSOR_SAMPLING_Init() != SENSOR_SAMPLING_OK)
    {
        // Handle sensor sampling initialization error
        while (1);
    }

    // Initialize and start the timer for 1Hz sampling
    // The timer_driver_init function should configure the timer to call
    // SENSOR_SAMPLING_Timer_ISR every 1000ms.
    if (TIMER_DRV_Init(1000) != TIMER_DRV_OK)
    {
        // Handle timer initialization error
        while (1);
    }

    // Main application loop
    while (1)
    {
        // In a real application, other tasks would run here.
        // For demonstration, we can periodically print the latest reading.
        // Note: Printing in a real-time system should be done carefully
        // to avoid impacting timing.
        static uint32_t last_print_time = 0;
        if (TIMER_DRV_Get_Uptime_ms() - last_print_time >= 5000) // Print every 5 seconds
        {
            int16_t latest_reading;
            if (SENSOR_SAMPLING_GetLatestSensorReading(&latest_reading) == SENSOR_SAMPLING_OK)
            {
                printf("Latest Sensor Reading: %d\n", latest_reading);
            }
            last_print_time = TIMER_DRV_Get_Uptime_ms();
        }

        // Potentially put the MCU into a low-power mode here
        // __WFI(); // Wait For Interrupt
    }

    return 0;
}
