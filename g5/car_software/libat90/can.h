#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include "can_std/can_lib.h"
#include "bitwise.h"

#define CAN_SEI()		( Can_sei()			)
#define CAN_RX_SEI()	( Can_set_rx_int() 	)
#define CAN_TX_SEI()	( Can_set_tx_int() 	)

#define CAN_INIT()		( can_init(0) )
#define CAN_INIT_ALL()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); CAN_TX_SEI(); 	}
#define CAN_INIT_RX()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); 					}
#define CAN_INIT_TX()	{ CAN_INIT(); CAN_SEI(); CAN_TX_SEI(); 					}

#define CAN_FORCE_CMD(cmd)			while( can_cmd((cmd)) != CAN_CMD_ACCEPTED 				)
#define CAN_FORCE_COMPLETE(cmd)		while( can_get_status((cmd)) != CAN_STATUS_COMPLETED 	)

void can_send(int id, uint8_t *data, uint8_t length);
void setup_mob_rx(uint8_t mob, uint16_t id, uint8_t dlc);
void setup_mob_tx(uint8_t mob, uint16_t id, uint8_t *data, uint8_t dlc);
void set_mob_id(uint8_t mob, uint16_t id);
void clear_mob_status(uint8_t mob);
void set_mob_dlc(uint8_t mob, uint8_t dlc);
void set_mob_mask(uint8_t mob, uint16_t mask);
int set_mob_mode(uint8_t mob, uint8_t mode);

// Debugging test functions
void can_testSender(void);

#endif /* CAN_H */
