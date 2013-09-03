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
* Counter1 (16-bit): -
* Counter2  (8-bit): Timer til gear motor PWM
*************************************************/

void ioinit(void)
{
	DDRA|= (1<<PA7);	// Retning A set
	DDRA|= (1<<PA6);	// Retning A Enable
	DDRA|= (1<<PA5);	// Retning B Enalbe
	DDRA|= (1<<PA4);	// Retning B set
	DDRD|= (1<<PD7);	// PWM til gearMotor

	DDRA &=~(1<<PA0);	// Position sense
	DDRA &=~(1<<PA1);	// CS (Current Sense !)

	// Hbro Disable
	PORTA &=~ (1<<PA7);	
	PORTA &=~ (1<<PA6);	
	PORTA &=~ (1<<PA5);	
	PORTA &=~ (1<<PA4);	

	// PCINT (og led)
	DDRC|= (1<<PC7);

	//LED
	DDRC|= (1<<PC0);
	DDRC|= (1<<PC1);
	DDRC|= (1<<PC2);

	// Ign cut
	DDRC|= (1<<PC3);

	// GearKontakter
	DDRB&=~(1<<PB0);
	DDRB&=~(1<<PB1);
	DDRB&=~(1<<PB2);
	PORTB |= (1<<PB0); // Pull-up
	PORTB |= (1<<PB1); // Pull-up
	PORTB |= (1<<PB2); // Pull-up
}

void uartinit(void)
{
	//Enable TXen og RXen
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); 
	
	// Format: 8data, 1 stop bit
	UCSR0C = (3<<UCSZ00);

	// Baud rate
	UBRR0L = BAUD_PRESCALE;
	UBRR0H = (BAUD_PRESCALE >> 8);
	
	// Rx Uart interrupt (Receive Complete Interrupt)
	UCSR0B|=(1<<RXCIE0);

	// Tx Uart interrupt (Transmit Complete Interrupt)
	//UCSR0B|=(1<<TXCIE0);
}

void pwm8Init(void)
{
	// (OC2A) Output
	DDRD|= (1<<PD7);      

	// Opsætning af compare match.
	TCCR2A |=(1<<COM2A1);
	TCCR2A &=~(1<<COM2A0);

	// FAST-PWM
	TCCR2A |=(1<<WGM20);
	TCCR2A |=(1<<WGM21);

	// Prescaler
    TCCR2B |=(1<<CS20); // 2.2 kHz
	TCCR2B |=(1<<CS21);

	PWM_duty_cycle_A_set(0);
}

void pwm16Init(void)
{
	//PWM, 16 bit counter (counter1)
	// (OC1A) Output
    DDRD|= (1<<PD4);  
     
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

void pcintInit(void)
{
	// PCINT2 Enable
	PCICR |=(1<<PCIE2);

	// PCMSK2 pin trigger
	PCMSK2 = 0b10000000;
}
