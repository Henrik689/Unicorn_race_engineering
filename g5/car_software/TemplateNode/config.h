#ifndef CONFIG_H
#define CONFIG_H

/*********************************************
 * Config header
 *********************************************/

// CPU clock
#define FOSC           110592		// 11,0592 MHz External cristal
#define F_CPU          (FOSC*100)	// Need for AVR GCC

// CAN
#include "compiler.h"
#define CAN_BAUDRATE   250        // in kBit

#endif /* CONFIG_H */
 