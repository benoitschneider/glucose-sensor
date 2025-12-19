#include "config.h"
#include "glucose_sensor.h"
#include "glucose_filter.h"
#include <stdio.h>

// Mock function for initial glucose calculation
// In a real system, this would come from the ADC/sensor driver
static float Glucose_CalculateRaw(void)
{
    // Simulate receiving a new raw glucose reading
    static float mock_raw_glucose = 100.0f;
    mock_raw_glucose += ((float)(rand() % 100) - 50.0f) / 100.0f; // Add some noise
    if (mock_raw_glucose < 50.0f) mock_raw_glucose = 50.0f;
    if (mock_raw_glucose > 200.0f) mock_raw_glucose = 200.0f;
    return mock_raw_glucose;
}

int main(void)
{
    printf("Glucose Processing Pipeline Initialization\n");

    // Initialize the glucose filter module
    GlucoseFilter_Init();

    // Main loop (or triggered by a timer/interrupt in a real-time system)
    while (1)
    {
        // 1. Read raw sensor data (simulated)
        float raw_value = Glucose_CalculateRaw();

        // 2. Process raw value through the filter
        // This function internally updates the filter's window and calculates the new filtered value
        GlucoseFilter_ProcessNewReading(raw_value);

        // 3. Get output based on configuration
        float output_value;
        if (USE_FILTERED_GLUCOSE_OUTPUT)
        {
            output_value = Glucose_GetFilteredValue();
            printf("Raw: %.2f mg/dL, Filtered: %.2f mg/dL\n", raw_value, output_value);
        }
        else
        {
            output_value = raw_value; // For debug/testing, output raw value directly
            printf("Raw: %.2f mg/dL (Unfiltered Output)\n", raw_value);
        }

        // 4. Further processing (e.g., drift compensation, display, transmission) would happen here
        // ...

        // Simulate a delay for the next reading cycle
        // In a real system, this would be handled by a precise timer or RTOS task delay
        for (volatile long i = 0; i < 500000; i++);
    }

    return 0;
}
