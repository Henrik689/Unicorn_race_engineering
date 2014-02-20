#include "config.h"
#include "extern.h"
#include "can_std/can_lib.h"
#include "can_new.h"
//#include "test_vars.h"
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "prototyper.h"

st_cmd_t tx_remote_msg;

unsigned short int can_update_rx_msg(st_cmd_t* msg, U8 msg_id, U8 dlc)
{
      //  U8 i;
        
        msg->id.std = msg_id;
        msg->ctrl.ide = 0;
        msg->ctrl.rtr = 0;
        msg->dlc = dlc;
        msg->cmd = CMD_RX_DATA_MASKED;

        while(can_cmd(msg) != CAN_CMD_ACCEPTED);
        return 0;
}

/* Interrupt routine to take care of can interrupts */
ISR(CANIT_vect)
{
	uint8_t i,interrupt, mob_back;
	uint16_t tmp,mask=1;
	//uint16_t tmp2;
	U8 rpm_response_buffer[8];
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

	/* Test mob's for pending interrupt */
	tmp = CANSIT2+(CANSIT1<<8);

	mob_back = CANPAGE;	// Save CANPAGE state
	for(i=0;i<=14;i++){
		if(tmp & mask){	/* True if mob have pending interrupt */
			Can_set_mob(i); /* Switch to mob */
			interrupt = (CANSTMOB & INT_MOB_MSK);
			switch (interrupt){
				case MOB_RX_COMPLETED:
					/* Can specific code */
					//can_get_data(&canDataTest[0]);	// Copy data to canDataTest
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					can_update_rx_msg(&rpm_msg, gear_msgid, 8);
//					Can_config_rx();	// Config mob for rx again
//					Can_set_mob_int(i);	// Enable interrupt
					/* Take care of the data code */
//					sendtekst("CAN val: ");
//					itoa(canDataTest[0],tempchar,10);
//					sendtekst(tempchar);
//					sendtekst("\r\n");

					// if (canDataTest[0] == gear)  		TESTING 
					// {  
					// 	gearButCAN = canDataTest[1];
					// }
					// /*	Kan bruges til udregning af gearratio
					// else if (canDataTest[0] == roadSpeed) 
					// {  
					// 	roadSpeed_val = ((canDataTest[1]<<8) + canDataTest[2]); // speed x 100
					// }
					// else if (canDataTest[0] == rpm) 
					// {  
					// 	rpm_val = ((canDataTest[1]<<8) + canDataTest[2])*0.9408;
					// }
					// */
					break;
				case MOB_TX_COMPLETED:
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status	
					/* Disable interrupt */
					Can_unset_mob_int(i);
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
	CANPAGE |= mob_back & 0xf0;	// Restore CANPAGE state
}

/* funktion til at sende en besked der er dlc byte lang og er ikke blocking 
 * return values:
 * 0 = Besked ikke kommet i udbakke
 * 1 = Besked kommet i udbakke
*/
U8 can_send_non_blocking(U8 msg_id, void* buf, U8 dlc)
{
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
	if (can_cmd(&tx_remote_msg) != CAN_CMD_ACCEPTED){
		return 0;	// No free mob could not put message in mail box
	}else{
		return 1;
	}
}

