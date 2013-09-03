/*********************************************
 * Function prototypes
 *********************************************/

// UART
void uartinit(void);
void sendtekst(char*);

void counter0Init(void);

// PWM functions
void pwm8Init(void);
void pwm16Init(void);
void PWM_duty_cycle_A_set(unsigned int);
void PWM_duty_cycle_B_set(unsigned int);
void PWM_duty_cycle_A16_set(unsigned int);

// ADC
void adcInit(unsigned int);
void adcStop(void);

// Controllers
void presscontroller(unsigned int);
void KoblingPosController(unsigned int);

// Debugging
void debugging(void);
