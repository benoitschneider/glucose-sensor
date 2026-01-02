#include <stdint.h>

// Basic delay function (blocking)
void delay_ms(uint32_t ms)
{
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 16000; j++); // Approximate 1ms delay for 16MHz clock
    }
}

int main(void)
{
    // Simple infinite loop to indicate firmware is running
    // In a real application, this would initialize peripherals and run a superloop or RTOS.
    while (1)
    {
        // Placeholder for application logic
        delay_ms(1000); // Wait for 1 second
    }
}
