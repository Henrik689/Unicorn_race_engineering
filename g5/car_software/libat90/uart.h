#ifndef UART_H
#define UART_H

#include <stdint.h>

/*
Alias macro-functions.
These make working with the uart a little
more pleasent.
------------------------------------------
*/
#define uart0_enableRX()			uart_enableRX(UART_NUMBER_0)
#define uart0_enableTX()			uart_enableTX(UART_NUMBER_0)
#define uart0_enableRXInterrupt() 	uart_enableRXInterrupt(UART_NUMBER_0)
#define uart0_enableTXInterrupt() 	uart_enableTXInterrupt(UART_NUMBER_0)

#define uart1_enableRX()			uart_enableRX(UART_NUMBER_1)
#define uart1_enableTX()			uart_enableTX(UART_NUMBER_1)
#define uart1_enableRXInterrupt() 	uart_enableRXInterrupt(UART_NUMBER_1)
#define uart1_enableTXInterrupt() 	uart_enableTXInterrupt(UART_NUMBER_1)

#define uart0_setModeAsync() 					uart_setModeAsync(UART_NUMBER_0)
#define uart0_setModeSync()						uart_setModeSync(UART_NUMBER_0)
#define uart0_setNumberOfStopBits(numStopBits) 	uart_setNumberOfStopBits(UART_NUMBER_0, (numStopBits))
#define uart0_setCharSize(size)					uart_setCharSize(UART_NUMBER_0, (size))

#define uart1_setModeAsync() 					uart_setModeAsync(UART_NUMBER_1)
#define uart1_setModeSync()						uart_setModeSync(UART_NUMBER_1)
#define uart1_setNumberOfStopBits(numStopBits) 	uart_setNumberOfStopBits(UART_NUMBER_1, (numStopBits))
#define uart1_setCharSize(size)					uart_setCharSize(UART_NUMBER_1, (size))

#define uart0_init()	uart_init(UART_NUMBER_0)
#define uart1_init()	uart_init(UART_NUMBER_1)

#define uart0_getChar()	uart_getChar(UART_NUMBER_0)
#define uart1_getChar()	uart_getChar(UART_NUMBER_1)

#define uart0_txarr(arr, length)	(UART_NUMBER_0, (arr), (length))
#define uart1_txarr(arr, length)	(UART_NUMBER_1, (arr), (length))

#define uart0_txchar(c)		uart_txchar(UART_NUMBER_0, (c))
#define uart1_txchar(c)		uart_txchar(UART_NUMBER_1, (c))

#define uart0_txstring(str) uart_txstring(UART_NUMBER_0, (str))
#define uart1_txstring(str) uart_txstring(UART_NUMBER_1, (str))

#define uart0_printf(str, ...) uart_printf(UART_NUMBER_0, (str), ##__VA_ARGS__)
#define uart1_printf(str, ...) uart_printf(UART_NUMBER_1, (str), ##__VA_ARGS__)



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
void uart_enableRXInterrupt(const enum uart_number_t number);
void uart_enableTXInterrupt(const enum uart_number_t number);


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
void uart_printf(enum uart_number_t n, char *str, ...);

#endif /* UART_H */
