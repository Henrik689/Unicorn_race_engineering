
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "can.h"
#include "can_std/can_lib.h"
#include "uart.h"

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

/* Interrupt routine to take care of can interrupts */
ISR(CANIT_vect){
	uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n");
	uart_txstring(UART_NUMBER_1, "Got can interrupt \r\n");

	canTestReceiver();
}
