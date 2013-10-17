#ifndef TIMER_H
#define TIMER_H

enum timer_prescalar_t {
	PRESCALAR_1,
	PRESCALAR_8,
	PRESCALAR_64,
	PRESCALAR_256,
	PRESCALAR_1024
};

enum timer_16bit_ConReg {
	TIMER1,
	TIMER3
};

int Waveform_Generation_Mode;


void timer_setPrescaler(enum timer_prescalar_t p);
void timer_setMode(enum timer_16bit_ConReg, int Waveform_Generation_Mode);

#endif /* TIMER_H */