#ifndef CAN_H
#define CAN_H

#include "can_std/can_lib.h"


#define CAN_SEI()		( Can_sei()			)
#define CAN_RX_SEI()	( Can_set_rx_int() 	)
#define CAN_TX_SEI()	( Can_set_tx_int() 	)

#define CAN_INIT()		( can_init(0) )
#define CAN_INIT_ALL()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); CAN_TX_SEI(); 	}
#define CAN_INIT_RX()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); 					}
#define CAN_INIT_TX()	{ CAN_INIT(); CAN_SEI(); CAN_TX_SEI(); 					}

#define CAN_FORCE_CMD(cmd)			while( can_cmd((cmd)) != CAN_CMD_ACCEPTED 				)
#define CAN_FORCE_COMPLETE(cmd)		while( can_get_status((cmd)) != CAN_STATUS_COMPLETED 	)

void can_send(int id, uint8_t *data, size_t length);
void can_receive(int id, uint8_t *data, size_t length);

// Debugging test functions
void can_testReceiver(void);
void can_testSender(void);

#endif /* CAN_H */
