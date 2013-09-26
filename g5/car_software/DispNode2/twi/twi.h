#include "../integer.h"

void TWI_init(void);
BOOL TWI_start(void);
/* Send a byte to the TWI bus */
void TWI_send(uint8_t data);
/* Read a byte from the TWI bus */
uint8_t TWI_rcvr(BOOL ack);
void TWI_stop(void);
