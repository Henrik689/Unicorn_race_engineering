#include <stdint.h>
#include <stdio.h>

#include <config.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <can.h>
#include <uart.h>
#include <adc.h>

#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "init.h"



int main(void)
{
	//Initialise the Gear node
	ioinit();									//Port setup
	uart1_init();					//Serial communication
	CAN_INIT_RX();								//Can setup
    pwm16Init2();								//Setup PWM controller
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	sei();										//Enable interrupt

	adc_setChannel(1);
	adc_setVref(INTERNAL);
	adc_enable();
	adc_setPrescaler(ADC_PRESCALAR_128);

	uart1_txstring("\r\n\r\n\r\nSTARTING \r\n");
	can_testReceiver();
	int i=0;
	while(1){
		// Main work loop
		_delay_ms(250);

		//can_testSender();
		//can_testReceiver();

		uint16_t res = adc_readChannel(i);
		uart1_printf("ADC channel %d = %d \r\n", i, res);	

		if(++i == 8){
			i = 0;
			uart1_txstring("\r\n");
		}

	}

    return 0;
   
}
