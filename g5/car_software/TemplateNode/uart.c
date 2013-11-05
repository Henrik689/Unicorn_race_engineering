/*********************************************
 * Functions
 *********************************************/

#include <stdint.h>
#include "config.h" // F_CPU

#include <avr/io.h>
#include "bitwise.h"
#include "uart.h"


/*
	UBRRn: Baud Rate Registor
		value of UBRRnL and UBRRnH

*/


/*
UCSRB: USART Control and Status Register B
------------------------------------------
*/

void uart_enableRX(const enum uart_number_t number){
	// use RXEN1 instead?
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0B, RXEN); 
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1B, RXEN);
			break;
	}
}

void uart_enableTX(const enum uart_number_t number){
	// use TXEN1 instead?
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0B, TXEN);
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1B, TXEN);
			break;
	}
}

void uart_enableRXInterupt(const enum uart_number_t number){
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0B, RXCIE);
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1B, RXCIE);
			break;
	}
}

void uart_enableTXInterupt(const enum uart_number_t number){
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0B, TXCIE);
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1B, TXCIE);
			break;
	}
}

/*
UCSRC: USART Control And Status Register C
------------------------------------------
*/

void uart_setModeAsync(const enum uart_number_t number){
	// UMSEL: USART Mode Select
	switch(number){
		case UART_NUMBER_0:
			BIT_CLEAR(UCSR0C, UMSEL);
			break;
		case UART_NUMBER_1:
			BIT_CLEAR(UCSR1C, UMSEL);
			break;
	}
}

void uart_setModeSync(const enum uart_number_t number){
	// UMSEL: USART Mode Select
	switch(number){
		case UART_NUMBER_0:
			BIT_SET(UCSR0C, UMSEL);
			break;
		case UART_NUMBER_1:
			BIT_SET(UCSR1C, UMSEL);
			break;
	}
}

void uart_setNumberOfStopBits(const enum uart_number_t number, unsigned int numStopBits){
	// USBS: USART Stop Bit Select
	if(numStopBits == 1){
		switch(number){
			case UART_NUMBER_0:
				BIT_CLEAR(UCSR0C, USBS);
				break;
			case UART_NUMBER_1:
				BIT_CLEAR(UCSR1C, USBS);
				break;
		}
	}else if(numStopBits == 2){
		switch(number){
			case UART_NUMBER_0:
				BIT_SET(UCSR0C, USBS);
				break;
			case UART_NUMBER_1:
				BIT_SET(UCSR1C, USBS);
				break;
		}
	}
}

void uart_setCharSize(enum uart_number_t number, enum uart_charSelect_t size){
	// UCSZ: USART Character size
	//const unsigned int bitmask = (0x07 << 1); 
	//const unsigned int bitmask = (1 << 0x07);

	switch(number){
		case UART_NUMBER_0:
			//BITMASK_CLEAR(UCSR0C, bitmask);
			//BITMASK_SET(UCSR0C, BITMASK_CHECK(size, bitmask));
			UCSR0C |= (size << UCSZ0);
			break;

		case UART_NUMBER_1:
			UCSR1C |= (size << UCSZ1);
			//BITMASK_CLEAR(UCSR1C, bitmask);
			//BITMASK_SET(UCSR1C, BITMASK_CHECK(size, bitmask));
			break;
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
	//UCSR1C = (3<<UCSZ10);
	uart_setModeAsync(uartNum);
	uart_setNumberOfStopBits(uartNum, 1);
	uart_setCharSize(uartNum, UART_CHAR_8BIT);

	// Baud rate
	uart_setBaudRate(uartNum, baudrate, UART_MODE_ASYNC_NORMAL);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	//UCSR1B|=(1<<RXCIE1);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

unsigned char uart_getChar(enum uart_number_t n){
	unsigned char result = 0;

	switch(n){
		case UART_NUMBER_0:
			while(!BIT_CHECK(UCSR0A, RXC0));
			result = UDR0;
			break;
		case UART_NUMBER_1:
			while(!BIT_CHECK(UCSR1A, RXC1));
			result = UDR1;
			break;
	}
	return result;
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
