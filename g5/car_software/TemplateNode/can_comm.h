//#include "integer.h"

extern st_cmd_t tx_remote_msg;

uint8_t can_send_non_blocking(uint8_t msg_id, void* msg, uint8_t data_length);
void can_update_rx_msg(st_cmd_t* msg, uint8_t msg_id, uint8_t dlc);
