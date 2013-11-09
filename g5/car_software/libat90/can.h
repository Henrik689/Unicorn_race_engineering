#ifndef CAN_H
#define CAN_H

#include "config.h"
#include "can_std/can_lib.h"

#define CAN_INIT()		(can_init(0))

#define CAN_SEI()		(Can_sei())
#define CAN_RX_SEI()	(Can_set_rx_int())
#define CAN_TX_SEI()	(Can_set_tx_int())


void canTestReceiver(void);
void canTestSender(void);

#endif /* CAN_H */
