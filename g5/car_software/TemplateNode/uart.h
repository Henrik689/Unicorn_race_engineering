#ifndef UART_H
#define UART_H

#include "config.h" // F_CPU

#define USART_BAUDRATE 115200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void uart_init(void);

void uart_txchar(const char c);
void uart_txstring(char *str);

#endif /* UART_H */
