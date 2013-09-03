/*****************************************************************************************
*									ECU svar								   		     *
* For at få en streng med data fra ecu'en skal der sendes strengen ecu_data. 			 *
* Svaret afkodes efter tabellen herunder.												 *
+----------------------------------------------------------------------------------------+
|		Navn	|		Beskrivelse		|	Byte nr	|	Length	|gain| offset |
+----------------------------------------------------------------------------------------+
| fuel pressure		|					|	0	|	2	| 0|0|
| motor oil temp	|	Temp i ?			|	  8	|	2	 |0|0|
| water temp. 		|	Temp i ?			|	 46	|	2	 | -150,0/3840|120|
| rpm 			|	Omdrejninger i sek.		|	 54	|	2	 | 0,9408|0 |
+----------------------------------------------------------------------------------------+
*/

/* Streng der skal sendes til ecu for at få data */
static const char ecu_data[10] = {0x12,0x34,0x56,0x78,0x17,0x08,0,0,0,0};

/* Indeholder data fra ECU'en */
U8 EcuData[228]; 

/* Motor oil temp */
#define MOTOR_OIL_TEMP_START 20
#define MOTOR_OIL_TEMP_LENGTH 2
#define MOTOR_OIL_TEMP_GAIN 1
#define MOTOR_OIL_TEMP_OFFSET 0

/* WATER temp */
#define WATER_TEMP_START 46
#define WATER_TEMP_LENGTH 2
#define WATER_TEMP_GAIN -0.0390625
#define WATER_TEMP_OFFSET 120

/* RPM */
#define RPMSTART 54
#define RPMLENGTH 2
#define RPMGAIN 0.9408
#define RPMOFFSET 0

/* Road speed */
#define SPEEDSTART 62
#define SPEEDLENGTH 2
#define SPEEDGAIN 0.01
#define SPEEDOFFSET 0

/* potmeter */
#define POTSTART 50
#define POTLENGTH 2
#define POTGAIN 0.0510
#define POTOFFSET -31.4

/* Map */
#define MAPSTART 64
#define MAPLENGTH 2
#define MAPGAIN 1.0/800/5*3000
#define MAPOFFSET 0

/* Lambda */
#define LAMBDASTART 68
#define LAMBDALENGTH 1
#define LAMBDAGAIN -14.7*0.6/3840
#define LAMBDAOFFSET 0.7*14.7
