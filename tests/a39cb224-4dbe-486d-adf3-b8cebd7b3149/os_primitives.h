/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#ifndef OS_PRIMITIVES_H
#define OS_PRIMITIVES_H

#include <stdint.h>

// Define a generic mutex handle type
typedef void* OS_Mutex_t;

// Define a generic task handle type
typedef void* OS_TaskHandle_t;

// Define a generic task function pointer
typedef void (*OS_TaskFunction_t)(void *pvParameters);

// Mutex functions
void OS_Mutex_Create(OS_Mutex_t *mutex_handle);
void OS_Mutex_Lock(OS_Mutex_t mutex_handle, uint32_t timeout_ms);
void OS_Mutex_Unlock(OS_Mutex_t mutex_handle);

// Task functions (for thread safety testing)
void OS_CreateTask(OS_TaskFunction_t pxTaskCode, void *pvParameters, const char * const pcName);
void OS_Delay(uint32_t ms);
void OS_RunScheduler(uint32_t duration_ms);

#define OS_WAIT_FOREVER ((uint32_t)0xFFFFFFFFUL)

#endif // OS_PRIMITIVES_H
