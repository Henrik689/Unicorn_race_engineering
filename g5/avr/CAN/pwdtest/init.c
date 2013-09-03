/*********************************************
 * Hardware init
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*************************************************
* Timer
*
* Counter0  (8-bit): Timer til ADC konvertering
* Counter1 (16-bit): Timer til Motor PWM
* Counter2  (8-bit): Timer til ventil PWM
*************************************************/

void uartinit(void)
{
	//"Tænder" for TXen og RXen (send og modtag portene)
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 
	
	// Opsætning af format: 8data, 1 stop bit
	UCSR0C = (3<<UCSZ00);

	// Opsætning af baud rate
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = (BAUD_PRESCALE >> 8);
	
	// Opsætning af Rx Uart interrupt (Receive Complete Interrupt) til indkommen seriel data
	UCSR0B|=(1<<RXCIE0);
}

void pwm8Init(void)
{
	//PWM, 8 bit counter (counter0)
	// (OC2A) Output
    DDRD|= (1<<PD6);  
	// (OC2B) Output
	DDRD|= (1<<PD7);      

	// Opsætning af compare match.
	TCCR2A |=(1<<COM2A1);
	TCCR2A &=~(1<<COM2A0);

	TCCR2A |=(1<<COM2B1);
	TCCR2A &=~(1<<COM2B0);

	// FAST-PWM
	TCCR2A |=(1<<WGM20);
	TCCR2A |=(1<<WGM21);

	// Prescaler, 1
    TCCR2B |=(1<<CS20);

	PWM_duty_cycle_A_set(0);
	PWM_duty_cycle_B_set(0);
}

void pwm16Init(void)
{
	//PWM, 16 bit counter (counter1)
	// (OC1A) Output
    DDRD|= (1<<PD4);  
    DDRD|= (1<<PD5);  
     
	// Opsætning af compare match.
	TCCR1A |=(1<<COM1A1);
	TCCR1A |=(1<<COM1A0);

	// FAST-PWM TOP = ICRn
	TCCR1A &=~(1<<WGM10);
	TCCR1A |=(1<<WGM11);
	TCCR1B |=(1<<WGM12);
	TCCR1B |=(1<<WGM13);

	//Top (10-bit)
	ICR1H = 0x07;
	ICR1L = 0xFF;

	// Prescaler, 64
    TCCR1B &=~(1<<CS10);
	TCCR1B |=(1<<CS11); 
	TCCR1B &=~(1<<CS12); 

	// Compare match værdi
	PWM_duty_cycle_A16_set(0);
}

void counter0Init(void)
{
	//TCCR0B |=(1<<CS00);
	//TCCR0B &=~(1<<CS01);
	//TCCR0B |=(1<<CS02);

	TCCR0B |= counter0prescale256;

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
	ADCSRA &=~(1<<ADPS1);
	ADCSRA |=(1<<ADPS2);

	// ADC Tigger mode
	/*
	ADCSRA |=(1<<ADATE); 

	// ADC trigger source
	ADCSRB &=~(1<<ADTS0); 
	ADCSRB &=~(1<<ADTS1);
	ADCSRB |=(1<<ADTS2);
	*/
	// ADC interrupt enable
	ADCSRA |=(1<<ADIE);
}
