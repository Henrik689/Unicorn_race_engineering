/*********************************************
 * Hardware init
 *********************************************/

#include "config.h"
#include "prototyper.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*************************************************
* Timer
*
* Counter0  (8-bit):
* Counter1 (16-bit): Puls width counter1
* Counter2  (8-bit): 
* Counter3 (16-bit): Puls width counter2
*************************************************/

void ioinit(void)
{
/*
	DDRA|= (1<<PA0);	// Retning A set
	DDRA|= (1<<PA1);	// Retning B set
	DDRA|= (1<<PA2);	// Retning A Enable
	DDRA|= (1<<PA3);	// Retning B Enalbe
		
	DDRB|= (1<<PB5);	// PWM til gearMotor

	DDRF &=~(1<<PF1);	// Position sense
	DDRF &=~(1<<PF0);	// CS (Current Sense !)

	// Hbro Disable (h-bro slukkes ved start)
	PORTA &=~ (1<<PA0);	
	PORTA &=~ (1<<PA1);	
	PORTA &=~ (1<<PA2);	
	PORTA &=~ (1<<PA3);	

	// Ign cut
	DDRE|= (1<<PE4);
	PORTE &=~ (1<<PE4);

	// GearKontakter
	DDRE&=~(1<<PE5);
	DDRE&=~(1<<PE6);
	DDRE&=~(1<<PE7);
	PORTE |= (1<<PE5); // Pull-up
	PORTE |= (1<<PE6); // Pull-up
	PORTE |= (1<<PE7); // Pull-up
*/
}

void uartinit(void)
{
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

void extIntInit(void) 
{
	// ext int 6,7 init
	EICRB |= 0b11110000; // Rising edge trigger ext int
	EIMSK |= 0b01000000; // Ext int enable (int 6) 
}

void counter1Init(void) 
{
	TCCR1B |= counter0prescale8; // Giver 2^16 ved 2.38 km/t (min meas speed)
	TIMSK1 |=(1<<TOIE0);
}

void counter3Init(void) 
{
	TCCR3B |= counter0prescale8; // Giver 2^16 ved 2.38 km/t (min meas speed)
	TIMSK3 |=(1<<TOIE0);
}

void counter0Init(void) 
{
	TCCR0A |= counter0prescale1024;
	TIMSK0 |=(1<<TOIE0);
}

void adcInit(unsigned int channel)
{
	// ADC channel
	ADMUX=(channel & 0x0f);

	// Vref config
	ADMUX |=(1<<REFS0); 
	ADMUX &=~(1<<REFS1);

	// ADC ENABLE
	ADCSRA |=(1<<ADEN); 

	// ADC frequency prescaler
	ADCSRA |=(1<<ADPS0);
	ADCSRA |=(1<<ADPS1);
	ADCSRA |=(1<<ADPS2);

	// ADC interrupt enable
	ADCSRA |=(1<<ADIE);
}
