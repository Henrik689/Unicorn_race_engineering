#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h> // size_t
#include <stdarg.h> // va args
#include <avr/io.h>
#include <bitwise.h>

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

#ifndef NO_USART0_SUPPORT

	#define UART0_PRNT_BUFF_SIZE (256) //!< The size of the uart print buffer

	#define USART0_ENABLE_RX()	BIT_SET(UCSR0B, RXEN)
	#define USART0_ENABLE_TX()	BIT_SET(UCSR0B, TXEN)

	#define USART0_ENABLE_RX_INTERRUPT()	BIT_SET(UCSR0B, RXCIE)
	#define USART0_ENABLE_TX_INTERRUPT()	BIT_SET(UCSR0B, TXCIE)

	#define USART0_DISABLE_DATA_REG_EMPTY_INTERRUPT() BIT_CLEAR(UCSR0B, UDRIE)
	#define USART0_ENABLE_DATA_REG_EMPTY_INTERRUPT() BIT_SET(UCSR0B, UDRIE)

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

	void usart0_init(void);
	void usart0_setBaudrate(const uint32_t baudrate, enum uart_operationModes_t mode);
	bool usart0_hasData(void);
	uint8_t usart0_getc(void);
	int usart0_putc(const uint8_t c);
	int usart0_puts(const char *str);
	int usart0_putn(size_t n, const uint8_t *str);
	int usart0_printf(const char *str, ...);

#endif

#ifndef NO_USART1_SUPPORT

	#define UART1_PRNT_BUFF_SIZE (256) //!< The size of the uart print buffer

	#define USART1_ENABLE_RX()	BIT_SET(UCSR1B, RXEN)
	#define USART1_ENABLE_TX()	BIT_SET(UCSR1B, TXEN)

	#define USART1_ENABLE_RX_INTERRUPT()	BIT_SET(UCSR1B, RXCIE)
	#define USART1_ENABLE_TX_INTERRUPT()	BIT_SET(UCSR1B, TXCIE)

	#define USART1_DISABLE_DATA_REG_EMPTY_INTERRUPT() BIT_CLEAR(UCSR1B, UDRIE)
	#define USART1_ENABLE_DATA_REG_EMPTY_INTERRUPT() BIT_SET(UCSR1B, UDRIE)

	#define USART1_SET_MODE_ASYNC()	BIT_CLEAR(UCSR1C, UMSEL)
	#define USART1_SET_MODE_SYNC()	BIT_SET(UCSR1C, UMSEL)

	#define USART1_SET_1_STOP_BIT()	BIT_CLEAR(UCSR1C, USBS)
	#define USART1_SET_2_STOP_BIT()	BIT_SET(UCSR1C, USBS)

	#define USART1_RX_IS_BUSY()	(!(BIT_CHECK(UCSR1A, RXC1)))
	#define USART1_TX_IS_BUSY()	(!(BIT_CHECK(UCSR1A, UDRE1)))

	#define USART1_SET_CHAR_SIZE(size) { \
		BITMASK_CLEAR(UCSR1C, (0x07 << UCSZ1)); \
		UCSR1C |= (size << UCSZ1); \
	}

	void usart1_init(void);
	void usart1_setBaudrate(const uint32_t baudrate, enum uart_operationModes_t mode);
	bool usart1_hasData(void);
	uint8_t usart1_getc(void);
	int usart1_putc(const uint8_t c);
	int usart1_puts(const char *str);
	int usart1_putn(size_t n, const uint8_t *str);
	int usart1_printf(const char *str, ...);

#endif

#endif /* USART_H */
