
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "can.h"
#include "can_std/can_lib.h"
#include "uart.h"

#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(arr[0]))

#define TEST_MSG_LEN	5 // length of the test message used for debugging

void canTestReceiver(void){
	uint8_t received_data[TEST_MSG_LEN] = {};
	st_cmd_t received_msg = {
		.pt_data = &received_data[0],
		.status = 0,

		.id.std = 4,
		.ctrl.ide = 0,
		.ctrl.rtr = 0,
		.dlc = TEST_MSG_LEN,
		.cmd = CMD_RX_DATA_MASKED // CMD_RX_DATA_MASKED gives interrupt while CMD_RX_DATA does not
	};
	while(can_cmd(&received_msg) != CAN_CMD_ACCEPTED); // Rx cmd

	//_delay_ms(50); // Wait 50 ms and if the msg has not completed abort
	while( can_get_status(&received_msg) != CAN_STATUS_COMPLETED);
	//if(can_get_status(&received_msg) == CAN_STATUS_COMPLETED){
		// print the received msg
		uart_txstring(UART_NUMBER_1, "CAN rev: ");
		int i;
		for(i=0; i < TEST_MSG_LEN; i++){
			uart_txchar(UART_NUMBER_1, received_data[i]);
		}
		uart_txchar(UART_NUMBER_1, '\r');
		uart_txchar(UART_NUMBER_1, '\n');
	//}else{
	//	uart_txstring(UART_NUMBER_1, "CAN ERROR: ABORTING\r\n");

	//	received_msg.cmd = CMD_ABORT;
	//	while(can_cmd(&received_msg) != CAN_CMD_ACCEPTED);
	//}

}

void canTestSender(void){
	const uint8_t sender_id = 4;
	uint8_t databuffer[TEST_MSG_LEN] = {'H', 'e', 'l', 'l', 'o'}; 

	st_cmd_t msg = {
		.pt_data = &databuffer[0],
		.ctrl.ide = 0,
		.dlc = TEST_MSG_LEN,
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
