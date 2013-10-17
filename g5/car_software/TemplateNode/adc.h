#ifndef ADC_H
#define ADC_H

enum prescalar_t {
	PRESCALAR_2 = 2,
	PRESCALAR_4 = 4,
	PRESCALAR_8 = 8,
	PRESCALAR_16 = 16,
	PRESCALAR_32 = 32,
	PRESCALAR_64 = 64,
	PRESCALAR_128 = 128
};

void adc_setPrescaler(enum prescalar_t p);
void adc_setChannel(unsigned int ch);

#endif /* ADC_H */
