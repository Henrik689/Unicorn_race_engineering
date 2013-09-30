#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "./canparser.h"

#define ARR_LEN(x)  (sizeof(x) / sizeof(x[0]))
#define MIN(x, y)	((x < y) ? x : y)
#define MAX(x, y)	((x > y) ? x : y)

static double roundn(double value, int to){
	//double places = pow(10.0, to);
	//return round(value * places) / places;
	return value;
}

// As C does not support any form of lambda functions
// each convertion function must be declared here and
// then added to the list(array) of sensor configs 

static float stdConv(int x, int rounddec){
	return roundn((x*1+0), rounddec);
}

static float StatusLambdaV2Conv(int x, int rounddec){
	float val;
	if(x > 32768){
		x = -(65535-x);
	}
	val = 70-x/64;
	return roundn(val, rounddec);
}

static float airAndWaterTempConv(int x, int rounddec){
	float val = (x * (-150.0/3840) + 120);
	return roundn(val, rounddec);
}

static float potmeterConvert(int x, int rounddec){
	float val = ((x-336)/26.9);
	return roundn(val, rounddec);
}

static float rpmConv(int x, int rounddec){
	float val = (x*0.9408);
	return roundn(val, rounddec);
}

static float mBarConv(int x, int rounddec){
	float val = (x*0.75);
	return roundn(val, rounddec);
}

static float batteryConv(int x, int rounddec){
	float val = (x*(1/210)+0);
	return roundn(val, rounddec);
}

static float StatusLambdaV2Conv2(int x, int rounddec){
	float val;
	if(x > 32768){
		x = -(65535-x);
	}
	val = 70-x/64;
	return roundn(val/100, rounddec);
}

static float InjectorAndIgnitionTimeConv(int x, int rounddec){
	float val = -0.75*x+120;
	return roundn(val, rounddec);
}

static float GXGYGZconv(int x, int rounddec){
	float val;
	if(x > 32768){
		x = -(65535 - x);
	}
	val = x * (1/16384);
	return roundn(val, rounddec);
}

static float gearboardTempConv(int x, int rounddec){
	float val;
	double resistance = ((10240000/(1024 - x)) - 10000);
	double temp = log(resistance);
	temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
	val = temp - 273.15;
	return roundn(val, rounddec);
}

static float waterInOutletTemoConv(int x, int rounddec){
	float val = 127.5 * exp(-0.003286*x);
	return roundn(val, rounddec);
}

static float gearNeutralConv(int x, int rounddec){
	float val;
	if( x > 100){
		x=1;
	}
	val = x;
	return roundn(val, rounddec);
}

static config_t config[] = {
	{"Empty", 0, 2, 0, 9999, 0, &stdConv},
	{"Fuel Press.", 1, 2, 16, 9999, 0, &stdConv},
	{"StatusLapCount", 2, 2, 16, 9999, 0, &stdConv},
	{"StatusInjSum", 3, 2, 16, 9999, 0, &stdConv},
	{"LastGearShift", 4, 2, 16, 9999, 0, &stdConv},
	{"MotorOilTemp", 5, 2, 16, 9999, 0, &stdConv},
	{"OilPressure", 6, 2, 16, 9999, 0, &stdConv},
	{"StatusTime", 7, 2, 16, 9999, 0, &stdConv},
	{"StatusLapTime", 8, 2, 16, 9999, 0, &stdConv},
	{"GearOilTemp", 9, 2, 16, 9999, 0, &stdConv},
	{"StatusTraction", 10, 2, 16, 9999, 0, &stdConv},
	{"StatusGas", 11, 2, 16, 9999, 0, &stdConv},
	{"StatusLambdaV2", 12, 2, 16, 9999, 0, &StatusLambdaV2Conv},
	{"StatusCamTrigP1", 13, 2, 16, 9999, 0, &stdConv},
	{"StatusCamTrigP2", 14, 2, 16, 9999, 0, &stdConv},
	{"StatusChokerAdd", 15, 2, 16, 9999, 0, &stdConv},
	{"StatusLambdaPWM", 16, 2, 16, 9999, 0, &stdConv},
	{"WaterMotor temp.", 17, 0, 16, 150, 0, &airAndWaterTempConv},
	{"ManifoldAir temp.", 18, 1, 16, 120, 0, &airAndWaterTempConv},
	{"Potmeter (0-100%)", 19, 2, 16, 100, 0, &potmeterConvert},
	{"RPM", 20, 0, 16, 15000, 0, &rpmConv},
	{"TriggerErr", 21, 2, 16, 9999, 0, &stdConv},
	{"CamAngle1", 22, 2, 16, 9999, 0, &stdConv},
	{"CamAngle2", 23, 2, 16, 9999, 0, &stdConv},
	{"RoadSpeed (km/h)", 24, 2, 16, 9999, 0, &stdConv},
	{"Manifold press. (mBar)", 25, 2, 16, 1300, 0, &mBarConv},
	{"Batt. volt", 26, 1, 16, 20, 0, &batteryConv},
	{"Lambda (<1 => Rich)", 27, 2, 16, 2, 0, &StatusLambdaV2Conv2},
	{"Load", 28, 2, 16, 9999, 0, &stdConv},
	{"InjectorTime", 29, 2, 16, 9999, 0, &InjectorAndIgnitionTimeConv},
	{"IgnitionTime", 29, 2, 16, 9999, 0, &InjectorAndIgnitionTimeConv},
	{"DwellTime", 31, 2, 16, 9999, 0, &stdConv},
	{"GX", 32, 2, 16, 2, -2, &GXGYGZconv},
	{"GY", 33, 2, 16, 2, -2, &GXGYGZconv},
	{"GZ", 34, 2, 16, 2, -2, &GXGYGZconv},
	{"MotorFlags", 35, 2, 8, 9999, 0, &stdConv},
	{"OutBits", 36, 2, 8, 9999, 0, &stdConv},
	{"Time", 37, 2, 8, 9999, 0, &stdConv},
	{"GearChange", 38, 2, 8, 9999, 0, &stdConv},
	{"FWheelL", 39, 2, 16, 9999, 0, &stdConv},
	{"FWheelL", 40, 2, 16, 9999, 0, &stdConv},
	{"BWheelL", 41, 2, 16, 9999, 0, &stdConv},
	{"BWheelR", 42, 2, 16, 9999, 0, &stdConv},

	// id gab
	{"GearBoard temp.", 52, 2, 16, 50, 0, &gearboardTempConv},

	// id gab
	{"OilPress (0 = Low)", 56, 2, 16, 1024, 0, &stdConv},
	{"WaterInlet temp.", 57, 0, 16, 150, 0, &waterInOutletTemoConv},
	{"WaterOutlet temp.", 58, 0, 16, 150, 0, &waterInOutletTemoConv},
	{"GPS-sattelites", 59, 2, 8, 10, 0, &stdConv},
	{"GPS-speed (km/h)", 60, 2, 16, 120, 0, &stdConv},
	{"GPS-time-h", 61, 2, 16, 25, 0, &stdConv},
	{"GPS-time-m", 62, 2, 16, 61, 0, &stdConv},
	{"GPS-time-s", 63, 2, 16, 61, 0, &stdConv},
	{"GearNeutral", 64, 2, 16, 1, 0, &gearNeutralConv},
	{"GearEst", 65, 2, 16, 7, 0, &stdConv},
	{"Debug", 66, 2, 16, 9999, 0, &stdConv},
	{"ValueIdLength", 67, 2, 16, 9999, 0, &stdConv},
};

static int getConfigFromID(int id){
	int i = 0;
	for (i = 0; i < ARR_LEN(config); ++i){
		if(id == config[i].id){
			return i; // index in the config list
		}
	}
	return -1; // No ID was found
}

int parseNext(uint8_t dataByte, sensor_t *sensor){
	const uint8_t startSequence[] = {255, 123, 10};

	static int package_start_counter = 0; // how many of the start sequence bytes have we seen
	static int package_start = 0;
	static int bytesToRead = -1;
	static int valOut = 0;

	static int confIndex = -1;

	if((package_start_counter == 0) && (dataByte == startSequence[0]))
		package_start_counter = 1;
	else if((package_start_counter == 1) && (dataByte == startSequence[1]))
		package_start_counter = 2;
	else if((package_start_counter == 2) && (dataByte == startSequence[2])){
		package_start_counter = 0;
		package_start = 1;
		return PARSER_NEEDNEXT; // we are ready for next byte
	}

	if(package_start){
		// Reset
		package_start = 0;
		bytesToRead = -1;
		valOut = 0;

		confIndex = getConfigFromID(dataByte);
		if(confIndex == -1){
			// Invalid id found at currByte !
			return -(int)dataByte; // return the negative value as all other return codes are unsigned
		}
		bytesToRead = config[confIndex].datalength/8;
		return PARSER_NEEDNEXT; // Ready for next byte
	}

	if(bytesToRead > 0){
		valOut = valOut + (dataByte << (8*(bytesToRead-1)));
		bytesToRead -= 1; // We have read a byte so we obviously have one less to read
		return PARSER_NEEDNEXT; // We have read and added the byte, So ready for the next
	}

	if(bytesToRead == 0){
		const char* name = config[confIndex].name;
		float value = config[confIndex].conv(valOut, config[confIndex].rounddec);
		value = MIN(value, config[confIndex].max);
		value = MAX(value, config[confIndex].min);

		// Copy the value into the sensor object
		sensor->name = name;
		sensor->id = config[confIndex].id;
		sensor->confIndex = confIndex;
		sensor->value = value;

		// Reset 
		bytesToRead = -1;
		valOut = 0;

		// Are the a next data byte?
		confIndex = getConfigFromID(dataByte);
		if(confIndex == -1){
			// No more data
			return PARSER_FOUND;
		}

		bytesToRead = config[confIndex].datalength/8;
		return PARSER_FOUND;
		
	}
	return PARSER_NOTHINGTODO;
}



