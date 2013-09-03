/*********************************************
 * Config header
 *********************************************/

// UART
#define USART_BAUDRATE 230400
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1) 

// CPU clock
#define F_CPU 18432000

#define ADCtotnum 3

// Counter0 prescaler
#define counter0prescale0 0
#define counter0prescale1 1
#define counter0prescale8 2
#define counter0prescale64 3
#define counter0prescale256 4
#define counter0prescale1024 5

// PWM 16 bit duty Max
#define dutymax 2047

// Koblings ADC pos, Min/Max
#define KoblingPosMin 339
#define KoblingPosMax 480

// Mux middle PWM
#define muxMiddle 94
