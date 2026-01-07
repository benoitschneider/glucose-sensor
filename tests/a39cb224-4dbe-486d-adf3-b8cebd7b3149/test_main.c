/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#include "unity.h"
#include "unity_fixture.h"

// Mocks for dependencies
#include "mock_glucose_calculation.h"
#include "mock_drift_compensation.h"
#include "mock_timer.h"
#include "mock_os_primitives.h"

// Include the actual module under test
#include "glucose_processing_pipeline.h"
#include "glucose_filter.h"
#include "glucose_filter_config.h"

// Define test groups and run tests
TEST_GROUP_RUNNER(GlucoseFilterIntegration) {
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_GetFilteredValue_API_existence);
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_FilterExecutionTiming);
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_FilterLatency);
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_SwitchableOutput_Filtered);
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_SwitchableOutput_Unfiltered);
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_FilterDataStructureManagement);
    RUN_TEST_CASE(GlucoseFilterIntegration, test_Glucose_FilterThreadSafety);
}

void setUp(void) {
    // Initialize mocks and module under test before each test
    MockGlucoseCalculation_Init();
    MockDriftCompensation_Init();
    MockTimer_Init();
    MockOSPrimitives_Init();
    GlucoseFilter_Init(); // Initialize filter state
    // Reset filter config to a known state for tests
    GlucoseFilter_SetConfig(GLUCOSE_FILTER_DISABLED); 
}

void tearDown(void) {
    // Clean up after each test
    MockGlucoseCalculation_VerifyAndCleanup();
    MockDriftCompensation_VerifyAndCleanup();
    MockTimer_VerifyAndCleanup();
    MockOSPrimitives_VerifyAndCleanup();
}

int main(int argc, const char *argv[]) {
    UnityMain(argc, argv, UnityGet      GroupName(GlucoseFilterIntegration));
    return 0;
}
