/**
 * Generated Test File
 * 
 * Requirement IDs: 68a43afa-9045-4aa1-afca-f7d7edf9b5b1
 * Task ID: a39cb224-4dbe-486d-adf3-b8cebd7b3149
 * Generated: 2026-01-07T16:16:09.430Z
 */

#ifndef MOCK_DRIFT_COMPENSATION_H
#define MOCK_DRIFT_COMPENSATION_H

// Mock API for DriftCompensation_Apply()
void DriftCompensation_Apply(float glucose_value);

// Mock Control Functions
void MockDriftCompensation_Init(void);
void MOCK_DriftCompensation_Apply_Expect(float expected_value);
void MockDriftCompensation_VerifyAndCleanup(void);

#endif // MOCK_DRIFT_COMPENSATION_H
