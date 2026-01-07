/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#include "mock_timer.h"
#include "unity.h"

static uint32_t s_expected_tick_return = 0;

void MockTimer_Init(void) {
    s_expected_tick_return = 0;
}

void MOCK_Timer_GetTick_ExpectAndReturn(uint32_t ret_val) {
    s_expected_tick_return = ret_val;
}

uint32_t Timer_GetTick(void) {
    return s_expected_tick_return;
}

void MockTimer_VerifyAndCleanup(void) {
    // No specific verification needed for this simple mock
}
