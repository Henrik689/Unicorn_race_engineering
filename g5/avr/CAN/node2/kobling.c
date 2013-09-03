#include "config.h"
#include "compiler.h"
#include "kobling.h"

void init_kobling_adc()
{
	// ADC channel
	ADMUX = 0;

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
	ADCSRA |=(1<<ADATE); 

	// ADC trigger source
	ADCSRB &=~(1<<ADTS0); 
	ADCSRB |=(1<<ADTS1);
	ADCSRB |=(1<<ADTS2);

	// ADC interrupt enable
	ADCSRA |=(1<<ADIE);

	// Timer til adc auto-trigger
        TCCR1B |= 1<<CS11;              // prescaler til 8
	TIMSK1 |=(1<<TOIE1);
}
