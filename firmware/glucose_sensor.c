#include "glucose_sensor.h"
#include "config.h"
#include <stdbool.h>
#include <stdlib.h> // For rand()

// Internal state for the sensor (e.g., calibration factors)
static bool s_sensor_initialized = false;

// Mock sensor data for simulation
static float s_mock_base_glucose = 100.0f; // Initial stable glucose
static float s_mock_drift_rate = 0.01f;   // Drift rate per second (e.g., 0.01 mg/dL per second)
static uint32_t s_mock_last_read_time = 0;

/**
 * @brief Initializes the glucose sensor interface and ADC.
 *        In a real system, this would configure ADC, potentiostat, etc.
 */
void GLUCOSE_SENSOR_Init(void)
{
    if (!s_sensor_initialized) {
        // Configure ADC, GPIOs, potentiostat, etc.
        // Perform initial sensor calibration if required.
        s_sensor_initialized = true;
        s_mock_last_read_time = 0; // Reset mock time
    }
}

/**
 * @brief Reads the raw glucose value from the sensor.
 *        This function simulates reading from an ADC and converting to a glucose unit.
 *        Includes a simulated drift for testing purposes.
 * @return The raw glucose value (e.g., in mg/dL or mmol/L).
 */
float GLUCOSE_SENSOR_ReadRawGlucose(void)
{
    // In a real system:
    // 1. Trigger ADC conversion.
    // 2. Read ADC raw value.
    // 3. Apply sensor-specific calibration and conversion factors.
    //    float adc_voltage = (float)adc_raw_value * ADC_LSB_VOLTAGE;
    //    float glucose_concentration = (adc_voltage - OFFSET) * GAIN_FACTOR;

    // --- Simulation of raw glucose with drift ---
    if (!s_sensor_initialized) {
        GLUCOSE_SENSOR_Init();
    }

    // Simulate time passing and drift accumulating
    uint32_t current_sim_time = s_mock_last_read_time + GLUCOSE_SENSOR_READ_INTERVAL_SECONDS; // Assume interval passes
    float simulated_drift = s_mock_drift_rate * current_sim_time; // Linear drift

    // Add some random noise to simulate real sensor readings
    float noise = ((float)rand() / (float)RAND_MAX - 0.5f) * 2.0f; // Noise between -1 and 1
    noise *= 0.5f; // Scale noise magnitude

    float raw_glucose = s_mock_base_glucose + simulated_drift + noise;

    s_mock_last_read_time = current_sim_time;

    // Ensure non-negative glucose values
    if (raw_glucose < 0.0f) {
        raw_glucose = 0.0f;
    }

    return raw_glucose;
}

/**
 * @brief Sets the base glucose value for the mock sensor.
 *        Useful for simulating different glucose levels.
 * @param base_glucose The new base glucose value.
 */
void GLUCOSE_SENSOR_SetMockBaseGlucose(float base_glucose)
{
    s_mock_base_glucose = base_glucose;
}

/**
 * @brief Sets the drift rate for the mock sensor.
 * @param drift_rate The new drift rate per second.
 */
void GLUCOSE_SENSOR_SetMockDriftRate(float drift_rate)
{
    s_mock_drift_rate = drift_rate;
}
