#include <stdlib.h>
#include <avr/io.h>
#include "can.h"
#include "../lib/can_defs.h"
#include "display/display.h"
#include "prototyper.h"
#include "../lib/data_def.h"
#include "data.h"

//#define DEBUG_RX
//#define DEBUG_TX
#define DEBUG_ACK_ERROR
#define DEBUG_BIT_ERR
#define DEBUG_CRC_ERR
#define DEBUG_FORM_ERR
#define DEBUG_STUFF_ERR
#define DEBUG_DEFAULT

st_cmd_t tx_remote_msg;

/* Interrupt routine to take care of can interrupts */
ISR(CANIT_vect)
{
	uint8_t i,interrupt, mob_back;
	uint16_t tmp,tmp2,mask=1;
	uint8_t DataBuf[8];

	char textBuf[10];

	uint8_t rpm_response_buffer[8];
	st_cmd_t rpm_msg;

	rpm_msg.pt_data = rpm_response_buffer;
	rpm_msg.status = 0;
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
					can_get_data(&DataBuf[0]);	// Copy data to canDataTest
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					rpm_msg.status = 0;
					can_update_rx_msg(&rpm_msg, rpm_msgid, 8);	/* TODO Lav det her på en anden måde */
					#ifdef DEBUG_RX
						itoa(i, textBuf, 10);
						sendtekst("Rx ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					/* Take care of the data */
					PORTB ^= (1<<PB5);
					if (DataBuf[0] == rpm) {
						params.rpm = ((DataBuf[1]<<8) + DataBuf[2])*0.9408;
					} else if (DataBuf[0] == roadSpeed) {
						params.roadSpeed = ((DataBuf[1]<<8) + DataBuf[2])*0.01;
					} else if (DataBuf[0] == batteryV) {
						params.batteryV = ((DataBuf[1]<<8) + DataBuf[2])*(1.0/210.0)*10;
					} else if (DataBuf[0] == waterTemp) {
						params.waterTemp = (((DataBuf[1]<<8) + DataBuf[2])*(-150.0/3840.0)+120);
					} else if (DataBuf[0] == N6ADC5) {
						params.oilPressure = (DataBuf[1]<<8) + DataBuf[2];
					} else if (DataBuf[0] == GearNeutral) {
						params.GearNeutral = (DataBuf[1]<<8) + DataBuf[2];
					} else if (DataBuf[0] == GearEst) {
						params.GearEst = DataBuf[2];
					}

					break;
				case MOB_TX_COMPLETED:
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status	
					#ifdef DEBUG_TX
						itoa(i, textBuf, 10);
						sendtekst("Tx ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					Can_unset_mob_int(i);		/* TODO Måske virker funktionen ikke */
					break;				
				case MOB_ACK_ERROR:
					#ifdef DEBUG_ACK_ERROR
						itoa(i, textBuf, 10);
						sendtekst("ACK_ERR ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					/* TODO */
					break;
				case MOB_FORM_ERROR:
					#ifdef DEBUG_FORM_ERR
						itoa(i, textBuf, 10);
						sendtekst("FORM_ERR ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					/* TODO */
					break;
				case MOB_CRC_ERROR:
					#ifdef DEBUG_CRC_ERR
						itoa(i, textBuf, 10);
						sendtekst("CRC_ERR ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					/* TODO */
					break;
				case MOB_STUFF_ERROR:
					#ifdef DEBUG_STUFF_ERR
						itoa(i, textBuf, 10);
						sendtekst("STUFF_ERR ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					/* TODO */
					break;
				case MOB_BIT_ERROR:
					#ifdef DEBUG_BIT_ERR
						itoa(i, textBuf, 10);
						sendtekst("BIT_ERR ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					/* TODO */
					break;
				default:
					#ifdef DEBUG_DEFAULT
						itoa(i, textBuf, 10);
						sendtekst("Default ");
						sendtekst(textBuf);
						sendtekst("\n\r");
					#endif
					Can_mob_abort();        // Freed the MOB
					Can_clear_status_mob(); // and reset MOb status
					break;
			}
		}
		mask = mask<<1;
	}
	CANPAGE |= mob_back & 0xf0;	// Restore CANPAGE state
}

/* TODO 
 * Make a can_read function that is non blocking
*/

/* funktion til at sende en besked der er dlc byte lang og er ikke blocking 
 * return values:
 * 0 = Besked ikke kommet i udbakke
 * 1 = Besked kommet i udbakke
*/
uint8_t can_send_non_blocking(uint8_t msg_id, void* buf, uint8_t dlc)
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

uint8_t can_update_rx_msg(st_cmd_t* msg, uint8_t msg_id, uint8_t dlc)
{
        uint8_t i;
        
        msg->id.std = msg_id;
        msg->ctrl.ide = 0;
        msg->ctrl.rtr = 0;
        msg->dlc = dlc;
        msg->cmd = CMD_RX_DATA_MASKED;

        while(can_cmd(msg) != CAN_CMD_ACCEPTED);
}

uint8_t can_config_rx_mailbox(st_cmd_t* msg, uint8_t count)
{
	uint8_t i;

	msg->ctrl.ide = 0;
	msg->ctrl.rtr = 0;
	msg->cmd = CMD_RX_DATA_MASKED;
	
	for (i=0;i<count;i++) {
		if (can_cmd(msg) != CAN_CMD_ACCEPTED) {
			return CAN_CMD_REFUSED;
		}
	}
	return CAN_CMD_ACCEPTED;
}
