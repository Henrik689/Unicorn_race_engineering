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

void servoPosition(unsigned int);

// ADC
void adcInit(unsigned int);
void adcStop(void);

// Hbro
void hbroEnable(unsigned short int);
void motorControl(unsigned short int ret, unsigned int speed);

// Force controller
unsigned int torqueController(unsigned int current);

unsigned short int getBut(void);
