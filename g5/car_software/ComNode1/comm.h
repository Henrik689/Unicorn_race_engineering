#include "queue.h"

#ifndef _COMM_H_
#define _COMM_H_

#define TO_CAN	1
#define TO_SD	2
#define TO_XBEE 4

#define XBEE_Q_SIZE 180


struct {
	queue_hdr_t	hdr;                             // must be named "hdr"
	uint8_t		items[XBEE_Q_SIZE+1]; // must be named "items", 1 space wasted
} xbee_q;

static uint8_t start_sequence[] = {255,123,10};

volatile extern uint8_t xbee_seq_index;

volatile extern uint8_t xbee_sending; /* Var to indicate if xbee TX ISR is active */

/* Function to trigger sending of data in xbee out buffer */
void xbee_send_trigger( void );

/* Gammel funktion til at sende status pakke til computer */
void send_status( void );

//FF 7B 0A

#endif //_COMM_H_
