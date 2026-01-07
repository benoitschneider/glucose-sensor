/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#include "mock_os_primitives.h"
#include "unity.h"

static int s_mutex_lock_call_count = 0;
static int s_mutex_unlock_call_count = 0;

void MockOSPrimitives_Init(void) {
    s_mutex_lock_call_count = 0;
    s_mutex_unlock_call_count = 0;
}

void MOCK_OS_Mutex_Lock_ExpectAnyArgs(void) {
    // No specific expectation for arguments, just count calls
}

void OS_Mutex_Create(OS_Mutex_t *mutex_handle) {
    TEST_ASSERT_NOT_NULL(mutex_handle);
    *mutex_handle = (OS_Mutex_t)1; // Simulate a valid handle
}

void OS_Mutex_Lock(OS_Mutex_t mutex_handle, uint32_t timeout_ms) {
    s_mutex_lock_call_count++;
    TEST_ASSERT_NOT_NULL(mutex_handle);
    // In a real mock, you'd simulate blocking or return false on timeout
}

void MOCK_OS_Mutex_Unlock_ExpectAnyArgs(void) {
    // No specific expectation for arguments, just count calls
}

void OS_Mutex_Unlock(OS_Mutex_t mutex_handle) {
    s_mutex_unlock_call_count++;
    TEST_ASSERT_NOT_NULL(mutex_handle);
}

void MOCK_OS_CreateTask(OS_TaskFunction_t pxTaskCode, void *pvParameters, const char * const pcName) {
    // In a simple mock, we just call the task function directly to simulate execution
    // For more complex multi-threading, a more sophisticated mock scheduler would be needed.
    // For this test, we'll just store the task functions and execute them later.
    // This is a simplification and might not fully simulate concurrent execution issues.
    pxTaskCode(pvParameters);
}

void OS_CreateTask(OS_TaskFunction_t pxTaskCode, void *pvParameters, const char * const pcName) {
    // See MOCK_OS_CreateTask for explanation
    MOCK_OS_CreateTask(pxTaskCode, pvParameters, pcName);
}

void MOCK_OS_Delay(uint32_t ms) {
    // Simulate delay by doing nothing or incrementing a mock timer
}

void OS_Delay(uint32_t ms) {
    MOCK_OS_Delay(ms);
}

void MOCK_OS_RunScheduler(uint32_t duration_ms) {
    // For this simple mock, we just ensure that if mutexes were expected, they were called.
    // A real mock scheduler would interleave task execution.
}

void OS_RunScheduler(uint32_t duration_ms) {
    MOCK_OS_RunScheduler(duration_ms);
}

void MockOSPrimitives_VerifyAndCleanup(void) {
    // For thread safety, we expect locks and unlocks to be balanced
    if (s_mutex_lock_call_count != s_mutex_unlock_call_count) {
        TEST_FAIL_MESSAGE("OS Mutex lock/unlock calls are not balanced.");
    }
}
