#ifndef _DATA_DEF_
#define _DATA_DEF_

#define TO_CAN	1
#define TO_SD	2
#define TO_XBEE 4

enum valueID {
	empty,			// 0
	fuelPressure,		// 1
	statusLapCount,		// 2
	statusInjSum,		// 3
	lastGearShift,		// 4
	motorOilTemp,		// 5
	oilPressure,		// 6
	statusTime,		// 7
	statusLapTime,		// 8
	gearOilTemp,		// 9
	statusTraction,		// 10
	statusGas,		// 11
	statusLambdaV2,		// 12
	statusCamTrigP1,	// 13
	statusCamTrigP2,	// 14
	statusChokerAdd,	// 15
	statusLambdaPWM,	// 16
	waterTemp,		// 17
	airTemp,		// 18
	potmeter,		// 19
	rpm,			// 20
	triggerErr,		// 21
	camAngle1,		// 22
	camAngle2,		// 23
	roadSpeed,		// 24
	mapSensor,		// 25
	batteryV,		// 26
	lambdaV,		// 27
	load,			// 28
	injectorTime,		// 29
	ignitionTime,		// 30
	dwellTime,		// 31
	gX,			// 32
	gY,			// 33
	gZ,			// 34
	motorFlags,		// 35
	outBits,		// 36
	time,			// 37
	gear,			// 38 // GEARUPBUT 0b0001, GEARDOWNBUT 0b0010, GEARNEUBUT 0b0100
	FWheelL,		// 39
	FWheelR,		// 40
	BWheelL,		// 41
	BWheelR,		// 42
	N5ADC0,			// 43
	N5ADC1,			// 44
	N5ADC2,			// 45
	N5ADC3,			// 46
	N5ADC4,			// 47
	N5ADC5,			// 48
	N5ADC6,			// 49
	N5ADC7,			// 50
	N6ADC0,			// 51
	N6ADC1,			// 52
	N6ADC2,			// 53
	N6ADC3,			// 54
	N6ADC4,			// 55
	N6ADC5,			// 56
	N6ADC6,			// 57
	N6ADC7,			// 58
	gpssat,			// 59
	gpsspeed,		// 60
	gpstime_h,		// 61
	gpstime_m,		// 62
	gpstime_s,		// 63
	GearNeutral,	// 64
	GearEst,		// 65
	debug,			// 66
	ValueIdLength	// 67
};

typedef struct {
	uint8_t length;		/* Length of data in bits eg. 8, 10 or some other length */
	uint8_t action;		/* bit 0 = CANSend, bit 1 = SDSave, bit 2 = Xbee */
} valueObject;

/* Remember to set a queue size in comm.h large enough to hold enough data 
 * based on how many values from the ecu that are send via xbee */
static const valueObject valueObjects[] =	{0, 0,	// empty 
						16, 0,	// fuelPressure
						16, 0,	// statusLapCount
						16, 0,	// statusInjSum
						16, 0,	// lastGearShift
						16, 0,	// motorOilTemp
						16, 0,	// oilPressure
						16, 0,	// statusTime
						16, 0,	// statusLapTime
						16, 0,	// gearOilTemp
						16, 0,	// statusTraction
						16, 0,	// statusGas
						16, 0,	// statusLambdaV2
						16, 0,	// statusCamTrigP1
						16, 0,	// statusCamTrigP2
						16, 0,	// statusChokerAdd
						16, 0,	// statusLambdaPWM
						16, TO_SD | TO_CAN | TO_XBEE  ,	// waterTemp
						16, TO_SD | TO_XBEE ,	// airTemp
						16, TO_SD | TO_XBEE , // potmeter
						16, TO_SD | TO_CAN | TO_XBEE  ,	// rpm
						16, 0,	// triggerErr
						16, 0,	// camAngle1
						16, 0,	// camAngle2
						16, TO_SD | TO_XBEE,	// roadSpeed
						16, TO_SD | TO_XBEE,	// mapSensor
						16, TO_SD | TO_CAN | TO_XBEE ,	// batteryV
						16, TO_SD | TO_XBEE,	// lambdaV
						16, 0,	// load
						16, 0,	// injectorTime
						16, 0,	// ignitionTime
						16, 0,	// dwellTime
						16, TO_SD | TO_XBEE,	// gX
						16, TO_SD | TO_XBEE,	// gY
						16, TO_SD | TO_XBEE,	// gZ
						8, 0,	// motorFlags
						8, 0,	// outBits
						8, 0,	// time
						8, TO_XBEE,	// gear
						16, 0, // FWheelL
						16, 0, // FWheelR
						16, 0, // BWheelL
						16, 0, // BWheelR
						16, 0, // Node 5 ADC0
						16, 0, // Node 5 ADC1
						16, 0, // Node 5 ADC2
						16, 0, // Node 5 ADC3
						16, 0, // Node 5 ADC4
						16, 0, // Node 5 ADC5
						16, 0, // Node 5 ADC6
						16, 0, // Node 5 ADC7
						16, 0, // Node 6 ADC0
						16, TO_SD | TO_XBEE, // Node 6 ADC1 Gearbox temp.
						16, 0, // Node 6 ADC2
						16, 0, // Node 6 ADC3
						16, 0, // Node 6 ADC4
						16, TO_SD | TO_XBEE, // Node 6 ADC5 Engine oil press.
						16, TO_SD | TO_XBEE, // Node 6 ADC6 Cooler inlet temp.
						16, TO_SD | TO_XBEE, // Node 6 ADC7 Cooler outlet temp.
						8,  TO_XBEE, // gpssat
						16, TO_SD | TO_XBEE, // gpsspeed
						16, 0, // gpstime_h
						16, 0, // gpstime_m
						16, 0, // gpstime_s
						16, TO_XBEE, // GearNeutral
						16, TO_XBEE, // GearEst
						16, TO_XBEE}; // debug

#endif // _DATA_DEF_
