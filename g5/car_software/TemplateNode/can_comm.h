//#include "integer.h"

extern st_cmd_t tx_remote_msg;

U8 can_send_non_blocking(U8 msg_id, void* msg, U8 data_length);
void can_update_rx_msg(st_cmd_t* msg, U8 msg_id, U8 dlc);
