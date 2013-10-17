#ifndef ADC_H
#define ADC_H

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

void adc_setPrescaler(enum adc_prescalar_t p);
void adc_setChannel(unsigned int ch);

#endif /* ADC_H */
