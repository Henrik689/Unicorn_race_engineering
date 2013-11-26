#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "can_std/can_lib.h"
#include "bitwise.h"
#include "can.h"
#include "uart.h"

static void (*canit_callback[9])(uint8_t mob);

void set_canit_callback(enum canit_int interrupt, void (*callback)(uint8_t mob)) {
	canit_callback[interrupt] = callback;
}

void can_subscribe(can_msg_t *msg){
	Can_set_mob(msg->mob); // Move CANPAGE point the the given mob
	Can_set_std_id(msg->id);
	Can_set_std_msk((uint16_t){UINT16_MAX}); 
	Can_config_rx(); // OSBS!! we are configuring specifically for mode 2
	Can_set_mob_int(msg->mob);
}

int can_receive(can_msg_t *msg){
	if ( !MOB_HAS_PENDING_INT(msg->mob) ){
		return 1; // Error no pending interrupt
	}
	Can_set_mob(msg->mob);

	if ( !((CANSTMOB == MOB_RX_COMPLETED_DLCW) || (CANSTMOB == MOB_RX_COMPLETED)) ) {
		Can_clear_status_mob();
		return 2; // Error 
	}

	can_get_data(msg->data);
	Can_clear_status_mob(); 	// and reset MOb status
	BIT_SET(CANCDMOB, CONMOB1); // enable reception

	return 0; // success
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

void setup_mob_rx(uint8_t mob, uint16_t id, uint8_t dlc) {
	set_mob_id(mob, id);
	set_mob_mask(mob, (uint16_t){UINT16_MAX});
	set_mob_dlc(mob, dlc);
	set_mob_mode(mob, 2);
	Can_set_mob_int(mob);						/* Enable interrupt for MOB */
}

void setup_mob_tx(uint8_t mob, uint16_t id, uint8_t *data, uint8_t dlc) {
	set_mob_id(mob, id);
	set_mob_dlc(mob, dlc);
	set_data_reg(mob, data, dlc);
	set_mob_mode(mob, 1);
	Can_set_mob_int(mob);						/* Enable interrupt for MOB */
}

void set_data_reg(uint8_t mob, uint8_t *data, uint8_t dlc) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	CAN_TX_DATA(data, dlc);
}

void set_mob_dlc(uint8_t mob, uint8_t dlc) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	Can_set_dlc(dlc);							/* Expected msg length*/
}

void set_mob_id(uint8_t mob, uint16_t id) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	Can_set_std_id(id);							/* Sets the id */
}

void set_mob_mask(uint8_t mob, uint16_t mask) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	Can_set_std_msk(mask);						/* The mask is used to define the range of accepted IDs */
}

void clear_mob_status(uint8_t mob) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	Can_clear_mob();							/* Clear ALL status registers for MOB*/
}

void set_mob_mode(uint8_t mob, enum mob_mode mode) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	switch (mode) {
		case MOB_DISABLED:
			DISABLE_MOB;
			break;
		case MOB_TRANSMIT:
			Can_config_tx();
			break;
		case MOB_RECIEVE:
			Can_config_rx();
			break;
		case MOB_AUTOMATIC_REPLY:
			break;
		case MOB_FRAME_BUFF_RECEIVE:
			Can_config_rx_buffer();
			break;
	}
}

ISR (CANIT_vect) {
	int mob;

	// Loop over each MOB and check if it have pending interrupt
	for (mob = 0; mob <= LAST_MOB_NB; mob++) {
		if (MOB_HAS_PENDING_INT(mob)) { /* True if mob have pending interrupt */
			Can_set_mob(mob); /* Switch to mob */

			switch (CANSTMOB) {
				case MOB_RX_COMPLETED_DLCW:
					(*canit_callback[0])(mob);
					// Fall through to MOB_RX_COMPLETED on purpose
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
	/* CAN Timer Overrun interrupt */
}
