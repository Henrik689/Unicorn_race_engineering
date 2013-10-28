#ifndef UART_H
#define UART_H

#include <stdint.h>

enum uart_operationModes_t {
	UART_MODE_ASYNC_NORMAL,
	UART_MODE_ASYNC_DOUBLE,
	UART_MODE_SYNC_MASTER
};

uint16_t uart_baud2ubrr(const uint32_t baudrate, const enum uart_operationModes_t mode);

void uart_init(void);

void uart_txchar(const char c);
void uart_txstring(char *str);

#endif /* UART_H */
