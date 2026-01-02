#include "drift_compensation.h"
#include "config.h"
#include "log.h"
#include <math.h>
#include <string.h>

// Internal state for the drift compensation algorithm
typedef struct {
    float historical_raw_readings[DRIFT_COMP_HISTORY_SIZE];
    float historical_compensated_readings[DRIFT_COMP_HISTORY_SIZE];
    uint16_t history_idx;
    bool history_full;
    float current_drift_offset;
    uint32_t last_compensation_time;
    float last_stable_reference_glucose;
    uint32_t last_stable_reference_time;
} drift_comp_state_t;

static drift_comp_state_t drift_state;

// Kalman Filter state variables (simplified for demonstration)
// P_k: estimate error covariance
// Q: process noise covariance
// R: measurement noise covariance
// K_k: Kalman gain
// X_k: estimated state (drift offset)
static float P_k = DRIFT_COMP_KALMAN_P_INIT;
static float Q = DRIFT_COMP_KALMAN_Q;
static float R = DRIFT_COMP_KALMAN_R;
static float X_k = 0.0f; // Initial drift offset estimate

/**
 * @brief Initializes the drift compensation module.
 */
void DRIFT_COMP_Init(void)
{
    memset(&drift_state, 0, sizeof(drift_comp_state_t));
    drift_state.current_drift_offset = 0.0f; // Start with no offset
    drift_state.last_compensation_time = 0;
    drift_state.last_stable_reference_glucose = 0.0f; // No stable reference initially
    drift_state.last_stable_reference_time = 0;
    X_k = 0.0f; // Reset Kalman filter state
    P_k = DRIFT_COMP_KALMAN_P_INIT;
    LOG_LogMessage("Drift Compensation Initialized.");
}

/**
 * @brief Updates the Kalman filter with a new measurement.
 *        Estimates the current drift offset.
 * @param measurement The observed drift (e.g., difference from a reference).
 * @return The estimated drift offset.
 */
static float DRIFT_COMP_KalmanUpdate(float measurement)
{
    // Prediction step
    // X_k_minus = X_k (assuming drift is constant or slowly changing)
    // P_k_minus = P_k + Q
    P_k = P_k + Q;

    // Update step
    // K_k = P_k_minus / (P_k_minus + R)
    float K_k = P_k / (P_k + R);

    // X_k = X_k_minus + K_k * (measurement - X_k_minus)
    X_k = X_k + K_k * (measurement - X_k);

    // P_k = (1 - K_k) * P_k_minus
    P_k = (1 - K_k) * P_k;

    return X_k;
}

/**
 * @brief Applies drift compensation to a raw glucose value.
 *        This implementation uses a simple adaptive offset and a Kalman filter for refinement.
 * @param raw_glucose The raw glucose reading from the sensor.
 * @param current_time_seconds The current time in seconds from RTC.
 * @return The compensated glucose value.
 */
float DRIFT_COMP_ApplyCompensation(float raw_glucose, uint32_t current_time_seconds)
{
    // Store historical readings
    drift_state.historical_raw_readings[drift_state.history_idx] = raw_glucose;
    drift_state.historical_compensated_readings[drift_state.history_idx] = raw_glucose - drift_state.current_drift_offset; // Store uncompensated for initial history
    drift_state.history_idx = (drift_state.history_idx + 1) % DRIFT_COMP_HISTORY_SIZE;
    if (drift_state.history_idx == 0) {
        drift_state.history_full = true;
    }

    // Check for stable glucose conditions to trigger adaptive calibration
    // This is a placeholder for a more sophisticated stable condition detection
    // E.g., low standard deviation over a window, or external reference measurement.
    bool is_stable_condition = false;
    if (drift_state.history_full) {
        float sum_raw = 0.0f;
        for (uint16_t i = 0; i < DRIFT_COMP_HISTORY_SIZE; i++) {
            sum_raw += drift_state.historical_raw_readings[i];
        }
        float avg_raw = sum_raw / DRIFT_COMP_HISTORY_SIZE;

        float sum_sq_diff = 0.0f;
        for (uint16_t i = 0; i < DRIFT_COMP_HISTORY_SIZE; i++) {
            sum_sq_diff += (drift_state.historical_raw_readings[i] - avg_raw) * (drift_state.historical_raw_readings[i] - avg_raw);
        }
        float std_dev_raw = sqrtf(sum_sq_diff / DRIFT_COMP_HISTORY_SIZE);

        if (std_dev_raw < DRIFT_COMP_STABILITY_THRESHOLD_GLUCOSE && avg_raw > DRIFT_COMP_MIN_GLUCOSE_FOR_STABILITY) {
            is_stable_condition = true;
            drift_state.last_stable_reference_glucose = avg_raw; // Use average as a temporary stable reference
            drift_state.last_stable_reference_time = current_time_seconds;
        }
    }

    // Apply compensation periodically or when stable conditions are met
    if (is_stable_condition && 
        (current_time_seconds - drift_state.last_compensation_time >= DRIFT_COMP_ADAPTIVE_INTERVAL_SECONDS || 
         drift_state.last_compensation_time == 0)) 
    {
        // If we have a stable reference, calculate the observed drift
        // This assumes the 'true' glucose is the stable reference, and the raw reading has drifted.
        // In a real system, a true stable reference would come from an internal sensor or external calibration.
        if (drift_state.last_stable_reference_glucose > 0.0f) {
            float observed_drift = drift_state.last_stable_reference_glucose - raw_glucose;
            
            // Use Kalman filter to estimate the drift offset
            drift_state.current_drift_offset = DRIFT_COMP_KalmanUpdate(observed_drift);

            // Clamp the offset to prevent extreme values
            if (drift_state.current_drift_offset > DRIFT_COMP_MAX_OFFSET) {
                drift_state.current_drift_offset = DRIFT_COMP_MAX_OFFSET;
            } else if (drift_state.current_drift_offset < -DRIFT_COMP_MAX_OFFSET) {
                drift_state.current_drift_offset = -DRIFT_COMP_MAX_OFFSET;
            }
            
            LOG_LogDriftAdjustment(current_time_seconds, observed_drift, drift_state.current_drift_offset);
            drift_state.last_compensation_time = current_time_seconds;
        }
    }

    // Apply the current drift offset
    float compensated_glucose = raw_glucose + drift_state.current_drift_offset;

    // Ensure glucose values are non-negative
    if (compensated_glucose < 0.0f) {
        compensated_glucose = 0.0f;
    }

    return compensated_glucose;
}

/**
 * @brief Retrieves the current drift offset being applied.
 * @return The current drift offset.
 */
float DRIFT_COMP_GetCurrentDriftOffset(void)
{
    return drift_state.current_drift_offset;
}

/**
 * @brief Sets a new stable reference glucose value for adaptive calibration.
 *        This function would typically be called by an external calibration event.
 * @param reference_glucose The known accurate glucose value.
 * @param time_seconds The time when the reference was taken.
 */
void DRIFT_COMP_SetStableReference(float reference_glucose, uint32_t time_seconds)
{
    if (reference_glucose > 0.0f) {
        drift_state.last_stable_reference_glucose = reference_glucose;
        drift_state.last_stable_reference_time = time_seconds;
        LOG_LogMessageF("Stable reference set: %.2f at %lu", reference_glucose, time_seconds);
    }
}
