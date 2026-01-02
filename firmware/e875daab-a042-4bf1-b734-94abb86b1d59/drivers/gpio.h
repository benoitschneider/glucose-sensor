/*
 * drivers/gpio.h
 * Header for GPIO driver.
 */

#ifndef DRIVERS_GPIO_H_
#define DRIVERS_GPIO_H_

#include <stdint.h>
#include <stdbool.h>

// Define example LED pin (adjust as per your board)
#define LED_PIN 13 // Example for nRF52 DK (P0.13)

// Function prototypes
void GPIO_init(void);
void GPIO_set_pin(uint32_t pin_number, bool state);
void GPIO_toggle_pin(uint32_t pin_number);

#endif /* DRIVERS_GPIO_H_ */
