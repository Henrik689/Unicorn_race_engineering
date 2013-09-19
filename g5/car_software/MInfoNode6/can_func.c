#include "config.h"
#include "can_std/can_lib.h"
#include "can_func.h"
#include "can_logning.h"

U8 response_buffer[num_of_response_mobs][8];
st_cmd_t response_msg[num_of_response_mobs];

U8 databuffer[data_buffer_num][8];

U8 bufferindex = 0;

U8 tx_remote_buffer[8];
st_cmd_t tx_remote_msg;

U8 sample_time = 0; /* Should be updated by a timing function */

/* New can vars */
U8 tx_buffer[8];
st_cmd_t tx_std_msg;

ISR(CANIT_vect)
{
	uint8_t tmp,i;
	uint8_t data_buf[8];
	data_buf[0] = 10;
	data_buf[1] = 11;
	data_buf[2] = 11;
	data_buf[3] = 11;
	tmp = CANSIT2;
	//xprintf(PSTR("rc=%d\n"), tmp);
	for(i=0;i<=14;i++){
		Can_set_mob(i);
		Can_mob_abort();        // Freed the MOB
		Can_clear_status_mob(); //   and reset MOb status
	}
	//can_send_ny(133, data_buf, 4);
}

BOOL can_send_standart_data(
dataPackage* type,
U8 *datapakke	/* pointer to datapakke buffer. Datapakke should follow the protol!!! */
)
{
	can_send_test(STD_DATA, datapakke, type->dlc);
	return TRUE;
}

void init_can_data_mobs(void)
{
    U8 i,j;

    for (i=0; i<num_of_response_mobs; i++) {
        response_msg[i].pt_data = &response_buffer[i][0];
        response_msg[i].status = 0;
        for (j=0; j<9; j++) {
                response_buffer[i][j] = 0;
        }
    }
}

void can_data_mob_setup(U8 mob_num)
{
    response_msg[mob_num].id.std = 128;
    response_msg[mob_num].ctrl.ide = 0;
    response_msg[mob_num].ctrl.rtr = 0;
    response_msg[mob_num].dlc = 8;
    response_msg[mob_num].cmd = CMD_RX_DATA_MASKED;
    
    while (can_cmd(&response_msg[mob_num]) != CAN_CMD_ACCEPTED);
}

/* funktion til at sende en besked der er 1 byte lang */
void can_send(U8 msg_id, U8 msg, U8 dlc)
{
    tx_remote_buffer[0] = msg; 
	dlc = 4;
    tx_remote_msg.id.std = msg_id;
    tx_remote_msg.ctrl.ide = 0;
    tx_remote_msg.ctrl.rtr = 1;
    tx_remote_msg.dlc = dlc;
    tx_remote_msg.cmd = CMD_TX_DATA;

    while (can_cmd(&tx_remote_msg) != CAN_CMD_ACCEPTED);

    while (can_get_status(&tx_remote_msg) == CAN_STATUS_NOT_COMPLETED);
}

/* funktion til at sende en besked der er dlc byte lang */
void can_send_ny(U8 msg_id, void* buf, U8 dlc)
{
    tx_remote_msg.pt_data = buf; 
    tx_remote_msg.id.std = msg_id;
    tx_remote_msg.ctrl.ide = 0;
    tx_remote_msg.ctrl.rtr = 1;
    tx_remote_msg.dlc = dlc;
    tx_remote_msg.cmd = CMD_TX_DATA;

    while (can_cmd(&tx_remote_msg) != CAN_CMD_ACCEPTED);

    //while (can_get_status(&tx_remote_msg) == CAN_STATUS_NOT_COMPLETED);
}

/* funktion til at sende en besked der er dlc byte lang */
/* TODO find out why the board reboot when using this function. 
 * Probably the error can be fixed by making an interrupt function 
 * for the can tx interrupt to clear 
 */ 
void can_send_test(U8 msg_id, void* buf, U8 dlc)
{
	uint8_t *tmp;
	tmp = tx_remote_msg.pt_data; /* Backup of pointer to restore it */    
	tx_remote_msg.pt_data = buf; 
	
	tx_remote_msg.id.std = msg_id;
	tx_remote_msg.ctrl.ide = 0;
	tx_remote_msg.ctrl.rtr = 0;
	tx_remote_msg.dlc = dlc;
	tx_remote_msg.cmd = CMD_TX_DATA;

	while (can_cmd(&tx_remote_msg) != CAN_CMD_ACCEPTED);

	Can_set_mob_int(tx_remote_msg.handle);	/* Enable interrupt for mob so it will clear it self when data is send */

	tx_remote_msg.pt_data = tmp;	/* Restore pointer */
}
