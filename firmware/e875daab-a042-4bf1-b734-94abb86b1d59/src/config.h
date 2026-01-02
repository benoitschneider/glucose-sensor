/*
 * config.h
 * Global configuration defines for the firmware project.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Example configuration defines
#define FIRMWARE_VERSION_MAJOR 1
#define FIRMWARE_VERSION_MINOR 0
#define FIRMWARE_VERSION_PATCH 0

// System clock frequency (e.g., for nRF52832, typically 64 MHz for CPU)
#define SYSTEM_CLOCK_HZ 64000000UL

// Debugging options
#define DEBUG_ENABLED 1

#endif /* CONFIG_H_ */
