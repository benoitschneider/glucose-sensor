#include "config.h"
#include "glucose_filter.h"
#include "nvm.h"

// Mock function for getting raw glucose values. In a real system, this would come from an ADC.
static int16_t MOCK_GetRawGlucoseValue(void) {
    static int16_t mock_value = 100; // Start with a baseline
    // Simulate some noise and variation
    mock_value += (rand() % 11) - 5; // +/- 5 variation
    if (mock_value < 50) mock_value = 50;
    if (mock_value > 200) mock_value = 200;
    return mock_value;
}

int main(void) {
    // Initialize Non-Volatile Memory module
    NVM_Init();

    // Initialize the glucose filter module. This will load the window size from NVM.
    GlucoseFilter_Init();

    // Example usage loop
    while (1) {
        // Get a raw glucose value (mocked)
        int16_t raw_glucose = MOCK_GetRawRawGlucoseValue();

        // Add the new raw value to the filter and get the smoothed output
        int16_t smoothed_glucose = GlucoseFilter_ProcessValue(raw_glucose);

        // Print or use the smoothed value (for demonstration)
        // In a real system, this might be sent to a display, BLE module, etc.
        // For now, we'll just simulate some processing.
        (void)smoothed_glucose; // Suppress unused variable warning

        // Simulate some delay or other tasks
        for (volatile long i = 0; i < 100000; i++);

        // Example of changing window size at runtime (e.g., via a command or user input)
        // This would typically be triggered by an external event, not in a tight loop.
        static uint8_t counter = 0;
        counter++;
        if (counter == 50) {
            // Change window size to 10 after 50 samples
            GlucoseFilter_SetMovingAverageWindow(10);
        } else if (counter == 100) {
            // Change window size back to 5 after another 50 samples
            GlucoseFilter_SetMovingAverageWindow(5);
            counter = 0; // Reset counter
        }
    }

    return 0;
}
