/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#ifndef MOCK_OS_PRIMITIVES_H
#define MOCK_OS_PRIMITIVES_H

#include "os_primitives.h"

// Mock Control Functions
void MockOSPrimitives_Init(void);
void MOCK_OS_Mutex_Lock_ExpectAnyArgs(void);
void MOCK_OS_Mutex_Unlock_ExpectAnyArgs(void);
void MOCK_OS_CreateTask(OS_TaskFunction_t pxTaskCode, void *pvParameters, const char * const pcName);
void MOCK_OS_Delay(uint32_t ms);
void MOCK_OS_RunScheduler(uint32_t duration_ms);
void MockOSPrimitives_VerifyAndCleanup(void);

#endif // MOCK_OS_PRIMITIVES_H
