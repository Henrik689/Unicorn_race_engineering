#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "instrumentDefines.h"
#include <util/delay.h>

/*
     A
    ---
F |     | B
  |     |
    ---
E |  G  | C
  |     | .
    ---    \P
     D  
*/

#define segE _BV(0)
#define segD _BV(1)
#define segC _BV(2)
#define segP _BV(3)
#define segG _BV(4)
#define segF _BV(5)
#define segA _BV(6)
#define segB _BV(7)

#define disp1 (segB | segC)
#define disp2 (segA | segB | segG | segE | segD)
#define disp3 (segA | segB | segG | segC | segD)
#define disp4 (segF | segB | segG | segC)
#define disp5 (segA | segF | segG | segC | segD)
#define disp6 (segA | segF | segG | segE | segC | segD)
#define disp7 (segA | segB | segC)
#define disp8 (segA | segB | segC | segG | segE | segD | segF)
#define disp9 (segA | segB | segC | segG | segD | segF)
#define disp0 (segA | segB | segC | segE | segD | segF)

#define dispA (segA | segB | segC | segE | segF | segG)
#define dispB (segF | segE | segG | segC | segD)
#define dispC (segA | segF | segE | segD)
#define dispD (segG | segE | segD | segC | segB)
#define dispE (segA | segF | segG | segE | segD)
#define dispF (segA | segF | segG | segE)

#define dispN (segE | segG | segC)

#define pData   _BV(3)
#define pClock  _BV(2)
#define pStr    _BV(1)

#define displayDelay  100

typedef struct{
  uint16_t  rpm:  6;
  uint8_t  flags:  2;
  uint8_t  gear:  4;
}display_t;

void display(uint8_t rpm, uint8_t gear, uint8_t flags);

#endif
