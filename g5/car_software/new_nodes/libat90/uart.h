#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdlib.h> // size_t
#include <stdarg.h> // va args
#include <avr/io.h>
#include <bitwise.h>

#define UART0_PRNT_BUFF_SIZE (256) //!< The size of the uart print buffer

#define USART0_ENABLE_RX()	BIT_SET(UCSR0B, RXEN)
#define USART0_ENABLE_TX()	BIT_SET(UCSR0B, TXEN)

#define USART0_ENABLE_RX_INTERRUPT()	BIT_SET(UCSR0B, RXCIE)
#define USART0_ENABLE_TX_INTERRUPT()	BIT_SET(UCSR0B, TXCIE)

#define USART0_SET_MODE_ASYNC()	BIT_CLEAR(UCSR0C, UMSEL)
#define USART0_SET_MODE_SYNC()	BIT_SET(UCSR0C, UMSEL)

#define USART0_SET_1_STOP_BIT()	BIT_CLEAR(UCSR0C, USBS)
#define USART0_SET_2_STOP_BIT()	BIT_SET(UCSR0C, USBS)

#define USART0_RX_IS_BUSY()	(!(BIT_CHECK(UCSR0A, RXC0)))
#define USART0_TX_IS_BUSY()	(!(BIT_CHECK(UCSR0A, UDRE0)))

#define USART0_SET_CHAR_SIZE(size) { \
	BITMASK_CLEAR(UCSR0C, (0x07 << UCSZ0)); \
	UCSR0C |= (size << UCSZ0); \
}



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

void usart0_setBaudrate(const uint32_t baudrate, enum uart_operationModes_t mode);
unsigned char usart0_getc(void);
int usart0_putc(const uint8_t c);
int usart0_puts(const char *str);
int usart0_putn(size_t n, const char*str);
int usart0_printf(const char *str, ...);

#endif /* UART_H */
