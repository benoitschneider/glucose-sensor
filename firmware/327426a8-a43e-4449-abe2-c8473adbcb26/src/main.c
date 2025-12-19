#include "config.h"
#include "glucose_filter.h"
#include "nvm_storage.h"
#include <stdio.h>

// Mock function for getting raw glucose values
// In a real system, this would come from an ADC or sensor module
static int16_t get_raw_glucose_value(void)
{
    static int16_t mock_glucose_data[] = {
        100, 102, 101, 105, 103, 108, 106, 110, 109, 112,
        111, 115, 113, 117, 116, 120, 118, 122, 121, 125,
        123, 127, 126, 130, 128, 132, 131, 135, 133, 137,
        136, 140, 138, 142, 141, 145, 143, 147, 146, 150,
        148, 152, 151, 155, 153, 157, 156, 160, 158, 162
    };
    static uint32_t index = 0;
    if (index >= sizeof(mock_glucose_data) / sizeof(mock_glucose_data[0]))
    {
        index = 0; // Loop data for demonstration
    }
    return mock_glucose_data[index++];
}

int main(void)
{
    printf("Glucose Filter Firmware Initializing...\n");

    // Initialize Non-Volatile Memory module
    NVM_Init();

    // Initialize Glucose Filter module
    GlucoseFilter_Init();

    // Demonstrate setting and getting window size
    printf("Initial Filter Window Size: %u\n", GlucoseFilter_GetMovingAverageWindow());

    // Try setting a new window size and persist it
    uint8_t new_window_size = 5;
    if (GlucoseFilter_SetMovingAverageWindow(new_window_size) == GLUCOSE_FILTER_OK)
    {
        printf("Set Filter Window Size to: %u\n", new_window_size);
    }
    else
    {
        printf("Failed to set Filter Window Size to: %u\n", new_window_size);
    }

    // Simulate reading and filtering glucose values
    for (int i = 0; i < 20; ++i)
    {
        int16_t raw_value = get_raw_glucose_value();
        int16_t filtered_value = GlucoseFilter_ProcessValue(raw_value);
        printf("Raw: %d, Filtered: %d\n", raw_value, filtered_value);
    }

    // Simulate power cycling by re-initializing and checking window size
    printf("\nSimulating Power Cycle...\n");
    GlucoseFilter_Init(); // Re-initialize, should load from NVM
    printf("Window Size after power cycle: %u\n", GlucoseFilter_GetMovingAverageWindow());

    // Test with an invalid window size
    if (GlucoseFilter_SetMovingAverageWindow(GLUCOSE_FILTER_MAX_WINDOW_SIZE + 1) != GLUCOSE_FILTER_OK)
    {
        printf("Attempted to set invalid window size. Operation failed as expected.\n");
    }

    while (1)
    {
        // Main application loop
        // In a real system, this would involve periodic sensor reads,
        // data processing, and communication.
    }

    return 0;
}
