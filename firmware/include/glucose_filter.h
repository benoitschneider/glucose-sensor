#ifndef GLUCOSE_FILTER_H
#define GLUCOSE_FILTER_H

#include <stdint.h>

// Define filter types
typedef enum {
    FILTER_TYPE_NONE,
    FILTER_TYPE_MOVING_AVERAGE,
    FILTER_TYPE_MEDIAN,
    // Add other filter types as needed
} glucose_filter_type_t;

// Structure to hold filter parameters
typedef struct {
    glucose_filter_type_t type;
    uint8_t window_size; // For moving average or median filter
    // Add other filter-specific parameters here
} glucose_filter_params_t;

/**
 * @brief Initializes the glucose filter module.
 * @param params Pointer to the filter parameters to use.
 */
void glucose_filter_init(const glucose_filter_params_t *params);

/**
 * @brief Applies the configured filter to a raw glucose value.
 * @param raw_glucose The raw glucose value to filter.
 * @return The filtered glucose value.
 */
float glucose_filter_apply(float raw_glucose);

/**
 * @brief Sets new filter parameters.
 * @param params Pointer to the new filter parameters.
 */
void glucose_filter_set_params(const glucose_filter_params_t *params);

/**
 * @brief Gets the current filter parameters.
 * @param params Pointer to a structure to fill with current parameters.
 */
void glucose_filter_get_params(glucose_filter_params_t *params);

#endif // GLUCOSE_FILTER_H
