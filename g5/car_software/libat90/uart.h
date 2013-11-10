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

enum uart_charSelect_t {
	UART_CHAR_5BIT = 0x00,
	UART_CHAR_6BIT = 0x01,
	UART_CHAR_7BIT = 0x02,
	UART_CHAR_8BIT = 0x03,
	UART_CHAR_9BIT = 0x07
};

/*
UCSRB: USART Control and Status Register B
------------------------------------------
*/
void uart_enableRX(const enum uart_number_t number);
void uart_enableTX(const enum uart_number_t number);
void uart_enableRXInterupt(const enum uart_number_t number);
void uart_enableTXInterupt(const enum uart_number_t number);


/*
UCSRC: USART Control And Status Register C
------------------------------------------
*/
void uart_setModeAsync(const enum uart_number_t number);
void uart_setModeSync(const enum uart_number_t number);
void uart_setNumberOfStopBits(const enum uart_number_t number, unsigned int numStopBits);
void uart_setCharSize(enum uart_number_t number, enum uart_charSelect_t size);

uint16_t uart_baud2ubrr(const uint32_t baudrate, const enum uart_operationModes_t mode);

void uart_init(enum uart_number_t);

unsigned char uart_getChar(enum uart_number_t n);
void uart_txarr(enum uart_number_t n, const unsigned char *arr, size_t length);
void uart_txchar(enum uart_number_t n, const unsigned char c);
void uart_txstring(enum uart_number_t n, char *str);

#endif /* UART_H */
