#include <avr/interrupt.h>
#include "timer.h"
#include "bitwise.h"

void timer_setPrescaler(enum timer_prescalar_t p) {
	switch(p) {
		case PRESCALAR_1:
			TCCR0A |= (1 << CS00);
			TCCR0A &= ~(1 << CS01);
			TCCR0A &= ~(1 << CS02);
			break;
		case PRESCALAR_8:
			TCCR0A &= ~(1 << CS00);
			TCCR0A |= (1 << CS01);
			TCCR0A &= ~(1 << CS02);
			break;
		case PRESCALAR_64:
			TCCR0A |= (1 << CS00);
			TCCR0A |= (1 << CS01);
			TCCR0A &= ~(1 << CS02);
			break;
		case PRESCALAR_256:
			TCCR0A &= ~(1 << CS00);
			TCCR0A &= ~(1 << CS01);
			TCCR0A |= (1 << CS02);
			break;
		case PRESCALAR_1024:
			TCCR0A |= (1 << CS00);
			TCCR0A &= ~(1 << CS01);
			TCCR0A |= (1 << CS02);
			break;
	}
}

void timer_setMode(enum timer_16bit_ConReg timer, int Waveform_Generation_Mode) {
	switch (timer) {
		case TIMER1:
			TCCR1A &= ~0x03;
			TCCR1B &= ~0x03;
			TCCR1A |= (Waveform_Generation_Mode & 0x03);
			TCCR1B |= ((Waveform_Generation_Mode >> 2) & 0x03);
			break;
		case TIMER3:
			TCCR3A &= ~0x03;
			TCCR3B &= ~0x03;
			TCCR3A |= (Waveform_Generation_Mode & 0x03);
			TCCR3B |= ((Waveform_Generation_Mode >> 2) & 0x03);
			break;
	}
}