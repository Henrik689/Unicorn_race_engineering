/*********************************************
 * Config header
 *********************************************/

/*

OBS !! Op er ikke op i gear, men aktivering af gear armen ud af.
Omvendt er ned ikke ned i gear, men aktivering ind

*/


// UART
#define USART_BAUDRATE 230400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 

// UART Transmit queue
#define UARTQUEUESIZE 7

// CPU clock
#define F_CPU 18432000

#define ADCtotnum 2

// Counter0 prescaler
#define counter0prescale0 0
#define counter0prescale1 1
#define counter0prescale8 2
#define counter0prescale64 3
#define counter0prescale256 4
#define counter0prescale1024 5

// Ign Cut
#define IgnCutOn PORTC|= (1<<PC3)
#define IgnCutOff PORTC&=~ (1<<PC3)

// LED

#define LEDYellowOn PORTC|= (1<<PC2)
#define LEDYellowOff PORTC&=~ (1<<PC2)

#define LEDRed1On PORTC|= (1<<PC1)
#define LEDRed1Off PORTC&=~ (1<<PC1)

#define LEDRed2On PORTC|= (1<<PC0)
#define LEDRed2Off PORTC&=~ (1<<PC0)

// PWM 16 bit duty Max
#define dutymax 2047

#define AON PORTA|= (1<<PA7)
#define AOFF PORTA&=~ (1<<PA7)

#define BON PORTA|=(1<<PA4)
#define BOFF PORTA&=~(1<<PA4)

#define AENA PORTA|= (1<<PA6)
#define ADIS PORTA&=~ (1<<PA6)

#define BENA PORTA|=(1<<PA5)
#define BDIS PORTA&=~(1<<PA5)

#define CW 0
#define CCW 1

#define softwareTrig PORTC^=(1<<PORTC7)

#define GEAROP 1
#define GEARNED 2

// Gear ADC pos, Min/Max
#define GEARPOSMIDDLE 761

#define GEARPOSMAX GEARPOSMIDDLE+42 // 38
#define GEARPOSMIN GEARPOSMIDDLE-44 //-45

#define GEARPOSNEUTRALUP GEARPOSMIDDLE+14
#define GEARPOSNEUTRALDOWN GEARPOSMIDDLE-14

#define GearMiddleDeadZone 4

#define IGNCUT 2 // Middle + Deadzone + IGNCUT = Ignition cut off

// Force (current) safety limit
#define GEARFORCECRITICALMAX 320		// MAX current
#define GEARFORCEMAXTIMEOUT1 80		// Timeout 1, go back
#define GEARFORCEMAXTIMEOUT2 200		// Timeout 2, turn off

// Force Ref
#define GEARFORCEREF 320

// Gear Stock Timeout
#define GEARSTOCKTIMEOUT1 300 //280 pr tick pr sec
#define GEARSTOCKTIMEOUT2 500 

// Current filter
#define FILTERKONSTANT 0.9

#define FILTERKONSTANTPOS 0.9
