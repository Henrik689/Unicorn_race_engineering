#ifndef _CAN_LOG
#define _CAN_LOG

extern uint8_t sample_time;

#define STD_DATA 133//0x90	

/* Definition of std CAN message data types */
#define WhlSpdB 0x01	/* Wheel speed back */
#define WhlSpdF 0x02	/* Wheel speed front */

#define WatTmp 		0x03	/* Water-temp */
#define WatTmpLen	2


/* Definition of can messages */
typedef struct{
	uint8_t id;
	uint8_t dlc;
} dataPackage;

static dataPackage watPack = {WatTmp,2};

#endif // _CAN_LOG
