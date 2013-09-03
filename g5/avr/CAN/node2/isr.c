#include <avr/interrupt.h>
#include <avr/io.h>
#include "led.h"

volatile char LED_REG1 = 0;
volatile char LED_REG2 = 0;

unsigned int ADCconv = 0;
unsigned int adlow = 0;
unsigned int adhigh = 0;

/* tænder dioder der er sat i LED_REG1 og LED_REG2 */
ISR(TIMER0_OVF_vect)
{
        PORTE |= LED_REG1;
        PORTB |= LED_REG2;
}

ISR(TIMER1_OVF_vect)
{
}

/* Slukker rpm dioder ved comp */
ISR(TIMER0_COMP_vect)
{
        PORTE = 0;
        PORTB = 0;
}

ISR(ADC_vect)
{
/*	SEG_9;
	adlow=ADCL;
        adhigh=ADCH;
	ADCconv = (unsigned int)((adhigh<<8)|(adlow & 0xFF));
	disp_rpm(ADCconv/60);*/
}

ISR(INT2_vect)
{
/*	SEG_7;*/
}

ISR(INT3_vect)
{
/*	SEG_8;*/
}
/*ISR(CANIT)
{

}*/
