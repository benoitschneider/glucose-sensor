/**
 * @file ble_config.h
 * @brief Configuration definitions for Bluetooth Low Energy (BLE) functionality.
 */

#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

// Example: BLE device name
#define BLE_DEVICE_NAME         "GlucoseSensor"

// Example: Advertising intervals (in units of 0.625 ms)
#define BLE_ADVERTISING_INTERVAL_FAST   MSEC_TO_UNITS(200, UNIT_0_625_MS) // 200 ms
#define BLE_ADVERTISING_INTERVAL_SLOW   MSEC_TO_UNITS(1000, UNIT_0_625_MS) // 1 second

// Example: Connection parameters
#define MIN_CONN_INTERVAL       MSEC_TO_UNITS(10, UNIT_1_25_MS)  /**< Minimum acceptable connection interval (10 ms). */
#define MAX_CONN_INTERVAL       MSEC_TO_UNITS(20, UNIT_1_25_MS)  /**< Maximum acceptable connection interval (20 ms). */
#define SLAVE_LATENCY           0                                /**< Slave latency. */
#define CONN_SUP_TIMEOUT        MSEC_TO_UNITS(4000, UNIT_10_MS)  /**< Connection supervisory timeout (4 seconds). */

#endif // BLE_CONFIG_H
