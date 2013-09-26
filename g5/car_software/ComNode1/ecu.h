#include "comm.h"
#include "../lib/data_def.h"

/*****************************************************************************************
*									ECU svar								   		     *
* For at få en streng med data fra ecu'en skal der sendes strengen ecu_data. 			 *
* Svaret afkodes efter tabellen herunder.												 *
+----------------------------------------------------------------------------------------+
|		Navn	|		Beskrivelse			|  Byte nr	| Length | 		gain	| offset |
+----------------------------------------------------------------------------------------+
| fuel pressure	|							|     0		|	2	 |	  0			|	0	 |
| motor oil temp|	Temp i ?				|	  8		|	2	 |	  0			|	0	 |
| water temp. 	|	Temp i ?				|	 46		|	2	 | -150,0/3840	|	120	 |
| rpm 			|	Omdrejninger i sek.		|	 54		|	2	 |	  0,9408	|	0	 |
+----------------------------------------------------------------------------------------+
*/

/* Streng der skal sendes til ecu for at få data */
static const char ecu_data[10] = {0x12,0x34,0x56,0x78,0x17,0x08,0,0,0,0};

typedef struct {
	uint8_t length;
	enum valueID id;
} ECUValueObject;

static const ECUValueObject ECUObjects[] =	{2,fuelPressure,
						2,statusLapCount,
						2,statusInjSum,
						2,lastGearShift,
						2,motorOilTemp,
						2,oilPressure,
						4,statusTime,
						4,statusLapTime,
						2,gearOilTemp,
						2,statusTraction,
						2,statusGas,
						2,statusLambdaV2,
						2,statusCamTrigP1,
						2,statusCamTrigP2,
						2,statusChokerAdd,
						2,statusLambdaPWM,
						10,empty,
						2,waterTemp,
						2,airTemp,
						2,potmeter,
						2,empty,
						2,rpm,
						2,triggerErr,
						2,camAngle1,
						2,camAngle2,
						2,roadSpeed,
						2,mapSensor,
						2,batteryV,
						2,lambdaV,
						4,empty,
						2,load,
						2,empty,
						2,injectorTime,
						2,empty,
						2,ignitionTime,
						2,dwellTime,
						10,empty,
						2,gX,
						2,gY,
						2,gZ,
						9,empty,
						1,motorFlags,
						1,empty,
						1,outBits,
						1,time};


/* Indeholder data fra ECU'en */
extern U8 EcuData[115];

void ecu_data_handler( void );

void ecu_val_to_xbee(uint8_t i, uint8_t j);
