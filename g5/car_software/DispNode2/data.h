#include <stdint.h>

typedef struct {
	uint16_t rpm;
	uint16_t roadSpeed;
	uint16_t waterTemp;
	uint16_t batteryV;
	uint16_t oilPressure;
	uint16_t GearButDown;
	uint16_t GearButUp;
	uint16_t GearButNeutral;
	uint16_t GearNeutral;
	uint16_t GearEst;
} parameters;

extern parameters params;
extern int gearActive;
