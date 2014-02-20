/*********************************************
 * Hardware init
 *********************************************/

#include <avr/io.h>
#include <io.h>
#include <bitwise.h>
#include "timer.h"
#include "init.h"

/*************************************************
* Timer
*
* Counter0  (8-bit): Timer til ADC konvertering
*************************************************/

void ioinit(void)
{
    // Servo
    SET_PIN_MODE(PORTE, PE5, INPUT);
    SET_PIN_MODE(PORTF, PF1, INPUT);
    //DDRE|= (1<<PE5);    // PWM til Servo
	//DDRF &=~(1<<PF1);	// Position sense

	// Ign cut (output)
	SET_PIN_MODE(PORTE, PE4, OUTPUT);
	DIGITAL_WRITE(PORTE, PE4, LOW);
	//DDRE|= (1<<PE4);
	//PORTE &=~ (1<<PE4);

	// Neutral gear (input)
	SET_PIN_MODE(PORTE, PE7, INPUT_PULLUP);
	//DDRE &=~ (1<<PE7);
	//PORTE |= (1<<PE7); // Pull-up
}


void pwm16Init2(void)
{
	//PWM, 16 bit counter (counter3)
	// (OC3c) Output
    SET_PIN_MODE(PORTE, PE5, OUTPUT);

    // Set Wave Generator mode to mode 14, FAST-PWM TOP = ICRn (table 13-4)
	//TCCR3A &=~ (1<<WGM30);
	//TCCR3A |= (1<<WGM31);
	//TCCR3B |= (1<<WGM32);
	//TCCR3B |= (1<<WGM33);
	timer_setMode(TIMER3, 14);

	// These bits are set in order to control the behavior of Output Compare pin (OC0)(table 13-2)
	BIT_SET(TCCR3A, COM3C1);
	BIT_CLEAR(TCCR3A, COM3C0);


	// set the Input Capture Registers Top (11-bit)
	ICR3H = 0x07;
	ICR3L = 0xFF;

	// Set Clock Select Bits for a Prescaler = 64 (table 13-5)
	BIT_SET(TCCR3B, CS30);
	BIT_SET(TCCR3B, CS31);
	BIT_CLEAR(TCCR3B, CS32);
}



void counter0Init(void)
{
	timer_setPrescaler(PRESCALAR_256);
	//BIT_SET(TIMSK0, TOIE0); // This does not have an ISR!?
}

void adcInit(unsigned int channel)
{
	// ADC channel
	ADMUX = (channel & 0x0f);

    //ADC Left adjust
   // ADMUX &=~(1<<ADLAR);

	// Vref config
	ADMUX |= (1<<REFS0);
	ADMUX &=~ (1<<REFS1);


	// ADC ENABLE
	ADCSRA |= (1<<ADEN);

	// ADC frequency prescaler
	ADCSRA |= (1<<ADPS0);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS2);

     // ADC Tigger mode
     ADCSRA |= (1<<ADATE);

     // ADC trigger source
     ADCSRB &=~ (1<<ADTS0);
     ADCSRB &=~ (1<<ADTS1);
     ADCSRB |= (1<<ADTS2);


	// ADC interrupt enable
	ADCSRA |= (1<<ADIE);

}
