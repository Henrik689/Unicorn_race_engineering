#include "config.h"
#include "uart.h"
#include "comm.h"
#include "ecu.h"
#include <util/delay.h>
#include "error.h"
#include "queue.h"

volatile uint8_t xbee_seq_index=0;

void xbee_send_trigger( void )
{
	/* TODO
	 * 
	 *
	 * -Trigger package send 
	 */
	/* Check if xbee is already sending and if true just return */
	if (xbee_sending) return;

	/* If number of bytes in send buffer is greater than X send data */
	if (QUEUE_GET_NUM_ELE(xbee_q)>=40){
		xbee_sending = 1;
		xbee_seq_index = 0;
		UDR1 = start_sequence[xbee_seq_index++]; // Send first byte in the start sequence
		Usart1_tx_ei_en(); // Enable tx interrupt
	}
	return;
}
