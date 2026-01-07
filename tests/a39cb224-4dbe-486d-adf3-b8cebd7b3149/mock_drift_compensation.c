/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#include "mock_drift_compensation.h"
#include "unity.h"

static float s_expected_apply_value = 0.0f;
static int s_apply_call_count = 0;

void MockDriftCompensation_Init(void) {
    s_expected_apply_value = 0.0f;
    s_apply_call_count = 0;
}

void MOCK_DriftCompensation_Apply_Expect(float expected_value) {
    s_expected_apply_value = expected_value;
}

void DriftCompensation_Apply(float glucose_value) {
    s_apply_call_count++;
    TEST_ASSERT_EQUAL_FLOAT(s_expected_apply_value, glucose_value);
}

void MockDriftCompensation_VerifyAndCleanup(void) {
    // Add verification if needed, e.g., if a specific call count is expected
}
