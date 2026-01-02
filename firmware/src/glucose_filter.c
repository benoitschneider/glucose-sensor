#include "glucose_filter.h"
#include <stddef.h>
#include <string.h>

#define MAX_FILTER_WINDOW_SIZE 10 // Maximum supported window size

static glucose_filter_params_t current_filter_params;
static float glucose_buffer[MAX_FILTER_WINDOW_SIZE];
static uint8_t buffer_idx = 0;
static uint8_t buffer_fill_count = 0;

// Helper function for median filter (simple bubble sort for now)
static float calculate_median(float *arr, uint8_t size) {
    if (size == 0) return 0.0f;
    if (size == 1) return arr[0];

    float sorted_arr[MAX_FILTER_WINDOW_SIZE];
    memcpy(sorted_arr, arr, size * sizeof(float));

    for (uint8_t i = 0; i < size - 1; i++) {
        for (uint8_t j = 0; j < size - i - 1; j++) {
            if (sorted_arr[j] > sorted_arr[j+1]) {
                float temp = sorted_arr[j];
                sorted_arr[j] = sorted_arr[j+1];
                sorted_arr[j+1] = temp;
            }
        }
    }

    if (size % 2 == 1) {
        return sorted_arr[size / 2];
    } else {
        return (sorted_arr[size / 2 - 1] + sorted_arr[size / 2]) / 2.0f;
    }
}

void glucose_filter_init(const glucose_filter_params_t *params) {
    if (params != NULL) {
        current_filter_params = *params;
        if (current_filter_params.window_size == 0 || current_filter_params.window_size > MAX_FILTER_WINDOW_SIZE) {
            current_filter_params.window_size = 1; // Default to 1 if invalid
        }
    } else {
        // Default parameters if none provided
        current_filter_params.type = FILTER_TYPE_MOVING_AVERAGE;
        current_filter_params.window_size = 5; // Default window size
    }
    // Clear the buffer
    memset(glucose_buffer, 0, sizeof(glucose_buffer));
    buffer_idx = 0;
    buffer_fill_count = 0;
}

void glucose_filter_set_params(const glucose_filter_params_t *params) {
    if (params != NULL) {
        current_filter_params = *params;
        if (current_filter_params.window_size == 0 || current_filter_params.window_size > MAX_FILTER_WINDOW_SIZE) {
            current_filter_params.window_size = 1; // Default to 1 if invalid
        }
        // Reset buffer on parameter change to avoid stale data with new window size
        memset(glucose_buffer, 0, sizeof(glucose_buffer));
        buffer_idx = 0;
        buffer_fill_count = 0;
    }
}

void glucose_filter_get_params(glucose_filter_params_t *params) {
    if (params != NULL) {
        *params = current_filter_params;
    }
}

float glucose_filter_apply(float raw_glucose) {
    // Add new value to buffer
    glucose_buffer[buffer_idx] = raw_glucose;
    buffer_idx = (buffer_idx + 1) % current_filter_params.window_size;
    if (buffer_fill_count < current_filter_params.window_size) {
        buffer_fill_count++;
    }

    if (buffer_fill_count < current_filter_params.window_size) {
        // Not enough data to fill the window, return raw for now or a simple average of available data
        // For simplicity, returning raw until buffer is full.
        // A more sophisticated approach might average available data or use a shorter window initially.
        return raw_glucose;
    }

    float filtered_value = raw_glucose; // Default to raw if no filter or not enough data

    switch (current_filter_params.type) {
        case FILTER_TYPE_NONE:
            filtered_value = raw_glucose;
            break;
        case FILTER_TYPE_MOVING_AVERAGE:
        {
            float sum = 0.0f;
            for (uint8_t i = 0; i < current_filter_params.window_size; i++) {
                sum += glucose_buffer[i];
            }
            filtered_value = sum / current_filter_params.window_size;
            break;
        }
        case FILTER_TYPE_MEDIAN:
        {
            filtered_value = calculate_median(glucose_buffer, current_filter_params.window_size);
            break;
        }
        default:
            // Unknown filter type, return raw
            filtered_value = raw_glucose;
            break;
    }

    return filtered_value;
}
