#ifndef GLUCOSE_SENSOR_H
#define GLUCOSE_SENSOR_H

#include <stdint.h>

/**
 * @brief Initializes the glucose sensor interface and ADC.
 */
void GLUCOSE_SENSOR_Init(void);

/**
 * @brief Reads the raw glucose value from the sensor.
 *        This function simulates reading from an ADC and converting to a glucose unit.
 * @return The raw glucose value (e.g., in mg/dL or mmol/L).
 */
float GLUCOSE_SENSOR_ReadRawGlucose(void);

#endif // GLUCOSE_SENSOR_H
