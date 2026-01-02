#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <stdint.h>

/**@brief Function for initializing the sensor manager and associated peripherals (ADC, I2C).
 */
void sensor_manager_init(void);

/**@brief Function for deinitializing the sensor manager and associated peripherals.
 */
void sensor_manager_deinit(void);

/**@brief Function to perform a single glucose measurement.
 * @return The measured glucose value (example placeholder).
 */
uint16_t sensor_manager_perform_measurement(void);

#endif // SENSOR_MANAGER_H
