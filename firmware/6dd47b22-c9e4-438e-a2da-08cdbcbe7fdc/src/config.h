/**
 * @file config.h
 * @brief Global configuration definitions for the Glucose Sensor Firmware.
 *
 * This file contains project-wide configuration macros, constants, and type
 * definitions that might be used across different modules.
 * For this 'hello world' task, it remains minimal.
 */

#ifndef CONFIG_H
#define CONFIG_H

// Define the target microcontroller family/series
#define NRF52832_XXAA

// Define the system clock frequency (e.g., 64 MHz for nRF52)
#define SYSTEM_CORE_CLOCK_HZ (64000000UL)

// Add other common configurations here, e.g., debug levels, feature flags
// #define DEBUG_MODE_ENABLED
// #define FEATURE_BLE_ENABLED

#endif // CONFIG_H
