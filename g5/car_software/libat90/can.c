#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "can_std/can_lib.h"
#include "bitwise.h"
#include "can.h"
#include "uart.h"

static void (*canit_callback[9])(uint8_t mob);
static void (*ovrit_callback)(void);

void set_canit_callback(enum can_int_t interrupt, void (*callback)(uint8_t mob)) {
	canit_callback[interrupt] = callback;
}

int can_setup(can_msg_t *msg){
	CAN_SET_MOB(msg->mob); // Move CANPAGE point the the given mob
	
	switch(msg->mode){
		case MOB_DISABLED:
			MOB_ABORT();
			break;
		case MOB_TRANSMIT:

			break;
		case MOB_RECIEVE:
			MOB_SET_STD_ID(msg->id);
			MOB_SET_STD_MASK_FILTER(MASK_FULL_FILTERING); 
			Can_config_rx(); // OSBS!! we are configuring specifically for mode MOB_RECIEVE
			CAN_ENABLE_MOB_INTERRUPT(msg->mob);
			break;
		case MOB_AUTOMATIC_REPLY:
			break;
		case MOB_FRAME_BUFF_RECEIVE:
			break;
		default:
			return 1;  // Error
			break;
	}
	return 0; // Success;
}

int can_receive(can_msg_t *msg){
	if ( !MOB_HAS_PENDING_INT(msg->mob) ){
		return 1; // Error no pending interrupt
	}
	CAN_SET_MOB(msg->mob);

	if ( !((CANSTMOB == MOB_RX_COMPLETED_DLCW) || (CANSTMOB == MOB_RX_COMPLETED)) ) {
		MOB_CLEAR_INT_STATUS();
		return 2; // Error 
	}
	// Fill the msg with received data
	MOB_SET_STD_ID(msg->id); 			// Fill in the msg id
	msg->dlc = MOB_GET_DLC(); 			// Fill in the msg dlc
	CAN_RX_DATA(msg->data, msg->dlc);	// Fill in the msg data

	MOB_CLEAR_INT_STATUS(); 	// and reset MOb status
	BIT_SET(CANCDMOB, CONMOB1); // enable reception

	return 0; // success
}

int can_send(can_msg_t *msg){
	CAN_SET_MOB(msg->mob);

	MOB_SET_DLC(msg->dlc); // Set the expected payload length
	CAN_TX_DATA(msg->data, msg->dlc);
	Can_config_tx();
	CAN_ENABLE_MOB_INTERRUPT(msg->mob);

	return CANSTMOB;
}

/*
 * The Can_clear_mob() function clears the following registers:
 * CANSTMOB 			-- Contains interrupt status
 * CANCDMOB 			-- Defines MOB mode and msg length
 * CANIDT1 ... CANIDT4	-- CAN Identifier Tag Registers
 * CANIDM1 ... CANIDT4	-- CAN Identifier Mask Registers
 */
	//Can_clear_rtr();							/* no remote transmission request */
	//Can_set_rtrmsk();							/* Remote Transmission Request - comparison true forced */
	//Can_set_idemsk();							/* Identifier Extension - comparison true forced */
	//clear_mob_status(mob);					/* Described above */

ISR (CANIT_vect) {
	uint8_t mob;

	// Loop over each MOB and check if it have pending interrupt
	for (mob = 0; mob <= LAST_MOB_NB; mob++) {
		if (MOB_HAS_PENDING_INT(mob)) { /* True if mob have pending interrupt */
			CAN_SET_MOB(mob); // Switch to mob

			switch (CANSTMOB) {
				case MOB_RX_COMPLETED_DLCW:
					(*canit_callback[0])(mob);
					// Fall through to MOB_RX_COMPLETED on purpose
					//!< @todo NEEDS TESTING. !!!URGENT!!!
				case MOB_RX_COMPLETED:
					(*canit_callback[1])(mob);
					break;
				case MOB_TX_COMPLETED:
					(*canit_callback[2])(mob);
					break;
				case MOB_ACK_ERROR:
					(*canit_callback[3])(mob);
					break;
				case MOB_FORM_ERROR:
					(*canit_callback[4])(mob);
					break;
				case MOB_CRC_ERROR:
					(*canit_callback[5])(mob);
					break;
				case MOB_STUFF_ERROR:
					(*canit_callback[6])(mob);
					break;
				case MOB_BIT_ERROR:
					(*canit_callback[7])(mob);
					break;
				default:
					(*canit_callback[8])(mob);
					break;
			}
		}
	}
}

ISR (OVRIT_vect) {
	(*ovrit_callback)();
}
