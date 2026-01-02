/**
 * @file app_config.h
 * @brief Application-specific configuration definitions.
 *
 * This file contains macros and definitions that are specific to the
 * main application logic, such as task priorities, buffer sizes, etc.
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

// Define example application-specific constants
#define APP_TASK_STACK_SIZE_BYTES       (1024)
#define APP_DATA_BUFFER_SIZE            (256)
#define APP_SENSOR_READ_INTERVAL_MS     (1000)

// Enable/disable features
#define APP_FEATURE_DEBUG_LOGGING_ENABLED   (1)

#endif // APP_CONFIG_H
