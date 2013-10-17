#include <avr/interrupt.h>
#include <stdint.h>
#include "adc.h"
#include "bitwise.h"

void adc_enable(){
	BIT_SET(ADCSRA, ADEN);
}

void adc_disable(){
	BIT_CLEAR(ADCSRA, ADEN);
}

void adc_autoTriggerEnable(void){
	BIT_SET(ADCSRA, ADATE);
}

void adc_autoTriggerDisable(void){
	BIT_CLEAR(ADCSRA, ADATE);
}

adc_InteruptEnable(void){
	BIT_SET(ADCSRA, ADIE);
}

adc_InteruptDisable(void){
	BIT_CLEAR(ADCSRA, ADIE);
}

adc_setTriggerSource(enum adc_triggerSource_t source){
	switch(source){
		case FREE_RUNNING:
			ADCSRB &= ~(1<<ADTS0);
     		ADCSRB &= ~(1<<ADTS1);
     		ADCSRB &= ~(1<<ADTS2);
			break;
		case ANALOG_COMPARE:
			ADCSRB |= (1<<ADTS0);
     		ADCSRB &= ~(1<<ADTS1);
     		ADCSRB &= ~(1<<ADTS2);
			break;
		case EXTERNAL_INTERUPT_REQUEST:
			ADCSRB &= ~(1<<ADTS0);
     		ADCSRB |= (1<<ADTS1);
     		ADCSRB &= ~(1<<ADTS2);
			break;
		case COUNTER_0_COMPARE_MATCH:
			ADCSRB |= (1<<ADTS0);
     		ADCSRB |= (1<<ADTS1);
     		ADCSRB &= ~(1<<ADTS2);
			break;
		case COUNTER_0_OVERFLOW:
			ADCSRB &= ~(1<<ADTS0);
     		ADCSRB &= ~(1<<ADTS1);
     		ADCSRB |= (1<<ADTS2);
			break;
		case COUNTER_COMPARE_MATCH_B:
			ADCSRB &= ~(1<<ADTS0);
     		ADCSRB &= ~(1<<ADTS1);
     		ADCSRB |= (1<<ADTS2);
			break;
	}
}

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

uint16_t adc_read(void){
	//Start Single conversion
	BIT_SET(ADCSRA, ADSC);

   //Wait for conversion to complete
	while(!BIT_CHECK(ADCSRA, ADIF));

   //Clear ADIF by writing one to it
   BIT_SET(ADCSRA, ADIF);

   return(ADC);
}

uint16_t adc_readChannel(uint8_t ch){
	adc_setChannel(ch);
	return(adc_read());
}

void adc_init(void){
	adc_setChannel(1);
	adc_setVref(AVCC);
	adc_setPrescaler(PRESCALAR_128);
}

