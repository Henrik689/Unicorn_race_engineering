/**
* @file adc.h
* @brief
*	Used for setting up and reading from the ADC
*/

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/**
* @brief
*	ADC frequency = cpu_f / prescalar \n
*	150lHz = 16mHz / 128
*/
enum adc_prescalar_t {
	ADC_PRESCALAR_2 = 2,
	ADC_PRESCALAR_4 = 4,
	ADC_PRESCALAR_8 = 8,
	ADC_PRESCALAR_16 = 16,
	ADC_PRESCALAR_32 = 32,
	ADC_PRESCALAR_64 = 64,
	ADC_PRESCALAR_128 = 128
};

/**
* @brief
*	Different types of reference
*	sources for the ADC
*/
enum adc_vref_t {
	AREF, 		//!< Supply out own ref across AREF and GND
	AVCC, 		//!< Use vcc as ref
	INTERNAL 	//!< use the internal ref
};

/**
* @brief
*	Different types of trigger
*	sources availble to the ADC
*
* @todo
*	Write more detailed infomation
*	about these different trigger
*	types
*/
enum adc_triggerSource_t {
	FREE_RUNNING = 0,
	ANALOG_COMPARE = 1,
	EXTERNAL_INTERUPT_REQUEST = 2,
	COUNTER_0_COMPARE_MATCH = 3,
	COUNTER_0_OVERFLOW = 4,
	COUNTER_COMPARE_MATCH_B = 5,
	COUNTER_1_OVERFLOW = 6,
	COUNTER_1_CAPTURE_EVENT = 7
};


float adc_toVolt(uint16_t ADCReading);

void adc_enable(void);
void adc_disable(void);
void adc_autoTriggerEnable(void);
void adc_autoTriggerDisable(void);
void adc_InteruptEnable(void);
void adc_InteruptDisable(void);

void adc_setTriggerSource(enum adc_triggerSource_t source);
void adc_setPrescaler(const enum adc_prescalar_t p);
void adc_setChannel(const uint8_t ch);
void adc_setVref(const enum adc_vref_t vref);
uint16_t adc_read(void);
uint16_t adc_readChannel(uint8_t ch);
void adc_init(void);

#endif /* ADC_H */
	