/**
* @file adc.c
* @brief 
*	Used for setting up and reading from the ADC
*/

#include <stdint.h>
#include <avr/io.h>
#include "adc.h"
#include "bitwise.h"

/**
* @brief
*	Converts a digital ADC reading
*	the the same value in Volts
*
* @param[in] ADCReading
*	The digital ADC reading that
*	should be converted.
*
* @return
*	The ADC reading converted
*	to Volts
*/
float adc_toVolt(uint16_t ADCReading){
	//const float maxAdcBits = 1023.0f; // Using Float for clarity
	//const float maxVolts = 5.0f;      // Using Float for clarity
	//const float voltsPerBit = (maxVolts / maxAdcBits);

	//float voltage = ADCReading * ADC_VOLTS_PER_BIT; //voltsPerBit;
	return ADC_TO_VOLT(ADCReading);
}


/**
* @brief
*	Set the trigger for the ADC Interrupt
*
* @param[in] source
*	The trigger source that should trigger
*	the ADC ISR
*/
void adc_setTriggerSource(enum adc_triggerSource_t source){
	const uint16_t three_heighest_bits = (0x07 << 5); 
	const uint16_t s = (source << 5); // shift the source up to match the mask

	BITMASK_CLEAR(ADCSRB, three_heighest_bits);
	BITMASK_SET(ADCSRB, BITMASK_CHECK(s, three_heighest_bits));
}

/**
* @brief
*	Sets the ADC prescalar
*
* @details
*	The ADC requires a frequency between 50KHz to 200KHz
*	this frequency is set as the relation between the 
*	ADC prescalar and the cpu_f.
*	ADC frequency = cpu_f / prescalar
*	For example if we have: \n
*	cpu_f = 16mHz and set the prescalar to 128
* 	we get 16mHz / 128 = 150kHz. \n
*	A higher ADC frequency yields a faster
*	conversion rate but lower precision and
* 	vice versa.
*
* @param[in] p
*	The ADC prescalar value that should
*	define the ADC frequency.
*/
void adc_setPrescaler(const enum adc_prescalar_t p){
	// this is well explained at:
	// http://www.avrbeginners.net/architecture/adc/adc.html#adcsr

	// TODO: use BITMASK_CLEAR and BITMAST_SET to do this

	switch(p){
		case ADC_PRESCALAR_2:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_CLEAR(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case ADC_PRESCALAR_4:
			BIT_SET(ADCSRA, ADPS0);
			BIT_CLEAR(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case ADC_PRESCALAR_8:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case ADC_PRESCALAR_16:
			BIT_SET(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_CLEAR(ADCSRA, ADPS2);
			break;
		case ADC_PRESCALAR_32:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_CLEAR(ADCSRA, ADPS1);
			BIT_SET(ADCSRA, ADPS2);
			break;
		case ADC_PRESCALAR_64:
			BIT_CLEAR(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_SET(ADCSRA, ADPS2);
			break;
		case ADC_PRESCALAR_128:
			BIT_SET(ADCSRA, ADPS0);
			BIT_SET(ADCSRA, ADPS1);
			BIT_SET(ADCSRA, ADPS2);
			break;
	}
}

/**
* @brief
*	Set the ADC to a specific channel. \n
*	Channels available are normally 0-7 including.
*
* @param[in] ch
*	The channel that the ADC will be set to.
*	This must be a integer value ranging from 0 to 7
*/
void adc_setChannel(uint8_t ch){
	// set the channel between 0 and 7 (including)

	// We check if the channel value
	// is between 0 and 7 (represented with the three lowest bits).
	// We then set the channel value in the ADMUX register.
	const unsigned int three_lowest_bits = 0x07; 

	BITMASK_CLEAR(ADMUX, three_lowest_bits);
	BITMASK_SET(ADMUX, BITMASK_CHECK(ch, three_lowest_bits));
}

/**
* @brief
*	Set the reference voltage used
*	by the ADC
*
* @param[in] vref
*	The reference voltage that the
*	ADC should be using.
*/
void adc_setVref(const enum adc_vref_t vref){
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

/**
* @brief
*	Reads from the ADC on the current channel
*
* @note
*	A channel must be set with adc_setChannel()
*	before calling
*
* @return
*	The digital value read from the ADC
*/
uint16_t adc_read(void){
	//Start Single conversion
	ADC_START_CONV();

	//Wait for conversion to complete
	ADC_WAIT_FOR_CONV();

	//Clear ADIF by writing one to it
	BIT_SET(ADCSRA, ADIF);

	// Alternatively ADCH and ADCL
	// hold the high and low bit
	// values of the ADC
	return(ADC);
}

/**
* @brief
*	Same as adc_read() but sets
*	to the specified channel
*	before reading.
*
* @see adc_read()
*
* @param[in] ch
*	The channe the ADC will
*	be set to before reading.
*
* @return
*	The digital value read from the ADC
*/
uint16_t adc_readChannel(const uint8_t ch){
	adc_setChannel(ch);
	return(adc_read());
}

/**
* @brief
*	Sets up the ADC on channel using
*	The specified channel, vref and prescalar.
*	This must becalled before using the ADC.
*
* @param[in] channel
*	The channel ADC is set to.
*
* @param[in] vref
*	The reference voltage that ADC is set to.
*
* @param[in] prescalar
*	The prescalar value used to set the ADC
*	frequency.
*
* @note
*	If you want to setup the ADC to use a trigger source
*	for interrupt usage these setup calls have to run
*	manually.
*/ 
void adc_init(int channel, enum adc_vref_t vref, enum adc_prescalar_t prescalar){
	adc_setChannel(channel);
	adc_setVref(vref);
	adc_setPrescaler(prescalar);
	ADC_ENABLE();
}

