/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#include "glucose_filter_config.h"

static GlucoseFilter_State_t s_filter_state = GLUCOSE_FILTER_DISABLED;

void GlucoseFilter_SetConfig(GlucoseFilter_State_t state) {
    s_filter_state = state;
}

GlucoseFilter_State_t GlucoseFilter_GetConfig(void) {
    return s_filter_state;
}
