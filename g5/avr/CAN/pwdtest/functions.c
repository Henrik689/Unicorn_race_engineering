/*********************************************
 * Functions
 *********************************************/

#include <config.h>
#include <prototyper.h>
#include <extern.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

void PWM_duty_cycle_A_set(unsigned int x)
{
	OCR2A = x; 
}

void PWM_duty_cycle_B_set(unsigned int x)
{
	OCR2B = x; 
}

void PWM_duty_cycle_A16_set(unsigned int x)
{
	OCR1AH = (0xFF) & (x>>8);
	OCR1AL = (0xFF) & (x); 
}

void adcStop(void)
{
	ADCSRA &=~(1<<ADEN); // ADC prescaler disable
}

void sendtekst(char *tekstarray)
{
	short int i;
	for (i = 0; tekstarray[i] != '\0'; i++)
	{	
		while ((UCSR0A & (1 << UDRE0)) == 0) {};
		UDR0 = tekstarray[i];
	}
}
