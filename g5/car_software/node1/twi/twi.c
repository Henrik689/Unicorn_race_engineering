#include "../config.h"
#include <avr/io.h>
#include <util/twi.h>
#include "twi.h"

void TWI_init(void)
{
	PORTD |= (1<<PORTD0)|(1<<PORTD1);	/* Enable pull-up on TWI pins */

	/* Set TWI clock */
	TWSR = 0;	/* Set prescaler to 1 */
	TWBR = (F_CPU/TWI_CLOCK-16)/2;	/* Calculate TWBR value */
}

BOOL TWI_start(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);	/* send start condition */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	if (!((TW_STATUS == TW_REP_START) || (TW_STATUS == TW_START))) return FALSE; /* Return if communication could not be started */
	return TRUE;
}

/* Send a byte to the TWI bus */
void TWI_send(uint8_t data)
{
	TWDR = data;
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
}	

/* Read a byte from the TWI bus */
uint8_t TWI_rcvr(BOOL ack)
{
	if (ack) {
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA); 	/* send ACK after data recived */
	} else {
		TWCR = _BV(TWINT) | _BV(TWEN);			/* send NACK after data recived */
	}		 
	while ((TWCR & _BV(TWINT)) == 0) ; 			/* wait for transmission */	
	return 	TWDR;
}
		
void TWI_stop(void)
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
}
