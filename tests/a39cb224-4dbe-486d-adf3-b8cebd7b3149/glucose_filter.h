/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#ifndef GLUCOSE_FILTER_H
#define GLUCOSE_FILTER_H

#include <stdint.h>

// Initialize the filter module
void GlucoseFilter_Init(void);

// Add a new raw glucose value to the filter's buffer
void GlucoseFilter_AddValue(float new_value);

// Get the current filtered average value
float GlucoseFilter_GetCurrentAverage(void);

#endif // GLUCOSE_FILTER_H
