#include <avr/interrupt.h>
#include <stdint.h>
#include "adc.h"
#include "bitwise.h"

void adc_enable(void){
	BIT_SET(ADCSRA, ADEN);
}

void adc_disable(void){
	BIT_CLEAR(ADCSRA, ADEN);
}

void adc_autoTriggerEnable(void){
	BIT_SET(ADCSRA, ADATE);
}

void adc_autoTriggerDisable(void){
	BIT_CLEAR(ADCSRA, ADATE);
}

void adc_InteruptEnable(void){
	BIT_SET(ADCSRA, ADIE);
}

void adc_InteruptDisable(void){
	BIT_CLEAR(ADCSRA, ADIE);
}

void adc_setTriggerSource(enum adc_triggerSource_t source){
	BITMASK_CLEAR(ADCSRB, (0x07<<5));
	BITMASK_SET(ADCSRB, (source<<5));
}

void adc_setPrescaler(enum adc_prescalar_t p){
	// The ADC requires a frequency between 50KHz to 200KHz
	// this frequency is set by the prescalar and the cpu_f
	// ADC frequency = cpu_f / prescalar
	// 16mhz / 128 = 150khz
	// this is well explaind at:
	// http://www.avrbeginners.net/architecture/adc/adc.html#adcsr

	// TODO: use BITMASK_CLEAR and BITMAST_SET to do this

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
	//BIT_CLEAR(ch, 0x07);
	//BIT_SET(ADMUX, ch);
	//ADMUX = (ch & 0x0f);

	//ch = ch & 0x07;
   	//ADMUX |= ch;
   	ADMUX |= BITMASK_CHECK(ch, 0x07);
}

void adc_setVref(enum adc_vref_t vref){
	switch(vref){
		case AREF:
			BIT_CLEAR(ADMUX, REFS0);
			BIT_CLEAR(ADMUX, REFS1);
			break;

		case AVCC:
			BIT_SET(ADMUX, REFS0);
			BIT_CLEAR(ADMUX, REFS1);
			break;

		case INTERNAL:
			BIT_SET(ADMUX, REFS0);
			BIT_SET(ADMUX, REFS1);
			break;
	}
}

uint16_t adc_read(void){
	//Start Single conversion
	BIT_SET(ADCSRA, ADSC);

   //Wait for conversion to complete
	while(!BIT_CHECK(ADCSRA, ADIF));

   //Clear ADIF by writing one to it
   BIT_SET(ADCSRA, ADIF);

   return(ADC);
   //return (ADCH << 8) | ADCL ;
}

uint16_t adc_readChannel(uint8_t ch){
	adc_setChannel(ch);
	return(adc_read());
}

void adc_init(void){
	adc_setChannel(1);
	adc_setVref(AVCC);
	adc_enable();
	adc_setPrescaler(PRESCALAR_128);
	adc_autoTriggerEnable();
	adc_setTriggerSource(ANALOG_COMPARE);
	adc_InteruptEnable();
}

