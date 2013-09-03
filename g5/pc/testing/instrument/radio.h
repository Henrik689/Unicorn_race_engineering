#ifndef _RADIO_H_
#define _RADIO_H_

#include "instrumentDefines.h"
#include <util/delay.h>

#define pConfigB _BV(0)
#define pConfigD _BV(7)
#define pResetC _BV(4)
#define pModeC _BV(5)

void radioInit(void);
void radioInit2(void);

#endif
