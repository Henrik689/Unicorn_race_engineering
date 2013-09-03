/*********************************************
 * Function prototypes
 *********************************************/

// OI
void ioinit(void);

// UART
void uartinit(void);
void sendtekst(char*);

void counter0Init(void);

// PWM functions
void PWM_duty_cycle_A16_set(unsigned int);
void pwm16Init(void);
void pwm16Init2(void);
void servo_direction(unsigned int)

// ADC
void adcInit(unsigned int);
void adcStop(void);


unsigned short int getBut(void);
