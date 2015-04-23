/**
 * Control module for UART commands. Intialization method is provided, and must
 * be ran before using the other methods.
 *
 * uart_send() sends a character over the UART, but isn't interrupt safe.
 *
 * uart_read() reads the UART Rx Buffer and returns the value on it.
 *
 * uart_wasRead() returns whether or not the Rx buffer was read successfully.
 * This is used for debugging purposes.
 *
 * @author James Ritchie III, Mario DeSantis, Jon Frederickson
 * @version 2015.04.22
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Initializes the UART pins and clocks.
 */
void uart_init(void);

/**
 * Sends a character over UART.
 *
 * @param data The data to send over UART.
 *
 * @warning This is not interrupt safe.
 */
void uart_send(char data);

/**
 * Reads and returns a char on the UART Rx Buffer.
 *
 * @return Current value on UART Rx Buffer
 */
char uart_read(void);

/**
 * Returns whether or not the UART Rx Buffer was read successfully.
 *
 * @return <code>true</code> if the receive buffer was read successfully,
 * <code>false</code> if otherwise.
 */
bool uart_wasRead(void);

#endif /* UART_H_ */
