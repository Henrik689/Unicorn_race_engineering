#ifndef INIT_H
#define INIT_H

#include "config.h" // F_CPU

// UART
#define USART_BAUDRATE 115200
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

// Counter0 Prescaler
#define counter0prescale256 4

// OI
void ioinit(void);

// ADC
void adcInit(unsigned int);

// PWM functions
void pwm16Init2(void);

// UART
void uartinit(void);
void sendtekst(char*);


void counter0Init(void);

#endif /* INIT_H */