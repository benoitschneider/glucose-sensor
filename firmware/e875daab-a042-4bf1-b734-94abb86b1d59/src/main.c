/*
 * main.c
 * This is a placeholder for the actual firmware application.
 * The CI/CD workflow will compile this project to verify the build system.
 */

#include "config.h"
#include "drivers/gpio.h"
#include "drivers/uart.h"

// Example application entry point
int main(void)
{
    // Initialize peripherals (placeholders)
    GPIO_init();
    UART_init(UART_BAUD_RATE_DEFAULT);

    // Send a startup message
    UART_send_string("Firmware started successfully!\r\n");

    // Main application loop
    while (1)
    {
        // Add application logic here
        // For example, read sensors, process data, communicate, etc.

        // Simple delay to prevent busy-waiting too aggressively
        for (volatile int i = 0; i < 100000; i++);

        // Example: Toggle an LED or send a periodic message
        // GPIO_toggle_pin(LED_PIN);
        // UART_send_string("Heartbeat...\r\n");
    }

    return 0;
}
