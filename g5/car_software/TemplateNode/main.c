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
#include <io.h>

void rx_complete(uint8_t mob);
void tx_complete(uint8_t mob);
void can_default(uint8_t mob);

int main(void)
{
	set_canit_callback(CANIT_RX_COMPLETED, rx_complete);
	set_canit_callback(CANIT_TX_COMPLETED, tx_complete);
	set_canit_callback(CANIT_DEFAULT, can_default);

	//Initialise the Gear node
	ioinit();									//Port setup
	uart1_init();					//Serial communication
	CAN_INIT_RX();								//Can setup
    pwm16Init2();								//Setup PWM controller
	counter0Init();								//Init interrupt counter to overflow with 168Hz

	sei();										//Enable interrupt

	adc_setChannel(1);
	adc_setVref(INTERNAL);
	ADC_ENABLE();
	adc_setPrescaler(ADC_PRESCALAR_128);

	uart1_txstring("\r\n\r\n\r\nSTARTING \r\n");
	setup_mob_rx(8, 4, 7);

	int i=0;
	while(1){
		// Main work loop
		_delay_ms(250);

		uint8_t msg[8] = {'H', 'E', 'Y', ' ', 'J', 'O', 'E'};

		setup_mob_tx(10, 4, &msg[0], 7);

		uint16_t res = adc_readChannel(i);
		uart1_printf("ADC channel %d = %d \r\n", i, res);	

		if(++i == 8){
			i = 0;
			uart1_txstring("\r\n");
		}
	}

    return 0;
}

void rx_complete(uint8_t mob) {
	uint8_t msg_buff[NB_DATA_MAX + 1] = {0};
	can_get_data(&msg_buff[0]);	// Copy data to canDataTest
	uart_printf(UART_NUMBER_1, "MOB_%d msg:%s\r\n", mob, msg_buff);
	Can_clear_status_mob();		// and reset MOb status
	BIT_SET(CANCDMOB, CONMOB1);	/* enable reception */
}

void tx_complete(uint8_t mob) {
	Can_mob_abort();		// Freed the MOB
	Can_clear_status_mob();	// and reset MOb status	
	Can_unset_mob_int(mob);	// Unset interrupt
}

void can_default(uint8_t mob) {
	Can_clear_status_mob(); // and reset MOb status
}
