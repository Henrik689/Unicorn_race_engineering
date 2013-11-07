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

void canTestReceiver(void){
	uint8_t rx_remote_buffer[5] = {};
	st_cmd_t tx_remote_msg;

	uint8_t response_buffer[5] = {};
	st_cmd_t response_msg;

	tx_remote_msg.pt_data = &rx_remote_buffer[0];
	tx_remote_msg.status = 0;

	response_msg.pt_data = &response_buffer[0];
	response_msg.status = 0;

	// ------ Done with init

	// Do the Rx
	response_msg.id.std = 4; // sender_id
	response_msg.ctrl.ide = 0;
	response_msg.ctrl.rtr = 0;
	response_msg.dlc = 5;
	response_msg.cmd = CMD_RX_DATA_MASKED;
	while(can_cmd(&response_msg) != CAN_CMD_ACCEPTED); // Rx cmd


	// Do the Tx
	tx_remote_msg.id.std = 4; // sender_id
	tx_remote_msg.ctrl.ide = 0;
	tx_remote_msg.ctrl.rtr = 1;
	tx_remote_msg.dlc = 5;
	tx_remote_msg.cmd = CMD_TX_REMOTE;
	while(can_cmd(&tx_remote_msg) != CAN_CMD_ACCEPTED); // tx cmd
	while(can_get_status(&tx_remote_msg) == CAN_STATUS_NOT_COMPLETED); // wait for tx to complete


	_delay_ms(50); // Wait 50 ms and if the msg has not completed abort
	if(can_get_status(&response_msg) == CAN_STATUS_COMPLETED){
		// print the received msg
		uart_txstring(UART_NUMBER_1, "CAN rev: ");
		int i;
		for(i=0; i < 5; i++){
			uart_txchar(UART_NUMBER_1, response_buffer[i]);
		}
		uart_txchar(UART_NUMBER_1, '\r');
		uart_txchar(UART_NUMBER_1, '\n');
	}else{
		uart_txstring(UART_NUMBER_1, "CAN ERROR: ABORTING\r\n");

		response_msg.cmd = CMD_ABORT;
		while(can_cmd(&response_msg) != CAN_CMD_ACCEPTED);
	}

}

void canTestSender(void){
	const uint8_t sender_id = 4;
	uint8_t databuffer[5] = {'H', 'e', 'l', 'l', 'o'}; 

	st_cmd_t sensor_message;

	sensor_message.pt_data = &databuffer[0];

	sensor_message.ctrl.ide = 0;            //- CAN 2.0A
	sensor_message.dlc = 5; // size of databuffer
	sensor_message.id.std = sender_id;
	sensor_message.cmd = CMD_REPLY_MASKED;

	while(can_cmd(&sensor_message) != CAN_CMD_ACCEPTED);
	while(can_get_status(&sensor_message) == CAN_STATUS_NOT_COMPLETED);
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
	Can_set_tx_int();
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
		_delay_ms(250);

		//testCanTX();
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
