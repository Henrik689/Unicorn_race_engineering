#ifndef CANPARSER_H
#define CANPARSER_H

#include <stdint.h>

typedef struct config_t {
   const char *name;			// Human readable name
   int id;						// Can ID
   int rounddec;				// Round to this many decimals
   int datalength;				// Length in bits
   int max;						// Maximum expected value
   int min;						// Minimum expected value
   double (*conv)(int, int);		// Convert to human numbers
} config_t;


typedef struct parser_t {
	int package_start_counter; // how many of the start sequence bytes have we seen
	int package_start; // Is this the beginning of a package?
	int bytesToRead; // How many bytes do we need to read in this package?
	int valOut; // The final value when all bytes are read

	int confIndex; // index in the sensor config

	int sensorFound; // Have we found a sensor?
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

#define INIT_PARSER 	{0, 0, -1, 0, -1, 0}
 

int parseNext(uint8_t dataByte, sensor_t *sensor, parser_t *p);

#endif /* CANPARSER_H */
