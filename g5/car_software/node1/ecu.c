#include "config.h"
#include <util/delay.h>
#include <util/atomic.h>
#include "uart.h"
#include "xitoa.h"
#include "ecu.h"
#include "queue.h"
#include "comm.h"
#include "can.h"
#include "log.h"
#include "../lib/can_defs.h"

uint8_t EcuData[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
    23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,
    48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,
    73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,
    98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114};

void ecu_val_to_xbee(uint8_t i, uint8_t j)
{
	uint8_t k=0;
//	xprintf(PSTR("i%d,j%d \n"), i, j);
	/* Add value to xbee buffer */
    	ATOMIC_BLOCK(ATOMIC_FORCEON)
    	{
		QUEUE_PUT(xbee_q, ECUObjects[j].id);	/* Add id */
	
		for (k=0;k<ECUObjects[j].length;k++) {	/* Add value */
			QUEUE_PUT(xbee_q, EcuData[i+k]);
		}
	}
	/* Check if more than 20 elements in xbee buffer */
//	if (QUEUE_GET_NUM_ELE(xbee_q) >= 40) {
		//xprintf(PSTR("More than 20 elements\n"));
//		xbee_send_trigger();
//	}
}

void ecu_data_handler( void )
{
	uint8_t i=0;
	uint8_t	j=0;
	uint8_t k;
	int hej = 40;
	uint8_t data[8];

	/* Loop over all values in EcuData */
	while(i<=114) {
		/* Check if anything to be done for value */

		if (valueObjects[ECUObjects[j].id].action & (TO_XBEE | TO_SD | TO_CAN) ) {
			/* Value to xbee? */
			if (valueObjects[ECUObjects[j].id].action & TO_XBEE) {
			    	//_delay_us(10);
				ecu_val_to_xbee(i,j);
			}


			/* Value to SD? */
			if (valueObjects[ECUObjects[j].id].action & TO_SD) {
				/* TODO
				 * Insert call to val_to_sd() when the function is
				 * made
				 */
				data[0] = ECUObjects[j].id;
				for (k=0;k<ECUObjects[j].length;k++){
					data[k+1]= EcuData[i+k];
				}
				sd_log_write(&data[0], ECUObjects[j].length+1);
			}
	
			/* Value to CAN? */
			if (valueObjects[ECUObjects[j].id].action & TO_CAN) {
				/* TODO
				 * Insert call to val_to_CAN() when the function is
				 * made
				 */
				data[0] = ECUObjects[j].id;
				for (k=0;k<ECUObjects[j].length;k++){
					data[k+1]= EcuData[i+k];
				}
				can_send_non_blocking(rpm_msgid, &data[0], ECUObjects[j].length+1);
			}
		}
		
		/* Find index for next value */
		i += ECUObjects[j].length;
		j++;
	}
}
