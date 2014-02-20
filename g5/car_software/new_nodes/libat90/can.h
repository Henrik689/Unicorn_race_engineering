/**
* @file can.h
* @brief
*	Used for setting up the CAN subsystem
*	and sending or receiving via the CAN.
*	This header file also contains many
*	driver functions in the form of macros
*/

#ifndef CAN_H
#define CAN_H

//_____ I N C L U D E S ________________________________________________________

#include <stdint.h>
uint8_t can_init(uint8_t mode);  //#include "can_std/can_lib.h" // Inserted so we can compile for now
#include "bitwise.h"
#include <avr/interrupt.h>

#ifndef UINT16_MAX
#define UINT16_MAX	((uint16_t)(~0))
#endif

#ifndef UINT32_MAX
#define UINT32_MAX	((uint32_t)(~0))
#endif

//_____ D E F I N I T I O N S __________________________________________________
//#define MASK_FULL_FILTERING	( (uint16_t){UINT16_MAX}	) //!< Only listen for the specified ID @todo are we sure this should not be uint32_t instead?
//#define MASK_NO_FILTERING	( (uint16_t){0} 			) //!< Listen for all ID's (Eg. a spy node)

#define NB_MOB			( 15		) //!< Number of MOB's
#define NB_DATA_MAX		( 8			) //!< The can can max transmit a payload of 8 uint8_t
#define LAST_MOB_NB		( NB_MOB-1	) //!< Index of the last MOB. This is useful when looping over all MOB's
#define NO_MOB			( 0xFF		)

#define MOB_Tx_ENA  	( 1 << CONMOB0 ) //!< Mask for Enabling Tx on the current MOB
#define MOB_Rx_ENA  	( 2 << CONMOB0 ) //!< Mask for Enabling Rx on the current MOB
#define MOB_Rx_BENA 	( 3 << CONMOB0 ) //!< Mask for Enabling Rx with buffer enabled for the current MOB

#define DLC_MSK     	( (1<<DLC3)|(1<<DLC2)|(1<<DLC1)|(1<<DLC0) 	) //!< Mask for Data Length Coding bits in CANCDMOB
#define MOB_CONMOB_MSK	( (1 << CONMOB1) | (1 << CONMOB0)			) //!< Mask for Configuration MOB bits in CANCDMOB

enum can_int_t {
	CANIT_RX_COMPLETED_DLCW = 	0,	//!< Data length code warning.
	CANIT_RX_COMPLETED = 		1,	//!< Receive completed.
	CANIT_TX_COMPLETED = 		2,	//!< Transmit completed.
	CANIT_ACK_ERROR = 			3,	//!< No detection of the dominant bit in the acknowledge slot.

	/**
	* @brief
	*	The form error results from one or more violations of the fixed
	*	form in the following bit fields: 
	*	+ CRC delimiter
	*	+ Acknowledgment delimiter
	*	+ EOF
	*/
	CANIT_FORM_ERROR = 			4,
	
	/**
	* @brief
	*	The receiver performs a CRC check on every de-stuffed received message 
	*	from the start of frame up to the data field.
	*	If this checking does not match with the de-stuffed CRC field, a CRC error is set.
	*/
	CANIT_CRC_ERROR = 			5,
	CANIT_STUFF_ERROR = 		6,	//!< Detection of more than five consecutive bits with same value.
	CANIT_BIT_ERROR = 			7,	//!< Bit Error (Only in Transmission).
	CANIT_DEFAULT = 			8	//!< This is hopefully temporarily. Should not be possible! Needs testing.
};

enum mob_mode_t {
	MOB_DISABLED,	//!< In this mode, the MOb is disabled.
	MOB_TRANSMIT,	//!< The mob is set in Transmit mode.
	MOB_RECIEVE,	//!< The mob is set in Receive mode.

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

/**
* @brief
*	Different states that CANSTMOB can take.
*	This is very useful for fx. making a conditional
*	switch on the given status of the MOB
*/
enum mob_status_t {
	MOB_NOT_COMPLETED 		= ( 0x00 ),													//!< 0x00
	MOB_TX_COMPLETED 		= ( 1<<TXOK ),												//!< 0x40
	MOB_RX_COMPLETED 		= ( 1<<RXOK ),												//!< 0x20
	MOB_RX_COMPLETED_DLCW 	= ( (1<<RXOK)|(1<<DLCW) ),									//!< 0xA0
	MOB_ACK_ERROR 			= ( 1<<AERR ),												//!< 0x01
	MOB_FORM_ERROR 			= ( 1<<FERR ),												//!< 0x02
	MOB_CRC_ERROR 			= ( 1<<CERR ),												//!< 0x04
	MOB_STUFF_ERROR 		= ( 1<<SERR ),												//!< 0x08
	MOB_BIT_ERROR 			= ( 1<<BERR ),												//!< 0x10
	MOB_PENDING 			= ( (1<<RXOK)|(1<<TXOK) ),									//!< 0x60
	MOB_NOT_REACHED 		= ( (1<<AERR)|(1<<FERR)|(1<<CERR)|(1<<SERR)|(1<<BERR) ),	//!< 0x1F
	MOB_DISABLE 			= ( 0xFF ) 													//!< 0xFF
 };

typedef struct can_msg_t {
	uint8_t mob; 				//!< Message Object to bind to
	uint16_t id; 				//!< Message id / priority
	uint8_t dlc; 				//!< Data Length Code
	uint8_t data[NB_DATA_MAX];	//!< The message payload. Specification states a max length of 8 regardless of dlc 
	enum mob_mode_t mode; 		//!< Denotes the mode that this msg is send or received
} can_msg_t;


//_____ M A C R O S ____________________________________________________________
//!< @name MOB Transmit and Receive
//!< Transmit or receive data on the current MOB
//!< @{
#define MOB_TX_DATA(data, len)			{ 	uint8_t i; \
											for (i = 0; i < len; i++) \
												{ CANMSG = data[i]; } 		} //!< Put data onto the can
#define MOB_RX_DATA(data, len)			{ 	uint8_t i; \
											for (i = 0; i < len; i++) \
												{ data[i] = CANMSG;} 		} //!< Get data from the can
//!< @} ----------


//!< @name CAN status Interrupt register
//!< @{
#define CANSIT_16 					( CANSIT2 + (CANSIT1 << 8) 		) //!< The CANSIT holds information about what mob has fired an interrupt. This combines it into a single 16 bit value.
#define MOB_HAS_PENDING_INT(mob)	( BIT_CHECK(CANSIT_16, (mob))	) //!< Check if the given mob has a pending interrupt.
//!< @} ----------


#define CAN_SET_MOB(mob)				( CANPAGE = ((mob) << 4)	) //!< Set the can the the specified MOB

//@name MOB interrupt
//!< Enable or disable interrupts on the specified MOB
//!< @{
#define CAN_ENABLE_MOB_INTERRUPT(mob)	{	CANIE2 |= ((1 << mob) & 0xff); \
											CANIE1 |= (((1 << mob) >> 8) & 0x7f);	}

#define CAN_DISABLE_MOB_INTERRUPT(mob)	{	CANIE2 &= !((1 << mob) & 0xff); \
											CANIE1 &= !(((1 << mob) >> 8) & 0x7f);	}
//!< @} ----------


//!< @name Can interrupt
//!< enable the can interrupt
//!< @{
#define CAN_SEI()			( BIT_SET(CANGIE, ENIT)	) //!< Enable global CAN interrupts
#define CAN_SET_TX_INT()	( BIT_SET(CANGIE, ENTX)	) //!< Enable CAN Tx interrupts
#define CAN_SET_RX_INT()	( BIT_SET(CANGIE, ENRX)	) //!< Enable CAN Rx interrupts
//!< @} ----------


//!< @name Data Length Code
//!< Getter and setter for the length of data that the given MOB holds
//!< @{
#define MOB_GET_DLC()		( BITMASK_CHECK(CANCDMOB, DLC_MSK) >> DLC0	) //!< Calculates the DLC that is set for the current MOB. @return The DLC sat for the current MOB
#define MOB_SET_DLC(dlc)	( BITMASK_SET(CANCDMOB, dlc)				) //!< Set the DLC for the current MOB
//!< @} ----------


//!< @name MOB ID
//!< @{
#define MOB_SET_STD_ID_10_4(id)			(	((*((uint8_t *)(&(id)) + 1)) << 5) + \
											((*(uint8_t *)(&(id))) >> 3)			)

#define MOB_SET_STD_ID_3_0(id)			(	(*(uint8_t *)(&(id))) << 5 				)

#define MOB_SET_STD_ID(id)				{	CANIDT1 = MOB_SET_STD_ID_10_4((id)); \
											CANIDT2 = MOB_SET_STD_ID_3_0((id)); \
											CANCDMOB &= (~(1<<IDE));				}

#define MOB_SET_STD_MASK_FILTER(mask)	{ 	CANIDM1 = MOB_SET_STD_ID_10_4(mask); \
											CANIDM2 = MOB_SET_STD_ID_3_0( mask);	}

#define MOB_SET_STD_FILTER_FULL()		{	uint32_t __filterMask_ = UINT32_MAX; \
											MOB_SET_STD_MASK_FILTER(__filterMask_); }

#define MOB_SET_STD_FILTER_NONE()		{	uint32_t __filterMask_ = 0; \
											MOB_SET_STD_MASK_FILTER(__filterMask_); }
//!< @} ----------


//!< @name MOB status
//!< @{
#define MOB_CLEAR_STATUS()				{ 	uint8_t  volatile *__i_; \
											for (__i_ =& CANSTMOB; __i_ < &CANSTML; __i_++) \
												{ *__i_= 0x00; }					}
#define MOB_CLEAR_INT_STATUS()			( CANSTMOB = 0x00	) //!< Clears the interrupt status for the current MOB
//!< @} ----------

//!< @name Configuration of Message Object
//!< These bits set the communication to be performed (no initial value after RESET).
//!< These bits are *NOT* cleared once communication is performed.
//!< The user must re-write the configuration to enable new communication.
//!< @{
#define MOB_ABORT()				( BITMASK_CLEAR(CANCDMOB, MOB_CONMOB_MSK) 					) //!< Disable MOB
#define MOB_EN_TX()				{ BIT_CLEAR(CANCDMOB, CONMOB1); BIT_SET(CANCDMOB, CONMOB0); } //!< Enable MOB Transmission
#define MOB_EN_RX()				{ BIT_SET(CANCDMOB, CONMOB1); BIT_CLEAR(CANCDMOB, CONMOB0); } //!< Enable MOB Reception
#define MOB_EN_FRM_BUFF_RX()	( BITMASK_SET(CANCDMOB, MOB_CONMOB_MSK) 					) //!< Enable MOB Frame Buffer Reception
//!< @} ----------


#define MOB_CONFIG_TX()					{	MOB_ABORT(); \
											BITMASK_SET(CANCDMOB, MOB_Tx_ENA);	}
#define MOB_CONFIG_RX()					{	MOB_ABORT(); \
											BITMASK_SET(CANCDMOB, MOB_Rx_ENA);	}
#define MOB_CONFIG_RX_BUFFER()			(	BITMASK_SET(CANCDMOB, MOB_Rx_BENA)	)
//----------

// ------------ Old wrappers
#define CAN_INIT()		( can_init(0) )
#define CAN_INIT_ALL()	{ CAN_INIT(); CAN_SEI(); CAN_SET_RX_INT(); CAN_SET_TX_INT();}
#define CAN_INIT_RX()	{ CAN_INIT(); CAN_SEI(); CAN_SET_RX_INT(); 					}
#define CAN_INIT_TX()	{ CAN_INIT(); CAN_SEI(); CAN_SET_TX_INT(); 					}

#define Can_config_tx()        { MOB_ABORT(); CANCDMOB |= (1 << CONMOB0); }
#define Can_config_rx()        { MOB_ABORT(); CANCDMOB |= (2 << CONMOB0); }
#define Can_config_rx_buffer() {              CANCDMOB |= (3 << CONMOB0); }

#define Can_set_mob_int(mob)	{ CANIE2 |= ((1<<mob) & 0xff); \
				  CANIE1 |= (((1<<mob)>>8) & 0x7f); }
#define Can_unset_mob_int(mob)	{ CANIE2 &= !((1<<mob) & 0xff); \
				  CANIE1 &= !(((1<<mob)>>8) & 0x7f); }
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
