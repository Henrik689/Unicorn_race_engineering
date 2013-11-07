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

char str[32] = {};

void canTestReceiver(void){
	uint8_t response_buffer[5] = {};
	st_cmd_t response_msg = {
		.pt_data = &response_buffer[0],
		.status = 0,

		.id.std = 4,
		.ctrl.ide = 0,
		.ctrl.rtr = 0,
		.dlc = 5,
		.cmd = CMD_RX_DATA_MASKED // CMD_RX_DATA_MASKED gives interrupt while CMD_RX_DATA does not
	};
	int cnt = 0;
	while(can_cmd(&response_msg) != CAN_CMD_ACCEPTED){cnt++;} // Rx cmd
	sprintf(str, "can_cmd counted to %d\r\n", cnt);
	uart_txstring(UART_NUMBER_1, str);

	//_delay_ms(50); // Wait 50 ms and if the msg has not completed abort
	cnt=0;
	while( can_get_status(&response_msg) != CAN_STATUS_COMPLETED){cnt++;}
	//if(can_get_status(&response_msg) == CAN_STATUS_COMPLETED){
		// print the received msg
		uart_txstring(UART_NUMBER_1, "CAN rev: ");
		int i;
		for(i=0; i < 5; i++){
			uart_txchar(UART_NUMBER_1, response_buffer[i]);
		}
		uart_txchar(UART_NUMBER_1, '\r');
		uart_txchar(UART_NUMBER_1, '\n');
	//}else{
	//	uart_txstring(UART_NUMBER_1, "CAN ERROR: ABORTING\r\n");

	//	response_msg.cmd = CMD_ABORT;
	//	while(can_cmd(&response_msg) != CAN_CMD_ACCEPTED);
	//}

	sprintf(str, "can_get_status counted to %d\r\n", cnt);
	uart_txstring(UART_NUMBER_1, str);

}

void canTestSender(void){
	const uint8_t sender_id = 4;
	uint8_t databuffer[5] = {'H', 'e', 'l', 'l', 'o'}; 

	st_cmd_t msg = {
		.pt_data = &databuffer[0],
		.ctrl.ide = 0,
		.dlc = 5,
		.id.std = sender_id,
		.cmd = CMD_TX_DATA
	};

	while(can_cmd(&msg) != CAN_CMD_ACCEPTED);
	while(can_get_status(&msg) == CAN_STATUS_NOT_COMPLETED);
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
	//Can_set_tx_int();
	Can_set_rx_int();

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
		//_delay_ms(250);

		//canTestSender();
		canTestReceiver();

		char buff[32] = {};

		//uint16_t res = adc_readChannel(i);

		//sprintf(buff, "ADC channel %d = %d \r\n", i, res);

		
		uart_txstring(UART_NUMBER_1, buff);
		
		if(++i == 8){
			i = 0;
			//uart_txstring(UART_NUMBER_1, "\r\n");
		}

	}

    return 0;
   
}
