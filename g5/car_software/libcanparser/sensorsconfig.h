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


 
#endif /* SENSORCONFIG_H */
