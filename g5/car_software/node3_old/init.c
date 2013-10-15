/*********************************************
 * Hardware init
 *********************************************/

#include "config.h"
#include "extern.h"
#include "prototyper.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*************************************************
* Timer
*
* Counter0  (8-bit): Timer til ADC konvertering
* Counter1 (16-bit): Timer til gear motor PWM
* Counter2  (8-bit): 
*************************************************/

void ioinit(void)
{
	DDRA|= (1<<PA0);	// Retning A set
	DDRA|= (1<<PA1);	// Retning B set
	DDRA|= (1<<PA2);	// Retning A Enable
	DDRA|= (1<<PA3);	// Retning B Enalbe
		
	DDRB|= (1<<PB5);	// PWM til gearMotor
    DDRE|= (1<<PE5);    // PWM til TEST

	DDRF &=~(1<<PF1);	// Position sense
	DDRF &=~(1<<PF0);	// CS (Current Sense !)

	// Hbro Disable (h-bro slukkes ved start)
	PORTA &=~ (1<<PA0);	
	PORTA &=~ (1<<PA1);	
	PORTA &=~ (1<<PA2);	
	PORTA &=~ (1<<PA3);	

	// Ign cut (output)
	DDRE|= (1<<PE4);
	PORTE &=~ (1<<PE4);

	// Neutral gear (input)
	DDRE &=~ (1<<PE7);
	PORTE |= (1<<PE7); // Pull-up

	// GearKontakter (direkte interface, kan bruges hvis CAN fejler)
	//DDRE&=~(1<<PE5);
	//DDRE&=~(1<<PE6);
	//DDRE&=~(1<<PE7);
	//PORTE |= (1<<PE5); // Pull-up
	//PORTE |= (1<<PE6); // Pull-up
	//PORTE |= (1<<PE7); // Pull-up
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

void pwm16Init(void)
{
	//PWM, 16 bit counter (counter1)
	// (OC1A) Output
    DDRB|= (1<<PB5);  
     
	// Compare match.
	TCCR1A |=(1<<COM1A1);
	TCCR1A &=~(1<<COM1A0);

	// FAST-PWM TOP = ICRn
	TCCR1A &=~(1<<WGM10);
	TCCR1A |=(1<<WGM11);
	TCCR1B |=(1<<WGM12);
	TCCR1B |=(1<<WGM13);

	//Top (11-bit)
	ICR1H = 0x07;
	ICR1L = 0xFF;

	// Prescaler, 64
    TCCR1B &=~(1<<CS10);
	TCCR1B |=(1<<CS11); 
	TCCR1B &=~(1<<CS12);
}

void pwm16Init2(void)
{
	//PWM, 16 bit counter (counter3)
	// (OC3c) Output
    DDRE|= (1<<PE5);
    
	// Compare match.
	TCCR3A |=(1<<COM3C1);
	TCCR3A &=~(1<<COM3C0);
    
	// FAST-PWM TOP = ICRn
	TCCR3A &=~(1<<WGM30);
	TCCR3A |=(1<<WGM31);
	TCCR3B |=(1<<WGM32);
	TCCR3B |=(1<<WGM33);
    
	//Top (11-bit)
	ICR3H = 0x07;
	ICR3L = 0xFF;
    
	// Prescaler, 64
    TCCR3B &=~(1<<CS30);
	TCCR3B |=(1<<CS31);
	TCCR3B &=~(1<<CS32);
    
    OCR3CH = (0x5); //& (64>>8);
	OCR3CL = (0x99); //& (64);
    
}



void counter0Init(void)
{
	TCCR0A |= counter0prescale256;
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

	/* ADC Tigger mode
	ADCSRA |=(1<<ADATE); 

	// ADC trigger source
	ADCSRB &=~(1<<ADTS0); 
	ADCSRB &=~(1<<ADTS1);
	ADCSRB |=(1<<ADTS2);
	*/	

	// ADC interrupt enable
	ADCSRA |=(1<<ADIE);
}
