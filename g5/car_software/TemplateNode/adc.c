#include <avr/interrupt.h>
#include <stdint.h>
#include "adc.h"

void adc_setPrescaler(enum adc_prescalar_t p){
	// The ADC requires a frequency between 50KHz to 200KHz
	// this frequency is set by the prescalar and the cpu_f
	// ADC frequency = cpu_f / prescalar
	// 16mhz / 128 = 150khz
	// this is well explaind at:
	// http://www.avrbeginners.net/architecture/adc/adc.html#adcsr

	switch(p){
		case PRESCALAR_2:
			ADCSRA &= ~(1<<ADPS0);
			ADCSRA &= ~(1<<ADPS1);
			ADCSRA &= ~(1<<ADPS2);
			break;
		case PRESCALAR_4:
			ADCSRA |= (1<<ADPS0);
			ADCSRA &= ~(1<<ADPS1);
			ADCSRA &= ~(1<<ADPS2);
			break;
		case PRESCALAR_8:
			ADCSRA &= ~(1<<ADPS0);
			ADCSRA |= (1<<ADPS1);
			ADCSRA &= ~(1<<ADPS2);
			break;
		case PRESCALAR_16:
			ADCSRA |= (1<<ADPS0);
			ADCSRA |= (1<<ADPS1);
			ADCSRA &= ~(1<<ADPS2);
			break;
		case PRESCALAR_32:
			ADCSRA &= ~(1<<ADPS0);
			ADCSRA &= ~(1<<ADPS1);
			ADCSRA |= (1<<ADPS2);
			break;
		case PRESCALAR_64:
			ADCSRA &= ~(1<<ADPS0);
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

void adc_setChannel(uint8_t ch){
	// set the channel between 0 and 7 (including)
	//ADMUX |= (ch & 0x0F); // maybe change |= to =
	ch &= 0x07; // 
	ADMUX = (ADMUX & 0xF8)|ch;
}

void adc_setVref(enum adc_vref_t vref){
	switch(vref){
		case AREF:
			ADMUX &= ~(1<<REFS0); 
			ADMUX &= ~(1<<REFS1);
			break;

		case AVCC:
			ADMUX |= (1<<REFS0); 
			ADMUX &= ~(1<<REFS1);
			break;

		case INTERNAL:
			ADMUX |= (1<<REFS0); 
			ADMUX |= (1<<REFS1);
			break;
	}
}


uint16_t adc_readChannel(uint8_t ch){
	adc_setChannel(ch);

	//Start Single conversion
   ADCSRA|=(1<<ADSC);

   //Wait for conversion to complete
   while(!(ADCSRA & (1<<ADIF)));

   //Clear ADIF by writing one to it
   ADCSRA|=(1<<ADIF);

   return(ADC);
}

