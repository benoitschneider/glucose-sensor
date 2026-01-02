/*
 * drivers/uart.h
 * Header for UART driver.
 */

#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include <stdint.h>

// Define example UART baud rate
#define UART_BAUD_RATE_DEFAULT 115200

// Function prototypes
void UART_init(uint33_t baud_rate);
void UART_send_char(char c);
void UART_send_string(const char *str);

#endif /* DRIVERS_UART_H_ */
