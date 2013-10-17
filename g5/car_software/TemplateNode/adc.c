#include <avr/interrupt.h>
#include "adc.h"

void adc_setPrescaler(enum prescalar_t p){
	// The ADC requires a frequency between 50KHz to 200KHz
	// this frequency is set by the prescalar and the cpu_f
	// ADC frequency = cpu_f / prescalar
	// 16mhz / 128 = 150khz
	// this is well explaind at:
	// http://www.avrbeginners.net/architecture/adc/adc.html#adcsr

	switch(p){
		case PRESCALAR_2:
			ADCSRA |= (0<<ADPS0);
			ADCSRA |= (0<<ADPS1);
			ADCSRA |= (0<<ADPS2);
			break;
		case PRESCALAR_4:
			ADCSRA |= (1<<ADPS0);
			ADCSRA |= (0<<ADPS1);
			ADCSRA |= (0<<ADPS2);
			break;
		case PRESCALAR_8:
			ADCSRA |= (0<<ADPS0);
			ADCSRA |= (1<<ADPS1);
			ADCSRA |= (0<<ADPS2);
			break;
		case PRESCALAR_16:
			ADCSRA |= (1<<ADPS0);
			ADCSRA |= (1<<ADPS1);
			ADCSRA |= (0<<ADPS2);
			break;
		case PRESCALAR_32:
			ADCSRA |= (0<<ADPS0);
			ADCSRA |= (0<<ADPS1);
			ADCSRA |= (1<<ADPS2);
			break;
		case PRESCALAR_64:
			ADCSRA |= (0<<ADPS0);
			ADCSRA |= (1<<ADPS1);
			ADCSRA |= (1<<ADPS2);
			break;
		case PRESCALAR_128:
			ADCSRA |= (1<<ADPS0);
			ADCSRA |= (1<<ADPS1);
			ADCSRA |= (1<<ADPS2);
			break;
	}
}

void adc_setChannel(unsigned int ch){
	// set the channel between 0 and 7 (including)
	ADMUX = (ch & 0x0F);
}
