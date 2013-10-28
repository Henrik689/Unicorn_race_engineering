/*********************************************
 * Functions
 *********************************************/

#include <avr/interrupt.h>
#include <stdint.h>
#include "config.h" // F_CPU
#include "bitwise.h"
#include "uart.h"

//#define USART_BAUDRATE 115200
//#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/*
	UBRRn: Baud Rate Registor
		value of UBRRnL and UBRRnH

*/

uint16_t uart_baud2ubrr(const uint32_t baudrate, const enum uart_operationModes_t mode){
	uint16_t val;
	switch (mode){
		case UART_MODE_ASYNC_NORMAL:
			val = ((F_CPU / (baudrate * 16UL))) - 1;
			break;
		case UART_MODE_ASYNC_DOUBLE:
			val = ((F_CPU / (baudrate * 8UL))) - 1;
			break;
		case UART_MODE_SYNC_MASTER:
			val = ((F_CPU / (baudrate * 2UL))) - 1;
			break;
		default:
			val = 0;
			break;
	}
	return val;
}

void uart_init(void) {
	const uint32_t baudrate = 115200;
	const uint16_t prescale = uart_baud2ubrr(baudrate, UART_MODE_ASYNC_NORMAL);

	//Enable TXen og RXen
	BIT_SET(UCSR1B, RXEN1);
	BIT_SET(UCSR1B, TXEN1);
	
	// Format: 8data, 1 stop bit
	UCSR1C = (3<<UCSZ10);

	// Baud rate
	UBRR1L = prescale;
	UBRR1H = (prescale >> 8);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	//UCSR1B|=(1<<RXCIE1);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

void uart_txchar(const char c) {
	while ( !(BIT_CHECK(UCSR1A, UDRE1)) ); 
	UDR1 = c;
}

void uart_txstring(char *str) {
	char *c = str;
	while(*c){
		uart_txchar(*c++);
	}
}
