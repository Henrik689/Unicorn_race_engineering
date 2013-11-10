
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "can_std/can_lib.h"
#include "can.h"
#include "uart.h"

#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(arr[0]))

#define TEST_MSG_ID		4
#define TEST_MSG_LEN	5 // length of the test message used for debugging

void can_send(int id, uint8_t *data, size_t length){
	st_cmd_t msg = {
		.pt_data = &data[0],
		.ctrl.ide = 0,
		.dlc = length,
		.id.std = id,
		.cmd = CMD_TX_DATA
	};

	CAN_FORCE_CMD(&msg);
	CAN_FORCE_COMPLETE(&msg);
}

void can_receive(int id, uint8_t *data, size_t length){
	st_cmd_t received_msg = {
		.pt_data = &data[0],
		.status = 0,

		.id.std = id,
		.ctrl.ide = 0,
		.ctrl.rtr = 0,
		.dlc = length,
		.cmd = CMD_RX_DATA_MASKED // CMD_RX_DATA_MASKED gives interrupt while CMD_RX_DATA does not
	};

	CAN_FORCE_CMD(&received_msg);
	CAN_FORCE_COMPLETE(&received_msg);
}

void canTestReceiver(void){
	uint8_t received_data[TEST_MSG_LEN] = {};
	st_cmd_t received_msg = {
		.pt_data = &received_data[0],
		.status = 0,

		.id.std = TEST_MSG_ID,
		.ctrl.ide = 0,
		.ctrl.rtr = 0,
		.dlc = TEST_MSG_LEN,
		.cmd = CMD_RX_DATA_MASKED // CMD_RX_DATA_MASKED gives interrupt while CMD_RX_DATA does not
	};
	CAN_FORCE_CMD(&received_msg); // Rx cmd
	CAN_FORCE_COMPLETE(&received_msg);

	uart_txstring(UART_NUMBER_1, "CAN rev: ");
	uart_txarr(UART_NUMBER_1, &received_data[0], TEST_MSG_LEN);
	uart_txstring(UART_NUMBER_1, "\r\n");
}

void canTestSender(void){
	uint8_t databuffer[TEST_MSG_LEN] = {'H', 'e', 'l', 'l', 'o'}; 

	st_cmd_t msg = {
		.pt_data = &databuffer[0],
		.ctrl.ide = 0,
		.dlc = TEST_MSG_LEN,
		.id.std = TEST_MSG_ID,
		.cmd = CMD_TX_DATA
	};

	CAN_FORCE_CMD(&msg);
	CAN_FORCE_COMPLETE(&msg);
}

/* Interrupt routine to take care of can interrupts */
ISR(CANIT_vect){
	uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n");
	uart_txstring(UART_NUMBER_1, "Got can interrupt \r\n");

	canTestReceiver();
}
