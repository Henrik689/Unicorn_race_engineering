#ifndef SENSORCONFIG_H
#define SENSORCONFIG_H

typedef struct config_t {
   const char *name;			// Human readable name
   int id;						// Can ID
   int rounddec;				// Round to this many decimals
   int datalength;				// Length in bits
   int max;						// Maximum expected value
   int min;						// Minimum expected value
   float (*conv)(int, int);		// Convert to human numbers
} config_t;

typedef struct sensor_t {
   const char *name;			// Human readable name
   int id;						// Can ID
   int confIndex; 				// Index number in the config array
   float value; 				// Value of the sensor
} sensor_t;


#define PARSER_FOUND 		0
#define PARSER_NEEDNEXT		1
#define PARSER_NOTHINGTODO	2
 
#endif /* SENSORCONFIG_H */
