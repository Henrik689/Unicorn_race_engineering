#ifndef INIT_H
#define INIT_H

// Counter0 Prescaler
#define counter0prescale256 4

// OI
void ioinit(void);

// ADC
void adcInit(unsigned int);

// PWM functions
void pwm16Init2(void);

void counter0Init(void);

#endif /* INIT_H */