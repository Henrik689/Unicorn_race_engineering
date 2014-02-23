/*********************************************
 * Functions
 *********************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h> // size_t
#include <stdarg.h> // va args
#include <stdio.h> // vsprintf

#include <avr/io.h>
#include "bitwise.h"
#include "usart.h"

#ifndef NO_USART0_BUFFERED_INPUT
	#include "ringbuffer.h"
	static ringbuffer_t usart0_inBuff = {{0}}; // @todo should this be volatil?
#endif

/**
 * Convert a given baudrate
 * to UBRR prescalar.
 * @param  baudrate Target baudrate
 * @param  mode     UART operation mode
 * @return          UBRR prescalar
 */
static inline uint16_t uart_baud2ubrr(const uint32_t baudrate, enum uart_operationModes_t mode){
	uint16_t ubrr_val;
	switch (mode){
		case UART_MODE_ASYNC_NORMAL:
			ubrr_val = ((F_CPU / (baudrate * 16UL))) - 1;
			break;
		case UART_MODE_ASYNC_DOUBLE:
			ubrr_val = ((F_CPU / (baudrate * 8UL))) - 1;
			break;
		case UART_MODE_SYNC_MASTER:
			ubrr_val = ((F_CPU / (baudrate * 2UL))) - 1;
			break;
		default:
			ubrr_val = 0;
			break;
	}
	return ubrr_val;
}


#ifndef NO_USART0_SUPPORT

void usart0_init(void) {
	const uint32_t baudrate = 115200;

#ifndef NO_USART0_BUFFERED_INPUT
	rb_init(&usart0_inBuff);
	USART0_ENABLE_RX_INTERRUPT();
#endif

	//Enable TXen og RXen
	USART0_ENABLE_RX();
	USART0_ENABLE_TX();

	USART0_SET_1_STOP_BIT();
	USART0_SET_CHAR_SIZE(UART_CHAR_8BIT);

	// Baud rate
	usart0_setBaudrate(baudrate, UART_MODE_ASYNC_NORMAL);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

/**
 * Set USART baudrate and operation mode.
 * @param baudrate baudrate that the USART will use
 * @param mode     USART operation mode
 */
void usart0_setBaudrate(const uint32_t baudrate, enum uart_operationModes_t mode){
	switch (mode) {
		case UART_MODE_ASYNC_NORMAL: USART0_SET_MODE_ASYNC(); break;
		case UART_MODE_ASYNC_DOUBLE: USART0_SET_MODE_ASYNC(); break;
		case UART_MODE_SYNC_MASTER: USART0_SET_MODE_SYNC(); break;
	}

	const uint16_t prescale = uart_baud2ubrr(baudrate, mode);

	UBRR0L = LOW_BYTE(prescale);
	UBRR0H = HIGH_BYTE(prescale);
}

bool usart0_hasData(void){
	return !rb_isEmpty(&usart0_inBuff);
}

uint8_t usart0_getc(void) {
#ifdef NO_USART0_BUFFERED_INPUT
	while(USART0_RX_IS_BUSY());
	return UDR0;
#else
	uint8_t data;
	while(rb_pop(&usart0_inBuff, &data) == 0);
	return data;
#endif
}


int usart0_putc(const uint8_t c) {
#ifndef USART0_NON_UNIX_LIKE_LINE_ENDINGS
	if(c == '\n'){
		usart0_putc('\r');
	}
#endif

	while (USART0_TX_IS_BUSY());
	UDR0 = c;

	return c;
}


int usart0_puts(const char *str) {
	if (str == NULL) return -1;
	int i = 0;

	while(str[i] != '\0'){
		usart0_putc(str[i++]);
	}

	return i;
}


int usart0_putn(size_t n, const char*str) {
	if (str == NULL) return -1;

	int i;
	for (i = 0; i < n; ++i){
		usart0_putc(str[i]);
	}

	return i;
}


int usart0_printf(const char *str, ...){
	if(str == NULL) return -1;

	char buffer[UART0_PRNT_BUFF_SIZE] = {'\0'}; // Warning this might overflow on long str
	va_list args;
	int rc_vsprintf;
	int rc_tx;

	va_start(args, str);
	if((rc_vsprintf = vsnprintf(buffer, UART0_PRNT_BUFF_SIZE, str, args)) < 0){
		return rc_vsprintf; // vsprintf return a negative value on err
	}
	va_end(args);

	if((rc_tx = usart0_puts(buffer)) != rc_vsprintf){
		return -1; // We havn't send the same amount as sprintf wrote the the buffer
	}

	if(rc_tx > UART0_PRNT_BUFF_SIZE) return -UART0_PRNT_BUFF_SIZE; // if buffer overflow

	return rc_tx;
}

#ifndef NO_USART0_BUFFERED_INPUT
ISR(USART0_RX_vect){
	uint8_t data = UDR0;

	rb_push(&usart0_inBuff, data);
}
#endif

#endif

