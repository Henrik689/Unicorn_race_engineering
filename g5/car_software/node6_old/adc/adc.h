#include "../integer.h"

#ifndef _ADC
#define _ADC 1

extern uint8_t data[8];

/* Used for ADC settings */
#define ADC_PRESCALER 		(1<<ADPS2 | 1<<ADPS1)/* Divide F_CPU with 64 */
#define SET_ADC_PRESCALER	ADCSRA |= ADC_PRESCALER	/* Set ADC prescaler */
#define ADC_REF			(1<<REFS0)	/* AVcc */
#define SET_ADC_REF		ADMUX |= ADC_REF	/* Set ADC reference */
#define ADC_MUX_MASK		0xE0			/* Mux mask */
#define Set_adc_mux(ADC)	{ ADMUX &= ADC_MUX_MASK; ADMUX |= ADC; }	
#define EN_ADC			ADCSRA |= 1<<ADEN	/* Enable ADC */
#define DIS_ADC			ADCSRA &= ~(1<<ADEN)	/* Disable ADC */
#define ADC_START_CONV		ADCSRA |= 1<<ADSC	/* Start conversion */
#define ADC_IE			ADCSRA |= 1<<ADIE	/* ADC interrupt */
#define ADC_ID			ADCSRA &= ~(1<<ADIE)	/* ADC interrupt 
							 * disable */

/* Used for timer */
#define ADC_TIMER_RES 		16		/* Timer is 16 bit resolution */
#define ADC_TIMER_MAX_VAL	65536		/* 2^16 */
#define ADC_TIMER_PRESCALER 	5		/* 1024 */
#define TICKS_PER_MS 		10.8		/* (F_CPU/prescaler)*10^-3 */
#define PRESCALER_MASK 		0xF8

#define SET_ADC_TIMER_PRESCALER		TCCR1B |= ADC_TIMER_PRESCALER
#define ADC_TIMER_TOIE			TIMSK1 |= 1<<TOIE1			
#define ADC_TIMER_TOID			TIMSK1 &= ~(1<<TOIE1)

typedef struct ADCReadObject_{
	uint8_t adc;		/* ADC input pin (0-7) */
	int interval;		/* Sampling rate = 1/(interval*10^-3) */
	int timeout;
	struct ADCReadObject_* next;
}ADCReadObject;

ADCReadObject *QH, *QT;		/* Pointers to queue head and tail */

/* ADC input numbers */
enum ADCInput {
	ADC0,
	ADC1,
	ADC2,
	ADC3,
	ADC4,
	ADC5,
	ADC6,
	ADC7
};

enum Sensor {
	Temp1,
	Temp2,
	Temp3,
	NumOfSensors
};

/* Buffer to hold ADCReadObjects that the ADC should make conversions for */ 
ADCReadObject* ADCReadObjectBuff[NumOfSensors];

/* Used together with ADCReadObjectBuff */
extern volatile uint8_t ADCReadObjectBuffSize;
extern volatile uint8_t ADCReadObjectBuffHead;


/* Function to call when ADCReadObject that are ready for conversion has
 * been put in ADCReadObjectBuff */
void MakeADCReadObjectConversion(void);

/* Function to init list and start reading values from ADC */
void AdcReadStart(void);

/* This function take the first item in the queue and reload it's timeout with
 * interval and put it into the queue again at the right place */
int ReloadQueue(void);

/* Function to set timer value based on wanted time (in ms) to next timer interrupt */ 
void SetTimer(uint16_t TimeToInt);

/* Function to count down timeout for all items in queue by QH->timeout */
void CountDown(void);

#endif // _ADC
