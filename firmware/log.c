#include "log.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

// Simple buffer for logging messages. In a real system, this would write to UART, flash, or a dedicated logging peripheral.
static char log_buffer[LOG_BUFFER_SIZE];

/**
 * @brief Initializes the logging module (e.g., UART, flash).
 */
void LOG_Init(void)
{
    // Initialize UART or other logging interface here.
    // For this example, we'll just clear the buffer.
    memset(log_buffer, 0, sizeof(log_buffer));
}

/**
 * @brief Prints a log message to a simulated console/UART.
 * @param message The message to print.
 */
static void LOG_PrintToConsole(const char *message)
{
    // In a real system, this would send data over UART, SWD, etc.
    // For simulation, we can print to stdout or a debug console.
    // printf("%s\n", message); // Uncomment for PC-based simulation
}

/**
 * @brief Logs a general message string.
 * @param message The string message to log.
 */
void LOG_LogMessage(const char *message)
{
    snprintf(log_buffer, LOG_BUFFER_SIZE, "[LOG] %s", message);
    LOG_PrintToConsole(log_buffer);
}

/**
 * @brief Logs a formatted message string.
 * @param format The format string.
 * @param ... Variable arguments for the format string.
 */
void LOG_LogMessageF(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(log_buffer, LOG_BUFFER_SIZE, format, args);
    va_end(args);
    LOG_PrintToConsole(log_buffer);
}

/**
 * @brief Logs a glucose reading with raw, compensated values and timestamp.
 * @param raw_glucose The raw glucose value.
 * @param compensated_glucose The compensated glucose value.
 * @param timestamp_seconds The timestamp of the reading.
 */
void LOG_LogGlucoseReading(float raw_glucose, float compensated_glucose, uint32_t timestamp_seconds)
{
    snprintf(log_buffer, LOG_BUFFER_SIZE, "[GLUCOSE] Time: %lu, Raw: %.2f, Comp: %.2f",
             timestamp_seconds, raw_glucose, compensated_glucose);
    LOG_PrintToConsole(log_buffer);
}

/**
 * @brief Logs a drift adjustment event.
 * @param timestamp_seconds The timestamp of the adjustment.
 * @param observed_drift The observed drift that triggered the adjustment.
 * @param applied_offset The drift offset applied after compensation.
 */
void LOG_LogDriftAdjustment(uint32_t timestamp_seconds, float observed_drift, float applied_offset)
{
#if LOG_ENABLE_DRIFT_ADJUSTMENTS
    snprintf(log_buffer, LOG_BUFFER_SIZE, "[DRIFT_ADJ] Time: %lu, Observed Drift: %.2f, Applied Offset: %.2f",
             timestamp_seconds, observed_drift, applied_offset);
    LOG_PrintToConsole(log_buffer);
#endif
}
