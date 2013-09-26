#include "../config.h"
#include <util/delay.h>
#include "../can_std/can_lib.h"
#include "../can_func.h"
#include "../../lib/can_defs.h"
#include "../../lib/data_def.h"
#include "adc.h"

ADCReadObject ADCReadObjects[] = {	{ADC1, 2000, 1003, 0},
					{ADC5, 100, 2000, 0},
					{ADC6, 1000, 2500, 0},
					{ADC7, 1000, 2550, 0}
				};	

uint8_t data[8];

/* Used together with ADCReadObjectBuff */
volatile uint8_t ADCReadObjectBuffSize = 0;
volatile uint8_t ADCReadObjectBuffHead = 0;

/* Timer1 overflow interrupt used for adc read function */
ISR (TIMER1_OVF_vect)
{
	uint8_t i;
	ADCReadObject *P1;
	/* test vars */
/*	uint8_t data_buf[8];*/
/*	data_buf[0] = 1;*/
/*	data_buf[1] = 120;*/
	/* test vars stop */

	ADC_TIMER_TOID;		/* Disable timer interrupt */ 
	
	/* Count timeout for all items in queue down by timeout for item 
	 * first i queue */
	CountDown();

	/* NEW CODE */
	/* Start conversion for items with timeout at zero */
	P1 = QH;
	ADCReadObjectBuffSize = 0;
	while (P1->timeout == 0 && ADCReadObjectBuffSize != (NumOfSensors-1)){	/* for alle Objects with timeout == 0 */
		ADCReadObjectBuff[ADCReadObjectBuffSize] = P1;
		ADCReadObjectBuffSize++;
		P1 = P1->next;
	}
	
	MakeADCReadObjectConversion(); /* Start conversion. Next objects
					* will be called from ADC conversion 
   					* done interrupt */
	/* NEW CODE END*/

	
	/* Remove items with timeout at zero or less and put them back into
	 * queue at the correct place */
	while(QH->timeout == 0){
		ReloadQueue();
	}
	
	/* test printing */
/*	data_buf[0] = QH->adc;*/
/*	data_buf[1] = QH->interval/20;*/
/*	data_buf[2] = QH->timeout/20;		*/
	/* test printing stop */

	/* Load timer with value of timeout for the first item in the queue */
	SetTimer(QH->timeout);

	ADC_TIMER_TOIE;
}

/* ADC conversion done interrupt */
ISR (ADC_vect)
{
	uint8_t data_buf[8];
	uint8_t adc_L;
	/* TODO
	 * Koden herunder er en test af at læse adc'en og smide dataen ud på canbussen.
	 * Ser ud til at det virker fint nok så der skal bare lige tænkes over hvordan ADC
	 * læsningerne skal kobles sammen med data id'erne.
	*/
/*	if ((ADMUX & 0x0f) == ADC0) {
		data_buf[0] = 43;
		data_buf[1] = ADCL;
		data_buf[2] = ADCH;
		can_send_non_blocking(rpm_msgid, data_buf, 4);
	}*/
	
	switch (ADMUX & 0x0f) {
		case ADC0:
			data[0] = N6ADC0;
			break;
		case ADC1:
			data_buf[0] = N6ADC1;
			break;
		case ADC2:
			data_buf[0] = N6ADC2;
			break;
		case ADC3:
			data_buf[0] = N6ADC3;
			break;
		case ADC4:
			data_buf[0] = N6ADC4;
			break;
		case ADC5:
			data_buf[0] = N6ADC5;
			break;
		case ADC6:
			data_buf[0] = N6ADC6;
			break;
		case ADC7:
			data_buf[0] = N6ADC7;
			break;
		defalut:
			break;
	}

	adc_L = ADCL;
	data_buf[1] = ADCH;
	data_buf[2] = adc_L;

	can_send_non_blocking(rpm_msgid, data_buf, 4);

	/* NEW CODE */
	/* After date is saved for last conversion start new conversion if
	 * there is anymore ADCReadObjects to make conversion for */
	MakeADCReadObjectConversion();  
	/* NEW CODE END */
}

/* NEW CODE */
void InitADC(void)
{
	SET_ADC_PRESCALER;	/* Set prescaler */
	SET_ADC_REF;		/* Set ADC conversion reference */
	EN_ADC;			/* Enable ADC */
	ADC_IE;			/* Enable ADC conversion done interrupt */
}

void MakeADCReadObjectConversion(void)
{
	if (ADCReadObjectBuffSize == 0){/* If no more Objects reset Head */ 
		ADCReadObjectBuffHead = 0;
	}else{
		/* Select ADC channel for conversion */
		Set_adc_mux( ADCReadObjectBuff[ADCReadObjectBuffHead]->adc );
		ADCReadObjectBuffHead++;
		ADCReadObjectBuffSize--;
		ADC_START_CONV;		/* Start conversion */
	}
}
/* NEW CODE END*/


void AdcReadStart(void)
{
	int i;
	ADCReadObject *P1;	

	InitADC();	/* Initialize ADC in accordance with definitions made in 
			 * adc.h */

	SET_ADC_TIMER_PRESCALER;	/* Start timer 1 but no interrup yet */

	/* Link ADCReadObjects */
	QH = &ADCReadObjects[0];
	P1 = QH;
	for (i=1; i<NumOfSensors; i++){
		P1->next = &ADCReadObjects[i];
		P1 = P1->next;
	}
	/* Load timer with interval for object first in list */ 
	SetTimer(100);	/* timer interrupt funktion starter efter 100 ms */
	ADC_TIMER_TOIE;	/* Enable interrupt for timer */
}

void SetTimer(uint16_t val)
{
	int ticks;
	
	ticks = val*TICKS_PER_MS;	/* Calculate number of ticks
					 * before timer interrupt */
	ticks = ADC_TIMER_MAX_VAL-ticks;	/* Calculate value for timer */
	TCNT1 = ticks;				/* Load timer with value */	
}

int ReloadQueue(void)
{
	ADCReadObject *P1, *P2, *P3;
	int i=0;
	
	/* If there only is one sensor it's a special case */
	if (NumOfSensors <= 1){	
		return 1;	
	}

	P3=QH;			/* Pointer to object first in queue that we 
				 * want to remove and put into the queue again 
				 * after reload of timeout */
	P3->timeout = P3->interval; /* Reload P3's timeout */	
	QH=P3->next;		/* New head is object right after P3 */
	
	P1=QH;
	P2=QH->next;

	if (P1->timeout > P3->timeout){		/* P3 new head ? */	
		P3->next=P1;
		QH=P3;
	}else{		/* If not smallest timeout find place in queue */
		/* Can be used as long as we are in the middle og the queue */
		while (P2->timeout < P3->timeout && (NumOfSensors-3) != i){
			i++;
			P1=P2;
			P2=P2->next;
		}
		/* Test if we stoped the loop because we reached the end 
		 * and if this is the case test on what side of P2 P3 belong */ 
		if (i == (NumOfSensors-3) && P2->timeout < P3->timeout){
			P2->next=P3;
		}else{
			P3->next=P2;
			P1->next=P3;
		}
	}
	return 1;
}

void CountDown(void)
{
	uint8_t i;
	int tmpTimeout;		
	
	tmpTimeout = QH->timeout;
	for (i=0; i<NumOfSensors; i++){
		ADCReadObjects[i].timeout -= tmpTimeout;
	}
}
