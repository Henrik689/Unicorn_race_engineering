#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include "can_std/can_lib.h"
#include "bitwise.h"

// ------------ Old wrappers
#define CAN_SEI()		( Can_sei()			)
#define CAN_RX_SEI()	( Can_set_rx_int() 	)
#define CAN_TX_SEI()	( Can_set_tx_int() 	)

#define CAN_INIT()		( can_init(0) )
#define CAN_INIT_ALL()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); CAN_TX_SEI(); 	}
#define CAN_INIT_RX()	{ CAN_INIT(); CAN_SEI(); CAN_RX_SEI(); 					}
#define CAN_INIT_TX()	{ CAN_INIT(); CAN_SEI(); CAN_TX_SEI(); 					}
// ------------- End old wrappers

#define MASK_FULL_FILTERING	( (uint16_t){UINT16_MAX}	) //!< Only listen for the specified ID
#define MASK_NO_FILTERING	( (uint16_t){0} 			) //!< Listen for all ID's (Eg a spynode)

#define CAN_TX_DATA(data, len)	{ uint8_t i; for(i=0; i<len; i++){CANMSG = data[i];} } //!< Put data onto the can
#define CAN_RX_DATA(data, len)	{ uint8_t i; for(i=0; i<len; i++){data[i] = CANMSG;} } //!< Get data from the can

#define READ_CANSIT 				( CANSIT2 + (CANSIT1 << 8) 		) //!< The CANSIT holds infomation about what mob has fired an interrupt. This combines it into a single 16bit variable.
#define MOB_HAS_PENDING_INT(mob)	( BIT_CHECK(READ_CANSIT, (mob)) ) //!< Check if the given mob has a pending interrupt.

// -------------- New wrappers
#define CAN_SET_MOB(mob)				( Can_set_mob((mob)) 		)
#define CAN_ENABLE_MOB_INTERRUPT(mob)	( Can_set_mob_int((mob)) 	)
#define CAN_DISABLE_MOB_INTERRUPT(mob)	( Can_unset_mob_int((mob)) 	)
#define CAN_CLEAR_STATUS_MOB()			( Can_clear_status_mob() 	)
#define CAN_GET_DLC()					( Can_get_dlc() 			)

#define MOB_SET_DLC(dlc)				( Can_set_dlc((dlc)) 		)
#define MOB_SET_STD_ID(id)				( Can_set_std_id((id)) 		)
#define MOB_SET_STD_MASK_FILTER(mask)	( Can_set_std_msk((mask)) 	)
#define MOB_CLEAR_STATUS()				( Can_clear_mob()			)
#define MOB_ABORT()						( Can_mob_abort() 			)
// -------------- End New Wrappers

enum can_int_t {
	CANIT_RX_COMPLETED_DLCW = 0,	//!< Data length code warning.
	CANIT_RX_COMPLETED = 1,			//!< Receive completed.
	CANIT_TX_COMPLETED = 2,			//!< Transmit completed.
	CANIT_ACK_ERROR = 3,			//!< No detection of the dominant bit in the acknowledge slot.

	/**
	* @brief
	*	The form error results from one or more violations of the fixed
	*	form in the following bit fields: 
	*	+ CRC delimiter
	*	+ Acknowledgment delimiter
	*	+ EOF
	*/
	CANIT_FORM_ERROR = 4,
	
	/**
	* @brief
	*	The receiver performs a CRC check on every de-stuffed received message 
	*	from the start of frame up to the data field.
	*	If this checking does not match with the de-stuffed CRC field, a CRC error is set.
	*/
	CANIT_CRC_ERROR = 5,
	CANIT_STUFF_ERROR = 6,			//!< Detection of more than five consecutive bits with same value.
	CANIT_BIT_ERROR = 7,			//!< Bit Error (Only in Transmission).
	CANIT_DEFAULT = 8				//!< This is hopefully temporarily. Should not be possible! Needs testing.
};

enum mob_mode_t {
	MOB_DISABLED,			//!< In this mode, the MOb is “free”.
	MOB_TRANSMIT,			//!< The mob is set in transmit mode.
	MOB_RECIEVE,			//!< The mob is set in transcieve mode.

	/**
	* @brief
	*	A reply (data frame) to a remote frame can be automatically
	*	sent after reception of the expected remote frame.
	*/
	MOB_AUTOMATIC_REPLY,

	/**
	* @brief
	*	This mode is useful to receive multi frames. The priority between MObs offers a management for
	*	these incoming frames. One set MObs (including non-consecutive MObs) is created when the
	*	MObs are set in this mode. Due to the mode setting, only one set is possible. A frame buffer
	*	completed flag (or interrupt) - BXOK - will rise only when all the MObs of the set will have
	*	received their dedicated CAN frame.
	*/
	MOB_FRAME_BUFF_RECEIVE
};

typedef struct can_msg_t {
	uint8_t mob; 				//!< Message Object to bind to
	uint16_t id; 				//!< Message id / priority
	uint8_t dlc; 				//!< Data Length Code
	uint8_t data[NB_DATA_MAX];	//!< The message payload. Specification states a max length of 8 regardless of dlc 

	enum mob_mode_t mode; 		//!< Tx or Rx
} can_msg_t;

void set_canit_callback(enum can_int_t interrupt, void (*callback)(uint8_t mob));
int can_setup(can_msg_t *msg);
int can_receive(can_msg_t *msg);
int can_send(can_msg_t *msg);

void setup_mob_rx(uint8_t mob, uint16_t id, uint8_t dlc);
void setup_mob_tx(uint8_t mob, uint16_t id, uint8_t *data, uint8_t dlc);
void set_mob_id(uint8_t mob, uint16_t id);
void clear_mob_status(uint8_t mob);
void set_mob_dlc(uint8_t mob, uint8_t dlc);
void set_mob_mask(uint8_t mob, uint16_t mask);
void set_mob_mode(uint8_t mob, enum mob_mode_t mode);
void set_data_reg(uint8_t mob, uint8_t *data, uint8_t dlc);

#endif /* CAN_H */
