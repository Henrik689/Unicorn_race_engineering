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
	const uint16_t three_heighest_bits = 0x07 << 5;
	source = source << 5; // shift the source up to match the mask
	BITMASK_CLEAR(ADCSRB, three_heighest_bits);
	BITMASK_SET(ADCSRB, source);
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
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_CLEAR(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case PRESCALAR_4:
			BIT_SET(ADCSRA, ADPS0);
			BIT_CLEAR(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case PRESCALAR_8:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case PRESCALAR_16:
			BIT_SET(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case PRESCALAR_32:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_CLEAR(ADCSRA, ADPS1);
			BIT_SET(ADCSRA, ADPS2);
			break;
		case PRESCALAR_64:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_SET(ADCSRA, ADPS2);
			break;
		case PRESCALAR_128:
			BIT_SET(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_SET(ADCSRA, ADPS2);
			break;
	}
}

void adc_setChannel(uint8_t ch){
	// set the channel between 0 and 7 (including)

	// We check if the channel value
	// is between 0 and 7 (represented with the three lowest bits).
	// We then set the channel value in the ADMUX register.
	const unsigned int three_lowest_bits = 0x07; 

	BITMASK_CLEAR(ADMUX, three_lowest_bits);
	BITMASK_SET(ADMUX, BITMASK_CHECK(ch, three_lowest_bits));
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

   // Alternatively ADCH and ADCL
   // hold the heigh and low bit
   // values of the ADC
   return(ADC);
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

