//_____ VARIABLES ______________________________________________________________
// Recieve buffer
U8 rpm_response_buffer[8];
st_cmd_t rpm_msg;

U8 gear_status_response_buffer[8];
st_cmd_t gear_status_msg;

U8 error_response_buffer[8];
st_cmd_t error_msg;

extern U8 tx_gear_shift_buffer[8];
extern st_cmd_t tx_gear_shift_msg;

/* function to check for new msg with msg_id */
unsigned short int can_check( U8 msg_id);

/* update rx mob to recive new data */
unsigned short int can_update_rx_msg(st_cmd_t* msg, U8 msg_id, U8 data_length);
