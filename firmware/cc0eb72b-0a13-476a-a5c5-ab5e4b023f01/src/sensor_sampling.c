#include "sensor_sampling.h"
#include "ads1115_driver.h"
#include "config.h"
#include <stddef.h>

// Circular buffer for sensor readings
static volatile int16_t sensor_data_buffer[SENSOR_BUFFER_SIZE];
static volatile uint16_t buffer_head = 0; // Points to the next write position
static volatile uint16_t buffer_tail = 0; // Points to the oldest unread data
static volatile bool buffer_full = false; // Flag to indicate if buffer has wrapped around

/**
 * @brief Initializes the sensor sampling module.
 * @return SENSOR_SAMPLING_OK if successful, error code otherwise.
 */
SENSOR_SAMPLING_Status_t SENSOR_SAMPLING_Init(void)
{
    // Initialize buffer pointers and state
    buffer_head = 0;
    buffer_tail = 0;
    buffer_full = false;
    // No other specific initialization needed for this module itself beyond buffer reset.
    return SENSOR_SAMPLING_OK;
}

/**
 * @brief Timer Interrupt Service Routine (ISR) callback for 1Hz sampling.
 *        This function should be called by the hardware timer ISR every 1000ms.
 */
void SENSOR_SAMPLING_Timer_ISR(void)
{
    int16_t raw_adc_value;

    // Read raw data from ADS1115
    // It's critical that ads1115_read_raw_data() is non-blocking and fast.
    // In an ISR, avoid I2C blocking waits if possible. A driver for ADS1115
    // should ideally handle conversions in a non-blocking manner or have
    // a very fast read operation after a conversion is known to be complete.
    // For this example, we assume ads1115_read_raw_data() is fast enough.
    if (ADS1115_ReadRawData(&raw_adc_value) == ADS1115_OK)
    {
        // Store the reading in the circular buffer
        sensor_data_buffer[buffer_head] = raw_adc_value;

        // Move head pointer to the next position
        buffer_head = (buffer_head + 1) % SENSOR_BUFFER_SIZE;

        // If head catches up to tail, the buffer is full and oldest data is overwritten
        if (buffer_head == buffer_tail)
        {
            buffer_full = true;
            buffer_tail = (buffer_tail + 1) % SENSOR_BUFFER_SIZE; // Advance tail to discard oldest data
        }
    }
    else
    {
        // Handle ADC read error in ISR (e.g., increment error counter, log, etc.)
        // For production, this might involve a more robust error handling mechanism
        // such as setting a flag for the main loop to handle.
    }
}

/**
 * @brief Retrieves the latest sensor reading from the buffer.
 * @param[out] reading Pointer to store the latest 16-bit sensor reading.
 * @return SENSOR_SAMPLING_OK if a reading is available, SENSOR_SAMPLING_ERROR_NO_DATA otherwise.
 */
SENSOR_SAMPLING_Status_t SENSOR_SAMPLING_GetLatestSensorReading(int16_t *reading)
{
    if (reading == NULL)
    {
        return SENSOR_SAMPLING_ERROR_INVALID_PARAM;
    }

    // Check if there is any data in the buffer
    if (!buffer_full && (buffer_head == buffer_tail))
    {
        return SENSOR_SAMPLING_ERROR_NO_DATA; // Buffer is empty
    }

    // The latest reading is at the position just before the head
    uint16_t latest_idx = (buffer_head == 0) ? (SENSOR_BUFFER_SIZE - 1) : (buffer_head - 1);
    *reading = sensor_data_buffer[latest_idx];

    return SENSOR_SAMPLING_OK;
}

/**
 * @brief Retrieves a block of sensor data from the circular buffer.
 *        Samples are returned from oldest to newest available data.
 * @param[in] num_samples The number of samples to retrieve.
 * @param[out] output_buffer Pointer to an array where samples will be stored.
 * @return The actual number of samples copied, or 0 if an error occurs.
 */
uint16_t SENSOR_SAMPLING_GetSensorDataBuffer(uint16_t num_samples, int16_t *output_buffer)
{
    if (output_buffer == NULL || num_samples == 0)
    {
        return 0; // Invalid parameters
    }

    uint16_t samples_available;
    if (buffer_full)
    {
        samples_available = SENSOR_BUFFER_SIZE;
    }
    else
    {
        if (buffer_head >= buffer_tail)
        {
            samples_available = buffer_head - buffer_tail;
        }
        else
        {
            // This case should not happen if buffer_full is false and head < tail
            // It implies the buffer wrapped around but buffer_full wasn't set.
            // For safety, recalculate based on head and tail positions.
            samples_available = SENSOR_BUFFER_SIZE - buffer_tail + buffer_head;
        }
    }

    uint16_t samples_to_copy = (num_samples < samples_available) ? num_samples : samples_available;
    uint16_t copied_count = 0;

    // Copy samples from tail towards head
    for (uint16_t i = 0; i < samples_to_copy; ++i)
    {
        output_buffer[i] = sensor_data_buffer[buffer_tail];
        buffer_tail = (buffer_tail + 1) % SENSOR_BUFFER_SIZE;
        copied_count++;
    }

    // If we copied all available samples, the buffer is no longer full
    if (copied_full_buffer_was_read)
    {
        buffer_full = false;
    }

    return copied_count;
}

// Dummy ADS1115 driver functions for compilation
// In a real system, these would be implemented in ads1115_driver.c/h
ADS1115_Status_t ADS1115_Init(void)
{
    // Simulate ADS1115 initialization
    return ADS1115_OK;
}

ADS1115_Status_t ADS1115_ReadRawData(int16_t *data)
{
    // Simulate reading raw data from ADS1115
    // In a real scenario, this would involve I2C communication
    // and reading the conversion register.
    static int16_t dummy_adc_value = 20000;
    if (data != NULL)
    {
        *data = dummy_adc_value++; // Simulate changing ADC value
        return ADS1115_OK;
    }
    return ADS1115_ERROR;
}

// Dummy Timer Driver functions for compilation
// In a real system, these would be implemented in timer_driver.c/h
static volatile uint32_t system_uptime_ms = 0;

TIMER_DRV_Status_t TIMER_DRV_Init(uint32_t period_ms)
{
    // Simulate timer initialization
    // In a real system, this would configure a hardware timer
    // to interrupt every `period_ms` and call SENSOR_SAMPLING_Timer_ISR.
    (void)period_ms; // Suppress unused parameter warning
    return TIMER_DRV_OK;
}

uint32_t TIMER_DRV_Get_Uptime_ms(void)
{
    // Simulate system uptime (for main loop demonstration)
    // In a real system, this would read a hardware timer counter.
    return system_uptime_ms;
}

// This function would be called by the actual hardware timer ISR
// For simulation purposes, we can manually call it or have a separate thread.
void TIMER_DRV_Simulate_Tick(void)
{
    system_uptime_ms += 1000; // Simulate 1 second passing
    SENSOR_SAMPLING_Timer_ISR();
}
