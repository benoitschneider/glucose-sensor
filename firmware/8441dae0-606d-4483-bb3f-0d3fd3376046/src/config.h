#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

// --- RTC Configuration ---
#define RTC_INSTANCE_ID         0                       /**< RTC instance used for power management. */
#define RTC_FREQUENCY           32768                   /**< LFCLK frequency. */
#define RTC_PRESCALER           (4095)                  /**< RTC prescaler to achieve 1-second tick (32768 / (4095 + 1) = 8 Hz). */
#define RTC_TICK_FREQ_HZ        (RTC_FREQUENCY / (RTC_PRESCALER + 1)) /**< RTC tick frequency in Hz. */
#define SECONDS_TO_TICKS(s)     ((s) * RTC_TICK_FREQ_HZ) /**< Macro to convert seconds to RTC ticks. */
#define NRF_RTC_COUNTER_MAX     0x00FFFFFF              /**< Maximum value of the RTC counter. */

// --- Power Management Intervals ---
// These intervals are critical for battery life calculation.
// Active measurement & BLE transmission: 5% of time
// Deep sleep: 95% of time
// Total cycle time = ACTIVE_MODE_INTERVAL_SEC + DEEP_SLEEP_INTERVAL_SEC
// For 1 minute BLE data transmission interval, let's assume active mode is 1 minute.
// Then deep sleep interval should be 19 minutes for 5% active time.

#define ACTIVE_MODE_INTERVAL_SEC    (60)    /**< Duration in seconds the device stays in active mode (e.g., for measurement and BLE transmission). */
#define DEEP_SLEEP_INTERVAL_SEC     (19 * 60) /**< Duration in seconds the device stays in deep sleep mode. */

// Total cycle time: 1 minute (active) + 19 minutes (sleep) = 20 minutes
// Active time percentage = (1 / 20) * 100 = 5%

// --- BLE Configuration (minimal for compilation) ---
#define DEVICE_NAME             "GlucoseSensor"         /**< Name of the device for BLE advertising. */
#define MIN_CONN_INTERVAL       MSEC_TO_UNITS(100, UNIT_1_25_MS) /**< Minimum acceptable connection interval (100 ms). */
#define MAX_CONN_INTERVAL       MSEC_TO_UNITS(200, UNIT_1_25_MS) /**< Maximum acceptable connection interval (200 ms). */
#define SLAVE_LATENCY           0                       /**< Slave latency. */
#define CONN_SUP_TIMEOUT        MSEC_TO_UNITS(4000, UNIT_10_MS) /**< Connection supervisory timeout (4 seconds). */
#define APP_BLE_OBSERVER_PRIO   3                       /**< Application's BLE observer priority. */
#define APP_BLE_CONN_CFG_TAG    1                       /**< A tag identifying the SoftDevice configuration. */

// --- App Timer Configuration (minimal for compilation) ---
#define APP_TIMER_PRESCALER     0                       /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE 4                       /**< Size of timer operation queues. */

// --- Sensor Manager Configuration (minimal for compilation) ---
// Placeholder for sensor-specific configurations
#define SENSOR_MEASUREMENT_INTERVAL_MS 1000             /**< Sensor measurement interval in milliseconds (1Hz). */

// --- LED Configuration (example, adjust to actual hardware) ---
#define LED_RED                 NRF_GPIO_PIN_MAP(0, 17) // Example pin
#define LED_GREEN               NRF_GPIO_PIN_MAP(0, 18) // Example pin

#endif // CONFIG_H
