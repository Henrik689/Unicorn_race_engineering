#include <stdio.h>
#include <avr/interrupt.h>
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

void mob_create(int mob_id, st_cmd_t* descriptor) {
	Can_set_mob(mob_id);						/* Move CANPAGE to point at given MOB */
	Can_clear_mob();							/* Clear ALL status registers for MOB*/
	Can_set_std_id(descriptor->id.std)			/* Sets the id */	
	Can_set_std_msk((uint16_t){0});				/* The mask is used to define the range of accepted IDs */
	Can_set_dlc(descriptor->dlc);				/* Expected msg length*/
	Can_clear_rtr();							/* no remote transmission request */
	Can_set_rtrmsk();							/* Remote Transmission Request - comparison true forced */
	Can_set_idemsk();							/* Identifier Extension - comparison true forced */
	Can_config_rx();  							/* Set MOB in recieve mode */
	Can_set_mob_int(mob_id);					/* Enable interrupt for MOB */
}

void can_send(int id, uint8_t *data, uint8_t length) {
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

void can_receive(int mob_id, int id, uint8_t *data, uint8_t length) {
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
}

void can_testReceiver(void) {
	can_receive(10, 5, &msg_buff[0], 6);
	can_receive(4, 4, &msg_buff[0], 5);
}

void can_testSender(void) {
	uint8_t databuffer[5] = {'H', 'E', 'L', 'L', 'O'};
	can_send(4, &databuffer[0], 5);
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
