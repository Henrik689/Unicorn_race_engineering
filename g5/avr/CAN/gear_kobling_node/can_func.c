#include "config.h"
#include "can_lib.h"
#include "can_func.h"

U8 tx_gear_shift_buffer[8];
st_cmd_t tx_gear_shift_msg;

unsigned short int can_check( U8 msg_id)
{
    unsigned short int i = 0;
    // --- Init Rx Commands        
    for(i=0; i<8; i++)  // Nulstiller buffer
        rpm_response_buffer[i]=255;

    rpm_msg.id.std = msg_id;
    rpm_msg.ctrl.ide = 0;
    rpm_msg.ctrl.rtr = 0;
    rpm_msg.dlc = 8;               //Antal bytes der skal modtages
    rpm_msg.cmd = CMD_RX_DATA_MASKED;
    // --- Rx Command
    while(can_cmd(&rpm_msg) != CAN_CMD_ACCEPTED);
    // Venter på data er modtaget
    while(can_get_status(&rpm_msg) == CAN_STATUS_NOT_COMPLETED); 

return rpm_response_buffer[0];
}

unsigned short int can_update_rx_msg(st_cmd_t* msg, U8 msg_id, U8 dlc)
{
        U8 i;
    
        for (i=0; i<dlc; i++) {
                *(msg->pt_data+i) = 255;
        }
        
        msg->id.std = msg_id;
        msg->ctrl.ide = 0;
        msg->ctrl.rtr = 0;
        msg->dlc = dlc;
        msg->cmd = CMD_RX_DATA_MASKED;

        while(can_cmd(msg) != CAN_CMD_ACCEPTED);
}
