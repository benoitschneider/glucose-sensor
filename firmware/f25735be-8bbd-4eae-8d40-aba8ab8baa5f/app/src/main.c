#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"           // Nordic specific registers, etc.
#include "nrf_delay.h"     // For basic delays
#include "nrf_gpio.h"      // For GPIO operations
#include "boards.h"        // For board specific definitions (e.g., LED pins)

// Placeholder for SDK configuration (e.g., from sdk_config.h)
// In a real project, this would be generated/configured by the SDK tools.
#ifndef LED_1
#define LED_1 NRF_GPIO_PIN_MAP(0,17) // Example for nRF52 DK
#endif

/**
 * @brief Function for application main entry point.
 */
int main(void)
{
    // Initialize LED pin as output
    nrf_gpio_cfg_output(LED_1);

    // Main loop
    while (true)
    {
        // Toggle LED_1
        nrf_gpio_pin_toggle(LED_1);

        // Delay for 500 milliseconds
        nrf_delay_ms(500);
    }
}
