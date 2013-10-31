/*********************************************
 * Functions
 *********************************************/

#include <avr/interrupt.h>
#include <stdint.h>
#include "config.h" // F_CPU
#include "bitwise.h"
#include "uart.h"


/*
	UBRRn: Baud Rate Registor
		value of UBRRnL and UBRRnH

*/

void uart_enableRX(const enum uart_number_t number){
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0B, RXEN1);
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1B, RXEN1);	
	}
}

void uart_enableTX(const enum uart_number_t number){
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0B, TXEN1);
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1B, TXEN1);
	}
}

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

void uart_setBaudRate(enum uart_number_t number, const uint32_t baudrate, enum uart_operationModes_t mode){
	const uint16_t prescale = uart_baud2ubrr(baudrate, mode);
	switch(number){
		case UART_NUMBER_0:
			UBRR0L = prescale;
			UBRR0H = (prescale >> 8);
			break;
		case UART_NUMBER_1:
			UBRR1L = prescale;
			UBRR1H = (prescale >> 8);
			break;
	}
}

void uart_init(enum uart_number_t uartNum) {
	const uint32_t baudrate = 115200;

	//Enable TXen og RXen
	uart_enableRX(uartNum);
	uart_enableTX(uartNum);
	
	// Format: 8data, 1 stop bit
	UCSR1C = (3<<UCSZ10);

	// Baud rate
	uart_setBaudRate(uartNum, baudrate, UART_MODE_ASYNC_NORMAL);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	//UCSR1B|=(1<<RXCIE1);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

void uart_txchar(enum uart_number_t n, const unsigned char c) {
	switch(n){
		case UART_NUMBER_0:
			while ( !(BIT_CHECK(UCSR0A, UDRE0)) ); 
			UDR0 = c;
			break;
		case UART_NUMBER_1:
			while ( !(BIT_CHECK(UCSR1A, UDRE1)) ); 
			UDR1 = c;
			break;
	}
}

void uart_txstring(enum uart_number_t n, char *str) {
	char *c = str;
	while(*c){
		uart_txchar(n, *c++);
	}
}
