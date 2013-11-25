#include <stdio.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "can_std/can_lib.h"
#include "bitwise.h"
#include "can.h"
#include "uart.h"

static uint8_t msg_buff[NB_DATA_MAX + 1] = {0};

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

int set_mob_mode(uint8_t mob, uint8_t mode) {
	Can_set_mob(mob);							/* Move CANPAGE to point at given MOB */
	switch (mode) {
		case 0:
			DISABLE_MOB;
			break;
		case 1:
			Can_config_tx();
			break;
		case 2:
			Can_config_rx();
			break;
		case 3:
			Can_config_rx_buffer();
			break;
		default:
			return -1;
			break;
	}
	return 0;
}

ISR (CANIT_vect) {
	const uint16_t cansit = CANSIT2 + (CANSIT1 << 8); /* CAN Status Interrupt MOb Registers */
	int i;

	// Loop over each MOB and check if it have pending interrupt
	for (i = 0; i <= LAST_MOB_NB; i++) {
		if (BITMASK_CHECK(cansit, (0x01 << i))) { /* True if mob have pending interrupt */
			Can_set_mob(i); /* Switch to mob */

			switch (CANSTMOB) {
				case MOB_RX_COMPLETED_DLCW:
				case MOB_RX_COMPLETED:
					can_get_data(&msg_buff[0]);	// Copy data to canDataTest

					uart_printf(UART_NUMBER_1, "MOB_%d msg:%s\r\n", i, msg_buff);

					Can_clear_status_mob(); // and reset MOb status
					BIT_SET(CANCDMOB, CONMOB1); /* enable reception */
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
					Can_clear_status_mob(); // and reset MOb status
					break;
			}
		}
	}
}

ISR (OVRIT_vect) {
	/* CAN Timer Overrun interrupt */
}
