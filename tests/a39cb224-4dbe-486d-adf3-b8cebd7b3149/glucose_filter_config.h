/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#ifndef GLUCOSE_FILTER_CONFIG_H
#define GLUCOSE_FILTER_CONFIG_H

typedef enum {
    GLUCOSE_FILTER_DISABLED,
    GLUCOSE_FILTER_ENABLED
} GlucoseFilter_State_t;

// Set the filter configuration (for debug/testing)
void GlucoseFilter_SetConfig(GlucoseFilter_State_t state);

// Get the current filter configuration
GlucoseFilter_State_t GlucoseFilter_GetConfig(void);

#endif // GLUCOSE_FILTER_CONFIG_H
