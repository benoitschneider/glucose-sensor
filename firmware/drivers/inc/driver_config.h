/**
 * @file driver_config.h
 * @brief Configuration definitions for various drivers.
 */

#ifndef DRIVER_CONFIG_H
#define DRIVER_CONFIG_H

// Example: Enable/disable specific drivers
#define DRIVER_SENSOR_ADS1115_ENABLED 1
#define DRIVER_I2C_ENABLED            1

// Example: I2C configuration
#define I2C_SCL_PIN     NRF_GPIO_PIN_MAP(0, 27)
#define I2C_SDA_PIN     NRF_GPIO_PIN_MAP(0, 26)
#define I2C_FREQUENCY   NRF_TWIM_FREQ_400K

#endif // DRIVER_CONFIG_H
