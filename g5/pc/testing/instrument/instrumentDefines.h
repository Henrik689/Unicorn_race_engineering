#ifndef _INSTRUMENT_DEF_H_
#define _INSTRUMENT_DEF_H_

#define F_CPU 4000000

#define true 1
#define false 0

#define cltCold (120-75)*26 /* cold @ 75*C */
#define cltWarn (120-110)*26
#define cltStop (120-115)*26

#include <avr/io.h>
#include <stdint.h>

volatile uint8_t timeDiv, timeOut, preScaler, radioTimeout;

volatile uint16_t newRPM, newSpeed, newCLT, newOilP;

volatile struct{
  uint8_t refresh: 1;
  uint8_t newMeasure: 1;
  uint8_t reqInfo: 1;
  uint8_t online: 1;
  uint8_t reply: 1;
  uint8_t forward: 1;
	uint8_t timeOut: 1;
} flags;

#endif
