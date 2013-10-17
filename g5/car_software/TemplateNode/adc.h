#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

/* x=target variable, y=mask */
#define BITMASK_SET(x,y) ((x) |= (y))
#define BITMASK_CLEAR(x,y) ((x) &= (~(y)))
#define BITMASK_FLIP(x,y) ((x) ^= (y))
#define BITMASK_CHECK(x,y) ((x) & (y))

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
	FREE_RUNNING,
	ANALOG_COMPARE,
	EXTERNAL_INTERUPT_REQUEST,
	COUNTER_0_COMPARE_MATCH,
	COUNTER_0_OVERFLOW,
	COUNTER_COMPARE_MATCH_B,
	COUNTER_1_OVERFLOW,
	COUNTER_1_CAPTURE_EVENT
}

void adc_setPrescaler(enum adc_prescalar_t p);
void adc_setChannel(uint8_t ch);
uint16_t adc_read(void);
uint16_t adc_readChannel(uint8_t ch);

#endif /* ADC_H */
	