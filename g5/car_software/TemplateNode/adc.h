#ifndef ADC_H
#define ADC_H

#include <stdint.h>

enum adc_prescalar_t {
	PRESCALAR_2 = 2,
	PRESCALAR_4 = 4,
	PRESCALAR_8 = 8,
	PRESCALAR_16 = 16,
	PRESCALAR_32 = 32,
	PRESCALAR_64 = 64,
	PRESCALAR_128 = 128
};

enum adc_vref_t {
	AREF, // Supply out own ref across AREF and GND
	AVCC, // Use vcc as ref
	INTERNAL // use the internal ref
};

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

void adc_enable(void);
void adc_disable(void);
void adc_autoTriggerEnable(void);
void adc_autoTriggerDisable(void);
void adc_InteruptEnable(void);
void adc_InteruptDisable(void);
void adc_setTriggerSource(enum adc_triggerSource_t source);

void adc_setPrescaler(enum adc_prescalar_t p);
void adc_setChannel(uint8_t ch);
void adc_setVref(enum adc_vref_t vref);
uint16_t adc_read(void);
uint16_t adc_readChannel(uint8_t ch);
void adc_init(void);

#endif /* ADC_H */
	