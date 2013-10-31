#ifndef UART_H
#define UART_H

#include <stdint.h>

enum uart_number_t { 
	UART_NUMBER_0,
	UART_NUMBER_1
};

enum uart_operationModes_t {
	UART_MODE_ASYNC_NORMAL,
	UART_MODE_ASYNC_DOUBLE,
	UART_MODE_SYNC_MASTER
};


void uart_enableRX(const enum uart_number_t number);
void uart_enableTX(const enum uart_number_t number);
void uart_enableRXInterupt(const enum uart_number_t number);
void uart_enableTXInterupt(const enum uart_number_t number);

uint16_t uart_baud2ubrr(const uint32_t baudrate, const enum uart_operationModes_t mode);

void uart_init(enum uart_number_t);

void uart_txchar(enum uart_number_t n, const unsigned char c);
void uart_txstring(enum uart_number_t n, char *str);

#endif /* UART_H */
