#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// --- Sensor Sampling Configuration ---
// The buffer can hold at least 60 seconds of data (60 samples).
// We'll make it slightly larger for robustness.
#define SENSOR_BUFFER_SIZE (60 + 5) // 65 samples to hold > 60 seconds of data

// --- General System Configuration ---
// Add any other global configurations here if needed.

#endif // CONFIG_H
