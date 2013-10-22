/*********************************************
 * Functions
 *********************************************/

#include <avr/interrupt.h>
#include "uart.h"


// static void adcStop(void)
// {
// 	ADCSRA &=~(1<<ADEN); // ADC prescaler disable
// }

void uart_init(void) {
	//Enable TXen og RXen
	UCSR1B = (1<<RXEN1)|(1<<TXEN1); 
	
	// Format: 8data, 1 stop bit
	UCSR1C = (3<<UCSZ10);

	// Baud rate
	UBRR1L = BAUD_PRESCALE;
	UBRR1H = (BAUD_PRESCALE >> 8);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	//UCSR1B|=(1<<RXCIE1);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

void uart_txchar(char c) {
	while ( !(UCSR1A & (1 << UDRE1)) ); 
	UDR1 = c;
}

void uart_txstring(char *str) {
	char *c = str;
	while(*c){
		uart_txchar(*c++);
	}
}
