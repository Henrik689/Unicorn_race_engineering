/*********************************************
 * Config header
 *********************************************/

// CPU clock
#define FOSC           110592		// 11,0592 MHz External cristal
#define F_CPU          (FOSC*100)	// Need for AVR GCC

// CAN
#include "compiler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can_std/at90can_drv.h"
#define CAN_BAUDRATE   250        // in kBit
#define ID_BASE 0x80

// ADC
#define ADCtotnum 2

// UART
#define USART_BAUDRATE 230400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 

// Counter0 prescaler
#define counter0prescale0 0
#define counter0prescale1 1
#define counter0prescale8 2
#define counter0prescale64 3
#define counter0prescale256 4
#define counter0prescale1024 5

// Avg length
#define AVGLENGTH 20
