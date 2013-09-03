#ifndef _GEAR_H_
#define _GEAR_H_

#include "instrumentDefines.h"

#include <avr/interrupt.h>

#define pNeutralIN  _BV(3)
#define pUpD  _BV(3)
#define pDownD _BV(2)

void gearInit(void);
uint8_t calcGear(uint16_t speed, uint8_t rpm);

#endif
