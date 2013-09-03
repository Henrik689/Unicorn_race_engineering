/*********************************************
 * Config header
 *********************************************/

// UART
#define USART_BAUDRATE0 230400
#define BAUD_PRESCALE0 (((F_CPU / (USART_BAUDRATE0 * 16UL))) - 1) 

#define USART_BAUDRATE1 4800
#define BAUD_PRESCALE1 (((F_CPU / (USART_BAUDRATE1 * 16UL))) - 1) 

// CPU clock
#define F_CPU 18432000
