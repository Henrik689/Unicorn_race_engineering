
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
#define TEST_MSG_LEN	3 // length of the test message used for debugging

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


void mob_create(int mob_id, st_cmd_t* descriptor) {
	Can_set_mob(mob_id);

	Can_clear_mob();
	
	Can_set_std_id(descriptor->id.std)
	
	//uint32_t temp = UINT32_MAX;
	Can_set_ext_msk((uint32_t){UINT32_MAX});

	uart_printf(UART_NUMBER_1, "SETUP dlc = %d", descriptor->dlc);
	Can_set_dlc(descriptor->dlc);

	Can_set_rtrmsk();
	Can_clear_rtr();
	Can_set_idemsk();
	Can_config_rx();  
	Can_set_mob_int(mob_id);
}

void can_receive(int mob_id, int id, uint8_t *data, uint8_t length){
	st_cmd_t received_msg = {
		.pt_data = &data[0],
		.status = 0,

		.id.std = id,
		.ctrl.ide = 0,
		.ctrl.rtr = 0,
		.dlc = length,
		.cmd = CMD_RX_DATA_MASKED // CMD_RX_DATA_MASKED gives interrupt while CMD_RX_DATA does not
	};

	mob_create(mob_id, &received_msg);
	//CAN_FORCE_CMD(&received_msg);
	//can_cmd(&received_msg);
	//can_get_status(&received_msg);
	//CAN_FORCE_CMD(&received_msg);
	//CAN_FORCE_COMPLETE(&received_msg);
}

void can_testReceiver(void){
	uint8_t received_data[TEST_MSG_LEN] = {0};
	uint8_t received_data_other[6] = {0};
	can_receive(10, 5, &received_data_other[0], 6);
	can_receive(4, TEST_MSG_ID, &received_data[0], TEST_MSG_LEN);

	uart_txstring(UART_NUMBER_1, "CAN rev: ");
	uart_txarr(UART_NUMBER_1, &received_data[0], TEST_MSG_LEN);
	uart_txstring(UART_NUMBER_1, "\r\n");
}

void can_testSender(void){
	uint8_t databuffer[TEST_MSG_LEN] = {'H', 'E', 'L'/*, 'L', 'O'*/}; 
	can_send(TEST_MSG_ID, &databuffer[0], TEST_MSG_LEN);
}

/* Interrupt routine to take care of can interrupts */
char str[64] = {0};
uint8_t buffer[TEST_MSG_LEN] = {0};
int cnt = 0;
ISR(CANIT_vect){
	const uint16_t cansit = CANSIT2+(CANSIT1<<8);
	const uint8_t mob_back = CANPAGE;	// Save CANPAGE state
	uint16_t thisMOBpos = 0x01;

	// Loop over each MOB and check if it have pending interrupt
	int i;
	for(i = 0; i <= LAST_MOB_NB; i++){
		if(BITMASK_CHECK(cansit, thisMOBpos)){	/* True if mob have pending interrupt */
			Can_set_mob(i); /* Switch to mob */

			const uint8_t interrupt = BITMASK_CHECK(CANSTMOB, INT_MOB_MSK);
			switch (interrupt){
				case MOB_RX_COMPLETED:
					/* Can specific code */
					//can_receive(TEST_MSG_ID, &buffer[0], TEST_MSG_LEN);
					can_get_data(&buffer[0]);	// Copy data to canDataTest
					//Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					BIT_SET(CANCDMOB, CONMOB1);
					BIT_SET(CANCDMOB, DLC0);
					BIT_SET(CANCDMOB, DLC2);

					//can_receive(TEST_MSG_ID, &buffer[0], TEST_MSG_LEN); // put this before can_get_data and see if it works
					uart_txarr(UART_NUMBER_1, &buffer[0], TEST_MSG_LEN);

					uart_printf(UART_NUMBER_1, "  MOB_%d msg %d\r\n", i, cnt++);
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
		thisMOBpos = thisMOBpos << 1;
	}

	CANPAGE |= BITMASK_CHECK(mob_back, 0xF0); // Restore CANPAGE state
}
