#ifndef GLUCOSE_FILTER_H
#define GLUCOSE_FILTER_H

#include <stdbool.h> // For bool type
#include <stddef.h> // For size_t

/**
 * @brief Initializes the glucose filter module.
 *        Must be called once before using other filter functions.
 */
void GlucoseFilter_Init(void);

/**
 * @brief Processes a new raw glucose reading and updates the filter's internal state.
 *        This function should be called whenever a new raw glucose value is available.
 * @param new_raw_glucose The latest raw glucose value to be filtered.
 */
void GlucoseFilter_ProcessNewReading(float new_raw_glucose);

/**
 * @brief Retrieves the current filtered glucose value.
 * @return The latest filtered glucose value. Returns 0.0f if not initialized.
 */
float Glucose_GetFilteredValue(void);

#endif // GLUCOSE_FILTER_H
