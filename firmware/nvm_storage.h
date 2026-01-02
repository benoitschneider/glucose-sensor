// This file is intentionally left minimal as the mock NVM functions
// are included in glucose_filter.c for simplicity in this example.
// In a real project, this would be a separate, full-fledged NVM driver.
// The NVM_Key_t and NVM_Status_t enums and function prototypes are
// already defined in glucose_filter.h to avoid circular dependencies
// and provide a clear interface. This file would typically contain
// platform-specific NVM definitions or includes.

#ifndef NVM_STORAGE_H
#define NVM_STORAGE_H

// Include the filter header to get NVM interface definitions
#include "glucose_filter.h"

// No additional content needed here for this specific mock setup.

#endif // NVM_STORAGE_H
