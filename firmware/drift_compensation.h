#ifndef DRIFT_COMPENSATION_H
#define DRIFT_COMPENSATION_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the drift compensation module.
 */
void DRIFT_COMP_Init(void);

/**
 * @brief Applies drift compensation to a raw glucose value.
 * @param raw_glucose The raw glucose reading from the sensor.
 * @param current_time_seconds The current time in seconds from RTC.
 * @return The compensated glucose value.
 */
float DRIFT_COMP_ApplyCompensation(float raw_glucose, uint32_t current_time_seconds);

/**
 * @brief Retrieves the current drift offset being applied.
 * @return The current drift offset.
 */
float DRIFT_COMP_GetCurrentDriftOffset(void);

/**
 * @brief Sets a new stable reference glucose value for adaptive calibration.
 *        This function would typically be called by an external calibration event.
 * @param reference_glucose The known accurate glucose value.
 * @param time_seconds The time when the reference was taken.
 */
void DRIFT_COMP_SetStableReference(float reference_glucose, uint32_t time_seconds);

#endif // DRIFT_COMPENSATION_H
