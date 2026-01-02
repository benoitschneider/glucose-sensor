#include "glucose_filter.h"
#include "nvm.h"
#include "config.h"
#include <math.h>
#include <string.h>

// --- Private Defines and Types --- //

// Fixed-point scaling factor for 16-bit signed integers
// Q15 format: 1 sign bit, 0 integer bits, 15 fractional bits
#define Q15_SCALING_FACTOR 32768.0f // 2^15
#define FLOAT_TO_Q15(x) ((int16_t)((x) * Q15_SCALING_FACTOR))
#define Q15_TO_FLOAT(x) ((float)(x) / Q15_SCALING_FACTOR)

// Structure to hold filter state and coefficients
typedef struct {
    // Filter parameter (alpha for EMA, cutoff_freq for Butterworth)
    float parameter_float;
    // Fixed-point coefficients (for Butterworth)
    int16_t b0_q15, b1_q15, b2_q15;
    int16_t a1_q15, a2_q15;
    // Filter state variables (for Butterworth: y[n-1], y[n-2], x[n-1], x[n-2])
    int16_t x_prev_q15[2]; // x[n-1], x[n-2]
    int16_t y_prev_q15[2]; // y[n-1], y[n-2]
    // For EMA, only need previous output
    int16_t ema_prev_output_q15;
} GlucoseFilterState_t;

// Filter state instance
static GlucoseFilterState_t glucoseFilterState;

// --- Private Function Prototypes --- //
static void GlucoseFilter_CalculateCoefficients(float parameter);
static void GlucoseFilter_ResetState(void);

#if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
/**
 * @brief Calculates the EMA filter coefficients based on alpha.
 *        For EMA, alpha is directly used. No complex coefficients needed.
 * @param alpha The smoothing factor (0.0 to 1.0).
 */
static void GlucoseFilter_CalculateCoefficients(float alpha) {
    // For EMA, the parameter is directly alpha. No complex coefficients to store.
    // The alpha value itself is stored in the float parameter_float.
    // The actual EMA calculation uses the float value directly for simplicity
    // or can be converted to fixed point if performance critical.
}

/**
 * @brief Processes a raw glucose value using an Exponential Moving Average (EMA) filter.
 * @param raw_glucose The current raw glucose reading.
 * @return The smoothed glucose reading.
 */
static float GlucoseFilter_ProcessEMA(float *raw_glucose) {
    // Convert raw input to Q15
    int16_t x_n_q15 = FLOAT_TO_Q15(*raw_glucose);

    // Get alpha from the stored float parameter
    float alpha = glucoseFilterState.parameter_float;

    // Handle initial state for EMA
    if (glucoseFilterState.ema_prev_output_q15 == 0 && *raw_glucose != 0.0f) {
        // If first non-zero reading, initialize filter with it
        glucoseFilterState.ema_prev_output_q15 = x_n_q15;
        return *raw_glucose;
    }

    // EMA formula: y[n] = alpha * x[n] + (1 - alpha) * y[n-1]
    // Using fixed-point arithmetic for calculation
    // (alpha * x[n]) + ((1 - alpha) * y[n-1])
    // To avoid float multiplication, convert alpha to Q15 and do fixed-point multiplication
    int16_t alpha_q15 = FLOAT_TO_Q15(alpha);
    int16_t one_minus_alpha_q15 = FLOAT_TO_Q15(1.0f - alpha);

    // Perform multiplication: (A * B) >> 15 for Q15 * Q15 -> Q15
    int32_t term1 = (int32_t)alpha_q15 * x_n_q15;
    int32_t term2 = (int32_t)one_minus_alpha_q15 * glucoseFilterState.ema_prev_output_q15;

    int16_t y_n_q15 = (int16_t)((term1 + term2) >> 15);

    // Update state
    glucoseFilterState.ema_prev_output_q15 = y_n_q15;

    // Convert back to float for return
    return Q15_TO_FLOAT(y_n_q15);
}

#elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
/**
 * @brief Calculates 2nd order Butterworth low-pass filter coefficients.
 *        Uses bilinear transform for analog to digital filter conversion.
 * @param normalized_cutoff_freq Normalized cutoff frequency (0.0 to 0.5, where 0.5 is Nyquist).
 */
static void GlucoseFilter_CalculateCoefficients(float normalized_cutoff_freq) {
    // Ensure cutoff frequency is within valid range
    if (normalized_cutoff_freq <= 0.0f) normalized_cutoff_freq = 0.001f;
    if (normalized_cutoff_freq >= 0.5f) normalized_cutoff_freq = 0.499f;

    // Analog prototype Butterworth coefficients (normalized)
    // For 2nd order, s^2 + sqrt(2)s + 1

    // Bilinear transform variables
    float K = tanf(M_PI * normalized_cutoff_freq); // K = tan(pi * fc/fs)
    float K_sq = K * K;
    float sqrt2 = 1.41421356237f;

    // Denominator for digital filter coefficients
    float D = 1.0f + sqrt2 * K + K_sq;

    // Digital filter coefficients (normalized by D)
    float b0 = K_sq / D;
    float b1 = 2.0f * K_sq / D;
    float b2 = K_sq / D;

    float a1 = 2.0f * (K_sq - 1.0f) / D;
    float a2 = (1.0f - sqrt2 * K + K_sq) / D;

    // Store coefficients in Q15 format
    glucoseFilterState.b0_q15 = FLOAT_TO_Q15(b0);
    glucoseFilterState.b1_q15 = FLOAT_TO_Q15(b1);
    glucoseFilterState.b2_q15 = FLOAT_TO_Q15(b2);
    glucoseFilterState.a1_q15 = FLOAT_TO_Q15(a1);
    glucoseFilterState.a2_q15 = FLOAT_TO_Q15(a2);

    // Store the float parameter for retrieval
    glucoseFilterState.parameter_float = normalized_cutoff_freq;
}

/**
 * @brief Processes a raw glucose value using a 2nd order IIR Butterworth low-pass filter.
 *        Implements Direct Form 2 Transposed structure.
 * @param raw_glucose The current raw glucose reading.
 * @return The smoothed glucose reading.
 */
static float GlucoseFilter_ProcessButterworth(float raw_glucose) {
    // Convert raw input to Q15
    int16_t x_n_q15 = FLOAT_TO_Q15(raw_glucose);

    // Direct Form 2 Transposed IIR filter structure:
    // y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
    // Or, for transposed form (more numerically stable for fixed point):
    // w[n] = x[n] - a1*w[n-1] - a2*w[n-2]
    // y[n] = b0*w[n] + b1*w[n-1] + b2*w[n-2]
    // For simplicity and direct implementation of the given formula, we'll use a direct form I approach
    // y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]

    // All multiplications are Q15 * Q15 -> Q30, then right shift by 15 to get Q15
    int32_t term_b0 = (int32_t)glucoseFilterState.b0_q15 * x_n_q15;
    int32_t term_b1 = (int32_t)glucoseFilterState.b1_q15 * glucoseFilterState.x_prev_q15[0];
    int32_t term_b2 = (int32_t)glucoseFilterState.b2_q15 * glucoseFilterState.x_prev_q15[1];

    int32_t term_a1 = (int32_t)glucoseFilterState.a1_q15 * glucoseFilterState.y_prev_q15[0];
    int32_t term_a2 = (int32_t)glucoseFilterState.a2_q15 * glucoseFilterState.y_prev_q15[1];

    // Sum the terms (intermediate result can be Q30 or higher, but fit in int32_t)
    int32_t sum_terms = term_b0 + term_b1 + term_b2 - term_a1 - term_a2;

    // Right shift to convert back to Q15
    int16_t y_n_q15 = (int16_t)(sum_terms >> 15);

    // Update state variables (shift history)
    glucoseFilterState.x_prev_q15[1] = glucoseFilterState.x_prev_q15[0]; // x[n-2] = x[n-1]
    glucoseFilterState.x_prev_q15[0] = x_n_q15;                           // x[n-1] = x[n]

    glucoseFilterState.y_prev_q15[1] = glucoseFilterState.y_prev_q15[0]; // y[n-2] = y[n-1]
    glucoseFilterState.y_prev_q15[0] = y_n_q15;                           // y[n-1] = y[n]

    // Convert back to float for return
    return Q15_TO_FLOAT(y_n_q15);
}
#endif // GLUCOSE_FILTER_TYPE

/**
 * @brief Resets the filter's internal state variables to zero.
 *        Should be called when filter parameters change or on startup.
 */
static void GlucoseFilter_ResetState(void) {
    memset(&glucoseFilterState.x_prev_q15, 0, sizeof(glucoseFilterState.x_prev_q15));
    memset(&glucoseFilterState.y_prev_q15, 0, sizeof(glucoseFilterState.y_prev_q15));
    glucoseFilterState.ema_prev_output_q15 = 0;
}

// --- Public API Functions --- //

/**
 * @brief Initializes the glucose filter module.
 *        Loads the filter parameter from NVM and calculates coefficients.
 */
void GlucoseFilter_Init(void) {
    float stored_parameter;

    // Load parameter from NVM
    if (NVM_ReadFloat(NVM_KEY_FILTER_PARAMETER, &stored_parameter) != NVM_OK) {
        // If NVM read fails or no parameter is stored, use default
        #if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
        stored_parameter = GLUCOSE_FILTER_DEFAULT_ALPHA;
        #elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
        stored_parameter = GLUCOSE_FILTER_DEFAULT_CUTOFF_FREQ;
        #endif
        // Attempt to save default to NVM for future boots
        NVM_WriteFloat(NVM_KEY_FILTER_PARAMETER, stored_parameter);
    }

    // Store the parameter and calculate coefficients
    glucoseFilterState.parameter_float = stored_parameter;
    GlucoseFilter_CalculateCoefficients(stored_parameter);
    GlucoseFilter_ResetState(); // Reset filter state on initialization
}

/**
 * @brief Sets the filter parameter (e.g., alpha for EMA, cutoff frequency for Butterworth).
 *        The parameter is saved to non-volatile memory.
 * @param parameter The new filter parameter.
 * @return GLUCOSE_FILTER_OK if successful, or an error code.
 */
GlucoseFilter_Status_t GlucoseFilter_SetIIRParameter(float parameter) {
    // Validate parameter range
    #if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
    if (parameter < GLUCOSE_FILTER_MIN_ALPHA || parameter > GLUCOSE_FILTER_MAX_ALPHA) {
        return GLUCOSE_FILTER_ERROR_INVALID_PARAM;
    }
    #elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
    if (parameter < GLUCOSE_FILTER_MIN_CUTOFF_FREQ || parameter > GLUCOSE_FILTER_MAX_CUTOFF_FREQ) {
        return GLUCOSE_FILTER_ERROR_INVALID_PARAM;
    }
    #endif

    // Save to NVM
    if (NVM_WriteFloat(NVM_KEY_FILTER_PARAMETER, parameter) != NVM_OK) {
        return GLUCOSE_FILTER_ERROR_NVM_WRITE_FAIL;
    }

    // Update internal state and recalculate coefficients
    glucoseFilterState.parameter_float = parameter;
    GlucoseFilter_CalculateCoefficients(parameter);
    GlucoseFilter_ResetState(); // Reset filter state after parameter change

    return GLUCOSE_FILTER_OK;
}

/**
 * @brief Gets the current filter parameter.
 * @return The current filter parameter.
 */
float GlucoseFilter_GetIIRParameter(void) {
    return glucoseFilterState.parameter_float;
}

/**
 * @brief Processes a raw glucose value through the configured IIR filter.
 * @param raw_glucose The current raw glucose reading.
 * @return The smoothed glucose reading.
 */
float GlucoseFilter_Process(float raw_glucose) {
    #if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
    return GlucoseFilter_ProcessEMA(raw_glucose);
    #elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
    return GlucoseFilter_ProcessButterworth(raw_glucose);
    #else
    // If no filter type is defined, return raw value
    return raw_glucose;
    #error "GLUCOSE_FILTER_TYPE not defined or invalid."
    #endif
}
