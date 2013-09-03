#include "config.h"
#include "can_std/can_lib.h"
#include "can.h"
#include <util/delay.h>
#include "../lib/can_defs.h"
#include "../lib/data_def.h"
#include "uart.h"
#include "ecu.h"
#include "error.h"
#include "comm.h"
#include "queue.h"
#include "log.h"
#include <util/atomic.h>

int RecIndex = 0;	// Bruges til at tælle hvor mange bytes der er modtaget fra ECU'en
uint8_t RecCount = 1;
uint8_t RecJ = 0;
uint8_t RecXbeeSend = 0;
uint8_t RecCanSend = 0;
uint8_t RecToSd = 0;
uint8_t SdIdNum = 0;
uint8_t CanDataIndex = 0;
uint8_t CanSendData[8];
int testvar = 0;	// Tmp var for at køre TIMER0_COMP_vect langsommere

volatile uint8_t xbee_sending = 0;
volatile uint8_t nextId = 0;

/* Funktion der sender data request til ECU */
ISR(TIMER0_COMP_vect)
{
	int i;
	testvar++;
	if (testvar == 8) {
		RecCount = 1;
		RecJ = 0;
		RecXbeeSend= 0;
		RecCanSend = 0;
		RecToSd = 0;
		if (RecIndex != 114 && EcuErrorTmp < 5) { // Test for fejl
			EcuErrorTmp++;
			if (EcuErrorTmp == 5) {
				EcuCommError = 1;
			}
		} else if (EcuErrorTmp > 0 && RecIndex == 114) {
			EcuErrorTmp--;
			EcuCommError = 0;
		}
		testvar=0;
		RecIndex = 0;
		for (i=0;i<=9;i++) {		// Send data req streng
			USART0_Transmit(ecu_data[i]); 
		}
	}
}

/* Funktion til at læse data fra ECU */
ISR(USART0_RX_vect)
{
	if (RecIndex <= 114) {			// Der skal modtages 228 bytes fra ECU'en
		EcuData[RecIndex] = UDR0;	// Gem data fra ECU
		RecIndex++;

		if (RecCount == RecIndex) {
			RecCount += ECUObjects[RecJ].length;
			if (RecCanSend == 1) {
				can_send_non_blocking(rpm_msgid, &CanSendData[0], 3);
			}
			if (valueObjects[ECUObjects[RecJ].id].action & (TO_XBEE | TO_SD | TO_CAN) ) {
				
				RecXbeeSend = 0;
				RecCanSend = 0;
				RecToSd = 0;

				/* Value to xbee? */				
				if (valueObjects[ECUObjects[RecJ].id].action & TO_XBEE) {
				    	//_delay_us(10);
					RecXbeeSend = 1;
				    	ATOMIC_BLOCK(ATOMIC_FORCEON)
				    	{
						QUEUE_PUT(xbee_q, ECUObjects[RecJ].id);
					}
				}
				
				/* Value to CAN? */
				if (valueObjects[ECUObjects[RecJ].id].action & TO_CAN) {
					/* TODO
					 * Insert call to val_to_CAN() when the function is
					 * made
					 */
					CanDataIndex = 0;
					CanSendData[CanDataIndex++] = ECUObjects[RecJ].id;
					RecCanSend = 1;
				}

				/* Value to SD? */
				if (valueObjects[ECUObjects[RecJ].id].action & TO_SD) {
					/* TODO
					 * Insert call to val_to_SD() when the function is
					 * made
					 */
					RecToSd = 1;
					SdIdNum = RecJ;
				}
			} else {
				RecCanSend = 0;
				RecXbeeSend = 0;
				RecToSd = 0;
			}

			RecJ++;
		}

		if (RecXbeeSend == 1) {
		    	ATOMIC_BLOCK(ATOMIC_FORCEON)
		    	{
				QUEUE_PUT(xbee_q, EcuData[RecIndex-1]);
			}
		}
		if (RecCanSend == 1) {
			CanSendData[CanDataIndex++] = EcuData[RecIndex-1];
		}
		if (RecToSd == 1) {
			if ((RecCount-1) == RecIndex) {
				sd_log_write( (uint8_t *)&ECUObjects[SdIdNum].id, 1);
				sd_log_write(&EcuData[RecIndex-2], 2);
			}
		}

	}
}

/* ISR to take care of xbee data sending */
ISR(USART1_UDRE_vect)
{
	uint8_t tmp;

	/* if xbee_seq_index is less than 3 - send package start sequence */
	if (xbee_seq_index<3) {
		UDR1 = start_sequence[xbee_seq_index++]; // Send the last two byte in the start sequence
	}else{
		QUEUE_GET(xbee_q, tmp);		// pop value from queue
		if (nextId == 0) {		// Check if at start of data package
			nextId = valueObjects[tmp].length+8;
		}
		UDR1 = tmp;			// Send value
		nextId = nextId - 8;
	    	if (nextId == 0) {		// Data package send, check number of bytes in the queue
			if (QUEUE_GET_NUM_ELE(xbee_q) <= 4) {	// If less than 4 bytes remain stop sending
				xbee_sending = 0;
				Usart1_tx_ei_dis(); // Done sending - disable tx interrupt 
			}
		}
	}
}
