/*********************************************
 * Functions
 *********************************************/

#include <avr/interrupt.h>
#include "config.h" // F_CPU
#include "bitwise.h"
#include "uart.h"

//#define USART_BAUDRATE 115200
//#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

// static void adcStop(void)
// {
// 	ADCSRA &=~(1<<ADEN); // ADC prescaler disable
// }

void uart_init(void) {
	const uint32_t baudrate = 115200;
	const uint32_t prescale = ((F_CPU / (baudrate * 16UL))) - 1;

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
