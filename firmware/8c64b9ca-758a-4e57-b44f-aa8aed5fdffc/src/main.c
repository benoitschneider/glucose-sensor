#include "config.h"
#include "glucose_filter.h"
#include "nvm.h"
#include <stdio.h>

// Mock function for reading raw glucose data
// In a real system, this would come from an ADC or sensor interface
static float MOCK_read_raw_glucose(void) {
    static float mock_glucose_value = 100.0f;
    // Simulate some noise and variation
    mock_glucose_value += ((float)rand() / (float)RAND_MAX - 0.5f) * 5.0f; // +/- 2.5 fluctuation
    if (mock_glucose_value < 50.0f) mock_glucose_value = 50.0f;
    if (mock_glucose_value > 200.0f) mock_glucose_value = 200.0f;
    return mock_glucose_value;
}

// Mock function for sending smoothed glucose data to another module
// In a real system, this would update a display, send to BLE, etc.
static void MOCK_send_smoothed_glucose(float smoothed_value) {
    printf("Smoothed Glucose: %.2f\n", smoothed_value);
}

int main(void) {
    printf("Initializing Glucose Filter System\n");

    // 1. Initialize Non-Volatile Memory module
    // In a real system, this might involve checking NVM status, etc.
    NVM_Init();

    // 2. Initialize Glucose Filter module
    // This will load the parameter from NVM
    GlucoseFilter_Init();

    // Example: Read current parameter
    float current_param = GlucoseFilter_GetIIRParameter();
    printf("Initial Filter Parameter (alpha): %.4f\n", current_param);

    // Example: Change filter parameter and save it (e.g., via a command interface)
    // For demonstration, we'll change it once
    #if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
    float new_alpha = 0.1f; // A slower filter
    #elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
    float new_cutoff_freq = 0.05f; // Normalized cutoff frequency (0 to 0.5)
    #endif

    #if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
    if (new_alpha != current_param) {
        printf("Setting new filter parameter (alpha) to: %.4f\n", new_alpha);
        GlucoseFilter_SetIIRParameter(new_alpha);
        current_param = GlucoseFilter_GetIIRParameter(); // Verify it's set
        printf("Verified Filter Parameter (alpha): %.4f\n", current_param);
    }
    #elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
    if (new_cutoff_freq != current_param) {
        printf("Setting new filter parameter (normalized cutoff frequency) to: %.4f\n", new_cutoff_freq);
        GlucoseFilter_SetIIRParameter(new_cutoff_freq);
        current_param = GlucoseFilter_GetIIRParameter(); // Verify it's set
        printf("Verified Filter Parameter (normalized cutoff frequency): %.4f\n", current_param);
    }
    #endif

    // Main loop simulation
    for (int i = 0; i < 50; ++i) {
        float raw_glucose = MOCK_read_raw_glucose();
        float smoothed_glucose = GlucoseFilter_Process(raw_glucose);
        printf("Raw: %.2f, ", raw_glucose);
        MOCK_send_smoothed_glucose(smoothed_glucose);
        // In a real system, this would be timed (e.g., every second)
        // For simulation, just loop
    }

    printf("Shutting down Glucose Filter System\n");

    return 0;
}
