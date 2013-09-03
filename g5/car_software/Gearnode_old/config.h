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

// UART
#define USART_BAUDRATE 115200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 

// POS Filter
#define FILTERKONSTANTPOS 0.95
// Current Sense Filter
#define FILTERKONSTANTCURRENT 0.95
// PWM Filter
#define FILTERKONSTANTPWM 0.3

// PWM duty Max
#define DUTYMAX 2047

// Buttons
#define GEARUPBUT 0b00000010
#define GEARDOWNBUT 0b00000001
#define GEARNEUBUT 0b00000100

// Gear ADC pos, Min/Max
#define GEARPOSMID 400              // 360
#define GEARPOSMAX GEARPOSMID+290   // 290
#define GEARPOSMIN GEARPOSMID-290   // 290
#define GEARPOSNEUTRALMAX GEARPOSMID+50
#define GEARPOSNEUTRALMIN GEARPOSMID-50
#define GearMiddleDeadZone 20       // 5
#define IGNCUT 50 // Middle + Deadzone + IGNCUT = Ignition cut off /* OLD */

// Force (over current) protection
#define GEARFORCECRITICALMAX 90			// MAX current
#define GEARFORCEMAXTIMEOUT1 40			// Timeout 1, go back (168 = ~ 1 sec)
#define GEARFORCEMAXTIMEOUT2 168		// Timeout 2, turn off (168 = ~ 1 sec)

#define ADCtotnum 2

// Counter0 prescaler
#define counter0prescale0 0
#define counter0prescale1 1
#define counter0prescale8 2
#define counter0prescale64 3
#define counter0prescale256 4
#define counter0prescale1024 5

// PWM 16 bit duty Max
#define dutymax 2047

#define AON PORTA|= (1<<PA0)
#define AOF PORTA&=~ (1<<PA0)

#define BON PORTA|=(1<<PA1)
#define BOF PORTA&=~ (1<<PA1)

#define AENA PORTA|= (1<<PA2)
#define ADIS PORTA&=~ (1<<PA2)

#define BENA PORTA|=(1<<PA3)
#define BDIS PORTA&=~(1<<PA3)

// Ign Cut
#define IgnCutOn PORTE|= (1<<PE4)	//Turn on PE4(måske)
#define IgnCutOff PORTE&=~ (1<<PE4)	//Turn off PE4(måske)

// Gearneutralmeasure
#define GEARNEUTRALMEAS (!((PINE&0b10000000)==0b10000000))

#define CW 0
#define CCW 1

#define UD 1
#define IND 0

// Error msg.
#define ERRORMSG_NOERROR 0
#define ERRORMSG_OVERCURRENT 1
#define ERRORMSG_GEARSTUCK 2
