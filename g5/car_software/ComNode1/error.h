#include "config.h"

#ifndef __ERROR_H_
#define __ERROR_H_ 1

extern U8 EcuCommError; // Sættes hvis der 5 gange i træk ikke er kommet rigtigt data fra ECU
extern U8 EcuErrorTmp;	// Tæller de 5 gange med fejl

#endif
