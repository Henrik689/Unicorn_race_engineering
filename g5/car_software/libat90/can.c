
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "can_std/can_lib.h"
#include "can.h"
#include "uart.h"

#include <util/delay.h>

#define SIZEOF_ARR(arr) (sizeof(arr) / sizeof(arr[0]))

#define TEST_MSG_ID		4 // Debuggin msg id
#define TEST_MSG_LEN	5 // length of the test message used for debugging

void can_send(int id, uint8_t *data, uint8_t length){
	st_cmd_t msg = {
		.pt_data = &data[0],
		.id.std = id,

		.ctrl.ide = 0, // ide : identifier extension: 0 = can2.0A (11bit), 1 = can2.0B (29bit)
		//.ctrl.rtr = 0, // rtr : request remote transmission
		
		.dlc = length,
		
		.cmd = CMD_TX_DATA
	};

	CAN_FORCE_CMD(&msg);
	CAN_FORCE_COMPLETE(&msg);
}

void can_receive(int id, uint8_t *data, uint8_t length){
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
	//can_cmd(&received_msg);
	//can_get_status(&received_msg);
	//CAN_FORCE_CMD(&received_msg);
	CAN_FORCE_COMPLETE(&received_msg);
}

void can_testReceiver(void){
	uint8_t received_data[TEST_MSG_LEN] = {};
	can_receive(TEST_MSG_ID, &received_data[0], TEST_MSG_LEN);

	uart_txstring(UART_NUMBER_1, "CAN rev: ");
	uart_txarr(UART_NUMBER_1, &received_data[0], TEST_MSG_LEN);
	uart_txstring(UART_NUMBER_1, "\r\n");
}

void can_testSender(void){
	uint8_t databuffer[TEST_MSG_LEN] = {'H', 'E', 'L', 'L', 'O'}; 
	can_send(TEST_MSG_ID, &databuffer[0], TEST_MSG_LEN);
}

/* Interrupt routine to take care of can interrupts */
char str[64] = {};
uint8_t buffer[TEST_MSG_LEN] = {};
int cnt = 0;
ISR(CANIT_vect){
	/*
	uint8_t save_page = CANPAGE;
	volatile int test = CANSTMOB;
	static int nInterrupt = 0;
	//uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n");
	
	sprintf(str, "interrupt number %d ", nInterrupt++);
	uart_txstring(UART_NUMBER_1, str);

	sprintf(str, "CANSTMOB %d ", test);
	uart_txstring(UART_NUMBER_1, str);

	sprintf(str, "CANREC %d ", CANREC);
	uart_txstring(UART_NUMBER_1, str);

	sprintf(str, "CANGIT %d ", CANGIT);
	uart_txstring(UART_NUMBER_1, str);
	
	sprintf(str, "CANSIT1: %d CANSIT2: %d\r\n", CANSIT1, CANSIT2);
	uart_txstring(UART_NUMBER_1, str);

	//BIT_SET(CANSTMOB, RXOK);
	//uart_txstring(UART_NUMBER_1, "Got can interrupt \r\n");

	//can_testReceiver();

	CANPAGE = save_page; */


	uint8_t mob_back = CANPAGE;	// Save CANPAGE state
	int i = 0;
	uint8_t interrupt = 0;
	uint16_t tmp = CANSIT2+(CANSIT1<<8);
	uint16_t mask = 1;

	for(i=0;i<=14;i++){
		if(tmp & mask){	/* True if mob have pending interrupt */
			Can_set_mob(i); /* Switch to mob */
			interrupt = (CANSTMOB & INT_MOB_MSK);
			switch (interrupt){
				case MOB_RX_COMPLETED:
					/* Can specific code */
					//can_get_data(&buffer[0]);	// Copy data to canDataTest
					//can_receive(TEST_MSG_ID, &buffer[0], TEST_MSG_LEN);
					//Can_mob_abort();        // Freed the MOB
					//Can_clear_status_mob(); // and reset MOb status

					
					uart_txarr(UART_NUMBER_1, &buffer[0], TEST_MSG_LEN);

					sprintf(str, "msg %d", cnt++);
					uart_txstring(UART_NUMBER_1, str);
					uart_txstring(UART_NUMBER_1, "\r\n");
					//Can_unset_mob_int(i);

//					xbee_send_trigger();				/* TODO Der skal nok være den i main */
					break;
				case MOB_TX_COMPLETED:
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status	
					Can_unset_mob_int(i);	// Unset interrupt
					break;				
				case MOB_ACK_ERROR:
					/* TODO */
					break;
				case MOB_FORM_ERROR:
					/* TODO */
					break;
				case MOB_CRC_ERROR:
					/* TODO */
					break;
				case MOB_STUFF_ERROR:
					/* TODO */
					break;
				case MOB_BIT_ERROR:
					/* TODO */
					break;
				default:
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					break;
			}
		}
		mask = mask<<1;
	}
	//CANPAGE = mob_back;
	CANPAGE |= mob_back & 0xF0;	// Restore CANPAGE state


}
