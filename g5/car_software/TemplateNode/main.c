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
	uart_init(UART_NUMBER_1);					//Serial communication
	CAN_INIT_RX();								//Can setup
    pwm16Init2();								//Setup PWM controller
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	sei();										//Enable interrupt

	adc_setChannel(1);
	adc_setVref(INTERNAL);
	adc_enable();
	adc_setPrescaler(PRESCALAR_128);

	uart_txstring(UART_NUMBER_1, "\r\n\r\n\r\nSTARTING \r\n");
	setup_mob_rx(8, 4, 5);
	int i=0;
	while(1){
		// Main work loop
		_delay_ms(250);

		//can_testSender();
		//can_testReceiver();

		char buff[32] = {0};

		uint16_t res = adc_readChannel(i);

		sprintf(buff, "ADC channel %d = %d \r\n", i, res);

		
		uart_txstring(UART_NUMBER_1, buff);
		
		if(++i == 8){
			i = 0;
			uart_txstring(UART_NUMBER_1, "\r\n");
		}

	}

    return 0;
   
}
