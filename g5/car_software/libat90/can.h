#ifndef CAN_H
#define CAN_H

//_____ I N C L U D E S ________________________________________________________

#include <stdint.h>
//#include "can_std/can_lib.h"
#include "bitwise.h"


//_____ D E F I N I T I O N S __________________________________________________

#define MASK_FULL_FILTERING	( (uint16_t){UINT16_MAX}	) //!< Only listen for the specified ID
#define MASK_NO_FILTERING	( (uint16_t){0} 			) //!< Listen for all ID's (Eg a spynode)

#define NB_MOB			( 15		) //!< Number of MOB's
#define NB_DATA_MAX		( 8			) //!< The can can max transmit a payload of 8 uint8_t
#define LAST_MOB_NB		( NB_MOB-1	) //!< Index of the last MOB. This is usefull when looping over all MOB's
#define NO_MOB			( 0xFF		)

#define MOB_Tx_ENA  	1
#define MOB_Rx_ENA  	2
#define MOB_Rx_BENA 	3

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
	MOB_DISABLED,					//!< In this mode, the MOb is “free”.
	MOB_TRANSMIT,					//!< The mob is set in transmit mode.
	MOB_RECIEVE,					//!< The mob is set in transcieve mode.

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
	uint8_t mob; 					//!< Message Object to bind to
	uint16_t id; 					//!< Message id / priority
	uint8_t dlc; 					//!< Data Length Code
	uint8_t data[NB_DATA_MAX];		//!< The message payload. Specification states a max length of 8 regardless of dlc 
	enum mob_mode_t mode; 			//!< Tx or Rx
} can_msg_t;


//_____ M A C R O S ____________________________________________________________

#define MOB_TX_DATA(data, len)			{ 	uint8_t i; \
											for (i = 0; i < len; i++) \
												{ CANMSG = data[i]; } 		} //!< Put data onto the can
#define MOB_RX_DATA(data, len)			{ 	uint8_t i; \
											for (i = 0; i < len; i++) \
												{ data[i] = CANMSG;} 		} //!< Get data from the can
//----------
#define CAN_READ_CANSIT 				( 	CANSIT2 + (CANSIT1 << 8) 			) //!< The CANSIT holds infomation about what mob has fired an interrupt. This combines it into a single 16bit variable.
#define MOB_HAS_PENDING_INT(mob)		( 	BIT_CHECK(CAN_READ_CANSIT, (mob))	) //!< Check if the given mob has a pending interrupt.
//----------
#define CAN_SET_MOB(mob)				{	CANPAGE = ((mob) << 4);					}

#define CAN_ENABLE_MOB_INTERRUPT(mob)	{	CANIE2 |= ((1 << mob) & 0xff); \
											CANIE1 |= (((1 << mob) >> 8) & 0x7f);	}

#define CAN_DISABLE_MOB_INTERRUPT(mob)	{	CANIE2 &= !((1 << mob) & 0xff); \
											CANIE1 &= !(((1 << mob) >> 8) & 0x7f);	}
#define CAN_SEI()						(	CANGIE |= (1 << ENIT)					)
#define CAN_SET_TX_INT()				( 	CANGIE |= (1 << ENTX)					)
#define CAN_SET_RX_INT()				( 	CANGIE |= (1 << ENRX)					)

#define MOB_CONMOB_MSK					(	(1 << CONMOB1) | (1 << CONMOB0)			) //! MaSK for CONfiguration MOb

#define MOB_SET_STD_ID_10_4(id)			(	((*((uint8_t *)(&(id)) + 1)) << 5) + \
											((*(uint8_t *)(&(id))) >> 3)			)

#define MOB_SET_STD_ID_3_0(id)			(	(*(uint8_t *)(&(id))) <<5 				)

#define MOB_GET_DLC()					(	(CANCDMOB & DLC_MSK) >> DLC				)
#define MOB_CLEAR_INT_STATUS()			{	CANSTMOB=0x00;							}
#define MOB_SET_DLC(dlc)				(	CANCDMOB |= (dlc)						)

#define MOB_SET_STD_ID(id)				{	CANIDT1 = MOB_SET_STD_ID_10_4(id); \
											CANIDT2 = MOB_SET_STD_ID_3_0(id); \
											CANCDMOB &= (~(1 << IDE));				}

#define MOB_SET_STD_MASK_FILTER(mask)	{ 	CANIDM1 = MOB_SET_STD_ID_10_4(mask); \
											CANIDM2 = MOB_SET_STD_ID_3_0( mask);	}

#define MOB_CLEAR_STATUS()				{ 	uint8_t  volatile *__i_; \
											for (__i_ =& CANSTMOB; __i_ < &CANSTML; __i_++) \
												{ *__i_= 0x00; }					}
#define MOB_ABORT()						( 	CANCDMOB &= \
											~((1 << CONMOB1)|(1 << CONMOB0))		)
#define MOB_CONFIG_TX()					{	MOB_ABORT(); \
											CANCDMOB |= (MOB_Tx_ENA << CONMOB);		}
#define MOB_CONFIG_RX()					{	MOB_ABORT(); \
											CANCDMOB |= (MOB_Rx_ENA << CONMOB);		}
#define MOB_CONFIG_RX_BUFFER()			{	CANCDMOB |= (MOB_Rx_BENA << CONMOB);	}

// ------------ Old wrappers
#define CAN_INIT()		( can_init(0) )
#define CAN_INIT_ALL()	{ CAN_INIT(); CAN_SEI(); CAN_SET_RX_INT(); CAN_SET_TX_INT();}
#define CAN_INIT_RX()	{ CAN_INIT(); CAN_SEI(); CAN_SET_RX_INT(); 					}
#define CAN_INIT_TX()	{ CAN_INIT(); CAN_SEI(); CAN_SET_TX_INT(); 					}
// ------------- End old wrappers


//_____ D E C L A R A T I O N S ________________________________________________

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
