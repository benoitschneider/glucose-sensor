/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#include "glucose_filter.h"
#include "glucose_filter_config.h"
#include "os_primitives.h" // For mutexes

#define FILTER_WINDOW_SIZE 3

static float s_filter_buffer[FILTER_WINDOW_SIZE];
static uint8_t s_buffer_idx = 0;
static bool s_buffer_filled = false;
static OS_Mutex_t s_filter_mutex; // For thread safety

void GlucoseFilter_Init(void) {
    for (int i = 0; i < FILTER_WINDOW_SIZE; i++) {
        s_filter_buffer[i] = 0.0f;
    }
    s_buffer_idx = 0;
    s_buffer_filled = false;
    OS_Mutex_Create(&s_filter_mutex);
}

void GlucoseFilter_AddValue(float new_value) {
    OS_Mutex_Lock(&s_filter_mutex, OS_WAIT_FOREVER);
    s_filter_buffer[s_buffer_idx] = new_value;
    s_buffer_idx = (s_buffer_idx + 1) % FILTER_WINDOW_SIZE;
    if (s_buffer_idx == 0) {
        s_buffer_filled = true;
    }
    OS_Mutex_Unlock(&s_filter_mutex);
}

float GlucoseFilter_GetCurrentAverage(void) {
    OS_Mutex_Lock(&s_filter_mutex, OS_WAIT_FOREVER);
    float sum = 0.0f;
    uint8_t count = s_buffer_filled ? FILTER_WINDOW_SIZE : s_buffer_idx;

    for (uint8_t i = 0; i < count; i++) {
        sum += s_filter_buffer[i];
    }
    OS_Mutex_Unlock(&s_filter_mutex);

    if (count == 0) {
        return 0.0f; // Or handle error/no data
    }
    return sum / count;
}
