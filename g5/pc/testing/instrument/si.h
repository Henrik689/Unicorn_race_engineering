#ifndef _SI_H_
#define _SI_H_

#include "instrumentDefines.h"
#include "radio.h"
#include <avr/interrupt.h>

#define pDCD _BV(4)

volatile uint8_t siCount;

void siInit(void);
void reqInfo(void);

#endif
