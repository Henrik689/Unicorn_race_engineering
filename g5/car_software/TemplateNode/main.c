#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#include "can_std/can_lib.h"
#include "can_comm.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "init.h"
#include "uart.h"
#include "adc.h"

#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(arr[0]))

void testCanTX(void){
	uint8_t databuff[5] = {'H', 'e', 'l', 'l', 'o'}; 
	uint8_t id = 141;

	can_send_non_blocking(id, &databuff, SIZEOF_ARR(databuff));
}

int main(void)
{
	//Initialise the Gear node
	ioinit();									//Port setup
	uart_init(UART_NUMBER_1);								//Serial communication
	can_init(0);								//Can setup
    pwm16Init2();								//Setup PWM controller
	//adcInit(1);									//Setup ADC for pot-meter or Amp meter
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	sei();										//Enable interrupt

	//Enable all interrupts
	Can_sei();									//Enable all interrupts

	adc_setChannel(1);
	adc_setVref(INTERNAL);
	adc_enable();
	adc_setPrescaler(PRESCALAR_128);

	uart_txstring(UART_NUMBER_1, "\r\n");
	uart_txstring(UART_NUMBER_1, "\r\n");
	uart_txstring(UART_NUMBER_1, "\r\n");

	uart_txstring(UART_NUMBER_1, "STARTING \r\n");

	int i=0;
	while(1){
		// Main work loop
		_delay_ms(250);

		testCanTX();

		char buff[32] = {};

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
