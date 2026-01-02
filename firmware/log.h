#ifndef LOG_H
#define LOG_H

#include <stdint.h>
#include <stdarg.h>

/**
 * @brief Initializes the logging module (e.g., UART, flash).
 */
void LOG_Init(void);

/**
 * @brief Logs a general message string.
 * @param message The string message to log.
 */
void LOG_LogMessage(const char *message);

/**
 * @brief Logs a formatted message string.
 * @param format The format string.
 * @param ... Variable arguments for the format string.
 */
void LOG_LogMessageF(const char *format, ...);

/**
 * @brief Logs a glucose reading with raw, compensated values and timestamp.
 * @param raw_glucose The raw glucose value.
 * @param compensated_glucose The compensated glucose value.
 * @param timestamp_seconds The timestamp of the reading.
 */
void LOG_LogGlucoseReading(float raw_glucose, float compensated_glucose, uint32_t timestamp_seconds);

/**
 * @brief Logs a drift adjustment event.
 * @param timestamp_seconds The timestamp of the adjustment.
 * @param observed_drift The observed drift that triggered the adjustment.
 * @param applied_offset The drift offset applied after compensation.
 */
void LOG_LogDriftAdjustment(uint32_t timestamp_seconds, float observed_drift, float applied_offset);

#endif // LOG_H
