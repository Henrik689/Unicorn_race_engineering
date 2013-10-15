#ifndef CANPARSER_H
#define CANPARSER_H

#include <stdint.h>
#include <stdbool.h>

#define ARR_LEN(x)  (sizeof(x) / sizeof(x[0]))

typedef struct config_t {
   const char *name;			// Human readable name
   int id;						// Can ID
   int rounddec;				// Round to this many decimals
   int datalength;				// Length in bits
   int max;						// Maximum expected value
   int min;						// Minimum expected value
   double (*conv)(int, int);		// Convert to human numbers
} config_t;

// Convertion functions used in the config
// These should not be called on their own
double stdConv(int x, int rounddec);
double StatusLambdaV2Conv(int x, int rounddec);
double airAndWaterTempConv(int x, int rounddec);
double potmeterConvert(int x, int rounddec);
double rpmConv(int x, int rounddec);
double mBarConv(int x, int rounddec);
double batteryConv(int x, int rounddec);
double StatusLambdaV2Conv2(int x, int rounddec);
double InjectorAndIgnitionTimeConv(int x, int rounddec);
double GXGYGZconv(int x, int rounddec);
double gearboardTempConv(int x, int rounddec);
double waterInOutletTemoConv(int x, int rounddec);
double gearNeutralConv(int x, int rounddec);


#define INIT_CONFIG	{ 														\
	{"Empty", 0, 2, 0, 9999, 0, &stdConv},									\
	{"Fuel Press.", 1, 2, 16, 9999, 0, &stdConv},							\
	{"StatusLapCount", 2, 2, 16, 9999, 0, &stdConv},						\
	{"StatusInjSum", 3, 2, 16, 9999, 0, &stdConv},							\
	{"LastGearShift", 4, 2, 16, 9999, 0, &stdConv},							\
	{"MotorOilTemp", 5, 2, 16, 9999, 0, &stdConv},							\
	{"OilPressure", 6, 2, 16, 9999, 0, &stdConv},							\
	{"StatusTime", 7, 2, 16, 9999, 0, &stdConv},							\
	{"StatusLapTime", 8, 2, 16, 9999, 0, &stdConv},							\
	{"GearOilTemp", 9, 2, 16, 9999, 0, &stdConv},							\
	{"StatusTraction", 10, 2, 16, 9999, 0, &stdConv},						\
	{"StatusGas", 11, 2, 16, 9999, 0, &stdConv},							\
	{"StatusLambdaV2", 12, 2, 16, 9999, 0, &StatusLambdaV2Conv},			\
	{"StatusCamTrigP1", 13, 2, 16, 9999, 0, &stdConv},						\
	{"StatusCamTrigP2", 14, 2, 16, 9999, 0, &stdConv},						\
	{"StatusChokerAdd", 15, 2, 16, 9999, 0, &stdConv},						\
	{"StatusLambdaPWM", 16, 2, 16, 9999, 0, &stdConv},						\
	{"WaterMotor temp.", 17, 0, 16, 150, 0, &airAndWaterTempConv},			\
	{"ManifoldAir temp.", 18, 1, 16, 120, 0, &airAndWaterTempConv},			\
	{"Potmeter (0-100%)", 19, 2, 16, 100, 0, &potmeterConvert},				\
	{"RPM", 20, 0, 16, 15000, 0, &rpmConv},									\
	{"TriggerErr", 21, 2, 16, 9999, 0, &stdConv},							\
	{"CamAngle1", 22, 2, 16, 9999, 0, &stdConv},							\
	{"CamAngle2", 23, 2, 16, 9999, 0, &stdConv},							\
	{"RoadSpeed (km/h)", 24, 2, 16, 9999, 0, &stdConv},						\
	{"Manifold press. (mBar)", 25, 2, 16, 1300, 0, &mBarConv},				\
	{"Batt. volt", 26, 1, 16, 20, 0, &batteryConv},							\
	{"Lambda (<1 => Rich)", 27, 2, 16, 2, 0, &StatusLambdaV2Conv2},			\
	{"Load", 28, 2, 16, 9999, 0, &stdConv},									\
	{"InjectorTime", 29, 2, 16, 9999, 0, &InjectorAndIgnitionTimeConv},		\
	{"IgnitionTime", 29, 2, 16, 9999, 0, &InjectorAndIgnitionTimeConv},		\
	{"DwellTime", 31, 2, 16, 9999, 0, &stdConv},							\
	{"GX", 32, 2, 16, 2, -2, &GXGYGZconv},									\
	{"GY", 33, 2, 16, 2, -2, &GXGYGZconv},									\
	{"GZ", 34, 2, 16, 2, -2, &GXGYGZconv},									\
	{"MotorFlags", 35, 2, 8, 9999, 0, &stdConv},							\
	{"OutBits", 36, 2, 8, 9999, 0, &stdConv},								\
	{"Time", 37, 2, 8, 9999, 0, &stdConv},									\
	{"GearChange", 38, 2, 8, 9999, 0, &stdConv},							\
	{"FWheelL", 39, 2, 16, 9999, 0, &stdConv},								\
	{"FWheelL", 40, 2, 16, 9999, 0, &stdConv},								\
	{"BWheelL", 41, 2, 16, 9999, 0, &stdConv},								\
	{"BWheelR", 42, 2, 16, 9999, 0, &stdConv},								\
																			\
	{"GearBoard temp.", 52, 2, 16, 50, 0, &gearboardTempConv},				\
																			\
	{"OilPress (0 = Low)", 56, 2, 16, 1024, 0, &stdConv},					\
	{"WaterInlet temp.", 57, 0, 16, 150, 0, &waterInOutletTemoConv},		\
	{"WaterOutlet temp.", 58, 0, 16, 150, 0, &waterInOutletTemoConv},		\
	{"GPS-sattelites", 59, 2, 8, 10, 0, &stdConv},							\
	{"GPS-speed (km/h)", 60, 2, 16, 120, 0, &stdConv},						\
	{"GPS-time-h", 61, 2, 16, 25, 0, &stdConv},								\
	{"GPS-time-m", 62, 2, 16, 61, 0, &stdConv},								\
	{"GPS-time-s", 63, 2, 16, 61, 0, &stdConv},								\
	{"GearNeutral", 64, 2, 16, 1, 0, &gearNeutralConv},						\
	{"GearEst", 65, 2, 16, 7, 0, &stdConv},									\
	{"Debug", 66, 2, 16, 9999, 0, &stdConv},								\
	{"ValueIdLength", 67, 2, 16, 9999, 0, &stdConv},						\
}																			\

#define CONFIG_LENGTH 54

typedef struct parser_t {
	int package_start_counter; // how many of the start sequence bytes have we seen
	bool package_start; // Is this the beginning of a package?
	int bytesToRead; // How many bytes do we need to read in this package?
	int valOut; // The final value when all bytes are read

	int confIndex; // index in the sensor config

	int sensorFound; // Have we found a sensor?

	const size_t confLength;
	config_t *config; // sensor configuration, See INIT_CONFIG
} parser_t;

typedef struct sensor_t {
   const char *name;			// Human readable name
   int id;						// Can ID
   int confIndex; 				// Index number in the config array
   double value; 				// Value of the sensor
} sensor_t;


#define PARSER_FOUND 		0
#define PARSER_NEEDNEXT		1
#define PARSER_NOMOREDATA 	2
#define PARSER_NOTHINGTODO	3


#define INIT_PARSER(config) 	{0, 0, -1, 0, -1, 0, ARR_LEN(config), config}


int parseNext(uint8_t dataByte, sensor_t *sensor, parser_t *p);
void canfile2csv(const char *path);

#endif /* CANPARSER_H */
