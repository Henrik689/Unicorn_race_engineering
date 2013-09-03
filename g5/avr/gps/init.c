/*********************************************
 * Hardware init
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <avr/interrupt.h>


void ioinit(void)
{
	// LED
	DDRC|= (1<<PC7);
}

void uartinit0(void)
{
	//Enable TXen og RXen
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 
	
	// Format: 8data, 1 stop bit
	UCSR0C = (3<<UCSZ00);

	// Baud rate
	UBRR0L = BAUD_PRESCALE0;
	UBRR0H = (BAUD_PRESCALE0 >> 8);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	UCSR0B|=(1<<RXCIE0);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

void uartinit1(void)
{
	//Enable TXen og RXen
	UCSR1B = (1<<RXEN1)|(1<<TXEN1); 
	
	// Format: 8data, 1 stop bit
	UCSR1C = (3<<UCSZ10);

	// Baud rate
	UBRR1L = BAUD_PRESCALE1;
	UBRR1H = (BAUD_PRESCALE1 >> 8);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	UCSR1B|=(1<<RXCIE1);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}
