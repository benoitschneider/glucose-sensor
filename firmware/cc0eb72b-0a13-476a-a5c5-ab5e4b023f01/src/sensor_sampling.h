#ifndef SENSOR_SAMPLING_H
#define SENSOR_SAMPLING_H

#include <stdint.h>
#include <stdbool.h>

// Define status codes for the sensor sampling module
typedef enum
{
    SENSOR_SAMPLING_OK = 0,
    SENSOR_SAMPLING_ERROR_NO_DATA,
    SENSOR_SAMPLING_ERROR_INVALID_PARAM,
    SENSOR_SAMPLING_ERROR_INIT_FAILED,
    // Add more error codes as needed
} SENSOR_SAMPLING_Status_t;

/**
 * @brief Initializes the sensor sampling module.
 *        Resets the circular buffer and prepares for data acquisition.
 * @return SENSOR_SAMPLING_OK if successful, error code otherwise.
 */
SENSOR_SAMPLING_Status_t SENSOR_SAMPLING_Init(void);

/**
 * @brief Timer Interrupt Service Routine (ISR) callback for 1Hz sampling.
 *        This function should be called by the hardware timer ISR every 1000ms.
 *        It reads the ADS1115 and stores the value in the circular buffer.
 */
void SENSOR_SAMPLING_Timer_ISR(void);

/**
 * @brief Retrieves the latest sensor reading from the buffer.
 * @param[out] reading Pointer to store the latest 16-bit sensor reading.
 * @return SENSOR_SAMPLING_OK if a reading is available, SENSOR_SAMPLING_ERROR_NO_DATA otherwise.
 */
SENSOR_SAMPLING_Status_t SENSOR_SAMPLING_GetLatestSensorReading(int16_t *reading);

/**
 * @brief Retrieves a block of sensor data from the circular buffer.
 *        Samples are returned from oldest to newest available data.
 *        The retrieved samples are removed from the buffer (buffer_tail advances).
 * @param[in] num_samples The maximum number of samples to retrieve.
 * @param[out] output_buffer Pointer to an array where samples will be stored.
 *                             The buffer must be large enough to hold num_samples.
 * @return The actual number of samples copied.
 */
uint16_t SENSOR_SAMPLING_GetSensorDataBuffer(uint16_t num_samples, int16_t *output_buffer);

// --- Dummy ADS1115 Driver Interface (for compilation) ---
// In a real system, these would be in ads1115_driver.h
typedef enum
{
    ADS1115_OK = 0,
    ADS1115_ERROR,
    ADS1115_BUSY
} ADS1115_Status_t;

ADS1115_Status_t ADS1115_Init(void);
ADS1115_Status_t ADS1115_ReadRawData(int16_t *data);

// --- Dummy Timer Driver Interface (for compilation) ---
// In a real system, these would be in timer_driver.h
typedef enum
{
    TIMER_DRV_OK = 0,
    TIMER_DRV_ERROR
} TIMER_DRV_Status_t;

TIMER_DRV_Status_t TIMER_DRV_Init(uint32_t period_ms);
uint32_t TIMER_DRV_Get_Uptime_ms(void);
void TIMER_DRV_Simulate_Tick(void); // For testing/simulation

#endif // SENSOR_SAMPLING_H
