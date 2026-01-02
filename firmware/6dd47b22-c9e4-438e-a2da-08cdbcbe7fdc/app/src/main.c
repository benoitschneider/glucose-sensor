#include <stdint.h>

/**
 * @brief Main application entry point.
 *        This function initializes the system and enters an infinite loop.
 *        For this basic 'hello world' build, it simply idles.
 */
int main(void)
{
    // Basic initialization (e.g., disable interrupts, set up watchdog if needed)
    // For a minimal build, we assume the startup code handles basic CPU setup.

    // Infinite loop to keep the microcontroller running.
    // In a real application, this loop would contain the main task scheduler
    // or power-saving modes.
    while (1)
    {
        // NOP (No Operation) to prevent busy-waiting from consuming too much power
        // if the compiler doesn't optimize an empty loop away.
        __asm__ volatile ("nop");
    }

    // Should never be reached
    return 0;
}
