#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// --- Glucose Filter Configuration ---
#define GLUCOSE_FILTER_MAX_WINDOW_SIZE      (20U)   // Maximum allowed window size for the filter
#define GLUCOSE_FILTER_MIN_WINDOW_SIZE      (3U)    // Minimum allowed window size for the filter
#define GLUCOSE_FILTER_DEFAULT_WINDOW_SIZE  (5U)    // Default window size at first boot or NVM error

// --- Non-Volatile Memory (NVM) Configuration ---
// Define NVM addresses for persistent storage.
// These addresses should be unique and within the NVM module's address space.
#define NVM_ADDR_GLUCOSE_FILTER_WINDOW_SIZE (0x0000U) // Address for storing filter window size (1 byte)
#define NVM_TOTAL_SIZE                      (0x0100U) // Total size of mock NVM storage (256 bytes for example)

#endif // CONFIG_H
