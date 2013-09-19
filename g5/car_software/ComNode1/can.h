#include <stdint.h>
#include "can_std/can_lib.h"

extern st_cmd_t tx_remote_msg;

extern uint8_t can_send_non_blocking(uint8_t msg_id, void* msg, uint8_t data_length);
extern uint8_t can_update_rx_msg(st_cmd_t* msg, uint8_t msg_id, uint8_t dlc);
extern uint8_t can_config_rx_mailbox(st_cmd_t* msg, uint8_t count);

