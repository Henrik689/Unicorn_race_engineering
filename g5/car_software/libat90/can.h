#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include "can_std/can_lib.h"
#include "bitwise.h"

// ------------ Old wrappers
#define CAN_SEI()		( Can_sei()			)
#define CAN_RX_SEI()	( Can_set_rx_int() 	)
#define CAN_TX_SEI()	( Can_set_tx_int() 	)

#define CAN_INIT()		( can_init(0) )
#define CAN_INIT_ALL()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); CAN_TX_SEI(); 	}
#define CAN_INIT_RX()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); 					}
#define CAN_INIT_TX()	{ CAN_INIT(); CAN_SEI(); CAN_TX_SEI(); 					}

#define CAN_FORCE_CMD(cmd)			while( can_cmd((cmd)) != CAN_CMD_ACCEPTED 				)
#define CAN_FORCE_COMPLETE(cmd)		while( can_get_status((cmd)) != CAN_STATUS_COMPLETED 	)
// ------------- End old wrappers

#define CAN_TX_DATA(data, len)	{ int i; for(i=0; i<len; i++){CANMSG = data[i];} }

enum canit_int {
	CANIT_RX_COMPLETED_DLCW = 0,
	CANIT_RX_COMPLETED = 1,
	CANIT_TX_COMPLETED = 2,
	CANIT_ACK_ERROR = 3,
	CANIT_FORM_ERROR = 4,
	CANIT_CRC_ERROR = 5,
	CANIT_STUFF_ERROR = 6,
	CANIT_BIT_ERROR = 7,
	CANIT_DEFAULT = 8
};

enum mob_mode {
	MOB_DISABLED,
	MOB_TRANSMIT,
	MOB_RECIEVE,
	MOB_AUTOMATIC_REPLY,
	MOB_FRAME_BUFF_RECEIVE
};

void set_canit_callback(enum canit_int interrupt, void (*callback)(uint8_t mob));
void setup_mob_rx(uint8_t mob, uint16_t id, uint8_t dlc);
void setup_mob_tx(uint8_t mob, uint16_t id, uint8_t *data, uint8_t dlc);
void set_mob_id(uint8_t mob, uint16_t id);
void clear_mob_status(uint8_t mob);
void set_mob_dlc(uint8_t mob, uint8_t dlc);
void set_mob_mask(uint8_t mob, uint16_t mask);
void set_mob_mode(uint8_t mob, enum mob_mode mode);
void set_data_reg(uint8_t mob, uint8_t *data, uint8_t dlc);

#endif /* CAN_H */
