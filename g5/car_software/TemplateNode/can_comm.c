#include <stdint.h>

#include "config.h"
#include <avr/interrupt.h>

#include "can_std/can_lib.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "can_comm.h"

#include "uart.h"
#include "bitwise.h"

unsigned char canDataTest[8];
st_cmd_t tx_remote_msg;

void can_update_rx_msg(st_cmd_t* msg, uint8_t msg_id, uint8_t dlc){
        msg->id.std = msg_id;
        msg->ctrl.ide = 0;
        msg->ctrl.rtr = 0;
        msg->dlc = dlc;
        msg->cmd = CMD_RX_DATA_MASKED;

        while(can_cmd(msg) != CAN_CMD_ACCEPTED);
}

/* Interrupt routine to take care of can interrupts */
ISR(CANIT_vect){
	uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n"); uart_txstring(UART_NUMBER_1, "\r\n");
	uart_txstring(UART_NUMBER_1, "Got can interrupt \r\n");
/*
	if(BIT_CHECK(CANGIT, 0x07)){
		uart_txstring(UART_NUMBER_1, "General interrupt\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x06)){
		uart_txstring(UART_NUMBER_1, "Bus Off Interrupt Flag\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x05)){
		uart_txstring(UART_NUMBER_1, "Overrun CAN Timer\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x04)){
		uart_txstring(UART_NUMBER_1, "Frame Buffer Receive Interrupt\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x03)){
		uart_txstring(UART_NUMBER_1, "Stuff Error General\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x02)){
		uart_txstring(UART_NUMBER_1, "CRC Error General\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x01)){
		uart_txstring(UART_NUMBER_1, "Form Error General\r\n");
	}

	if(BIT_CHECK(CANGIT, 0x00)){
		uart_txstring(UART_NUMBER_1, "Acknowledgment Error General\r\n");
	}
*/
	
	uint8_t i,interrupt, mob_back;
	uint16_t canstatus, current_MOB = 1;

	uint8_t rpm_response_buffer[8];
	st_cmd_t rpm_msg;

	rpm_msg.pt_data = rpm_response_buffer;
	rpm_msg.status = 0;
	
	//char tempchar[10];
	/*
	 * Function to clear only the mob that generated the interrupt 
	 * ------------- Code flow --------------------
	 * Read CANSIT1 and CANSIT2 to figure out what mob(s) that generated
	 * the interrupt.
	 * Read CANSTMOB for each mob that generated a interrupt to determin
	 * the interrupt type.
	 * Make a switch case over the types and do meaning full handling for
	 * the types.
	 * TXOK just need to call Can_mob_abort() and Can_clear_status_mob()
	 * Proper action for all other types is TODO
	*/

	// Test mob's for pending interrupt 
	canstatus = CANSIT2+(CANSIT1<<8);

	mob_back = CANPAGE;	// Save CANPAGE state
	for(i=0; i <= 14; i++){
		if(BITMASK_CHECK(canstatus, current_MOB)){	// True if mob have pending interrupt
			Can_set_mob(i); // Switch to mob 
			interrupt = BITMASK_CHECK(CANSTMOB, INT_MOB_MSK);
			switch (interrupt){
				case MOB_RX_COMPLETED:
					uart_txstring(UART_NUMBER_1, "MOB_RX_COMPLETED \r\n");
					// Can specific code 
					can_get_data(&canDataTest[0]);	// Copy data to canDataTest
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					can_update_rx_msg(&rpm_msg, gear_msgid, 8);

//					Can_config_rx();	// Config mob for rx again
//					Can_set_mob_int(i);	// Enable interrupt
					
					break;
				case MOB_TX_COMPLETED:
					uart_txstring(UART_NUMBER_1, "MOB_TX_COMPLETED \r\n");
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status	
					// Disable interrupt 
					Can_unset_mob_int(i);
					break;				
				case MOB_ACK_ERROR:
					uart_txstring(UART_NUMBER_1, "MOB_ACK_ERROR \r\n");
					// TODO 
					break;
				case MOB_FORM_ERROR:
					uart_txstring(UART_NUMBER_1, "MOB_FORM_ERROR \r\n");
					// TODO 
					break;
				case MOB_CRC_ERROR:
					uart_txstring(UART_NUMBER_1, "MOB_CRC_ERROR \r\n");
					// TODO 
					break;
				case MOB_STUFF_ERROR:
					uart_txstring(UART_NUMBER_1, "MOB_STUFF_ERROR \r\n");
					// TODO 
					break;
				case MOB_BIT_ERROR:
					uart_txstring(UART_NUMBER_1, "MOB_BIT_ERROR \r\n");
					// TODO 
					break;
				default:
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					break;
			}
		}
		current_MOB = current_MOB<<1;
	}
	CANPAGE |= mob_back & 0xF0;	// Restore CANPAGE state
	
}

/* funktion til at sende en besked der er dlc byte lang og er ikke blocking 
 * return values:
 * 0 = Besked ikke kommet i udbakke
 * 1 = Besked kommet i udbakke
*/
uint8_t can_send_non_blocking(uint8_t msg_id, void* buf, uint8_t dlc){

	tx_remote_msg.pt_data = buf; 
	tx_remote_msg.id.std = msg_id;
	tx_remote_msg.ctrl.ide = 0;
	tx_remote_msg.ctrl.rtr = 1;
	tx_remote_msg.dlc = dlc;
	tx_remote_msg.cmd = CMD_TX_DATA;
	tx_remote_msg.blocking = 0;	/* For non blocking */

	/* can_cmd function extended with a feature to enable interrupt for
	 * the message mob picked for the message
	*/
	//return (can_cmd(&tx_remote_msg) == CAN_CMD_ACCEPTED);
	
	if (can_cmd(&tx_remote_msg) != CAN_CMD_ACCEPTED){
		return 0;	// No free mob could not put message in mail box
	}else{
		return 1;
	}
	
}

