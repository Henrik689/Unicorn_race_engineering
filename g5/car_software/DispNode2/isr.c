#include "can.h"
#include "prototyper.h"
#include "data.h"


/* TODO
 * Find out if INT6 or INT7 is down
 */

/* ISR for gear buttom */
ISR(INT6_vect)
{
	params.GearButDown = 1;
	/* check state of neutral buttom */
	if(!((PINC&0b10000000)==0b10000000)) {
		params.GearButNeutral = 1;
	}
}

/* ISR for gear buttom */
ISR(INT7_vect)
{
	params.GearButUp = 1;
	/* check state of neutral buttom */
	if(!((PINC&0b10000000)==0b10000000)) {
		params.GearButNeutral = 1;
	}
}
