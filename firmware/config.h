#ifndef CONFIG_H
#define CONFIG_H

// --- Glucose Filter Configuration --- //

// Define the type of IIR filter to use
// Options:
//   GLUCOSE_FILTER_TYPE_EMA                 (1st order Exponential Moving Average)
//   GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER (2nd order Butterworth Low-Pass)
#define GLUCOSE_FILTER_TYPE GLUCOSE_FILTER_TYPE_EMA

// Filter Type Definitions
#define GLUCOSE_FILTER_TYPE_EMA                 1
#define GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER 2

#if (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_EMA)
    // EMA Filter Parameters
    #define GLUCOSE_FILTER_DEFAULT_ALPHA    0.2f  // Default smoothing factor (0.0 to 1.0)
    #define GLUCOSE_FILTER_MIN_ALPHA        0.01f // Minimum alpha (very slow filter)
    #define GLUCOSE_FILTER_MAX_ALPHA        0.99f // Maximum alpha (very fast filter, almost no smoothing)
#elif (GLUCOSE_FILTER_TYPE == GLUCOSE_FILTER_TYPE_BUTTERWORTH_2ND_ORDER)
    // Butterworth Filter Parameters
    // Cutoff frequency normalized to Nyquist frequency (fs/2).
    // E.g., if sampling at 1Hz, and want 0.1Hz cutoff, then normalized_cutoff_freq = 0.1 / (1/2) = 0.2
    #define GLUCOSE_FILTER_DEFAULT_CUTOFF_FREQ  0.1f // Default normalized cutoff frequency (0.0 to 0.5)
    #define GLUCOSE_FILTER_MIN_CUTOFF_FREQ      0.001f // Minimum normalized cutoff frequency
    #define GLUCOSE_FILTER_MAX_CUTOFF_FREQ      0.49f  // Maximum normalized cutoff frequency
    // For Butterworth, M_PI is used, ensure it's defined or include cmath/math.h
    #ifndef M_PI
    #define M_PI 3.14159265358979323846f
    #endif
#else
    #error "GLUCOSE_FILTER_TYPE not defined or invalid in config.h"
#endif

// --- Non-Volatile Memory (NVM) Configuration --- //

// Unique key for storing the filter parameter in NVM
#define NVM_KEY_FILTER_PARAMETER 0x0001

// Mock NVM module for demonstration
// In a real system, this would be an actual NVM driver (Flash, EEPROM, FRAM, etc.)
typedef enum {
    NVM_OK = 0,
    NVM_ERROR_READ_FAIL,
    NVM_ERROR_WRITE_FAIL,
    NVM_ERROR_INVALID_KEY,
    NVM_ERROR_NOT_INITIALIZED
} NVM_Status_t;

void NVM_Init(void);
NVM_Status_t NVM_WriteFloat(uint16_t key, float value);
NVM_Status_t NVM_ReadFloat(uint16_t key, float *value);

#endif // CONFIG_H
