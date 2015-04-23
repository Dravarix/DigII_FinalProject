
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

void uart_init(void);
void uart_send(char data);
char uart_read(void);
bool uart_wasRead(void);

#endif /* UART_H_ */
