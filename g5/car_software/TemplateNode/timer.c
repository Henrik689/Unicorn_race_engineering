#include <avr/interrupt.h>
#include "timer.h"
#include "bitwise.h"

void timer_setPrescaler(const enum timer_prescalar_t p) {
	switch(p) {
		case PRESCALAR_1:
			BIT_SET(TCCR0A, CS00);
			BIT_CLEAR(TCCR0A, CS01);
			BIT_CLEAR(TCCR0A, CS02);
			break;
		case PRESCALAR_8:
			BIT_CLEAR(TCCR0A, CS00);
			BIT_SET(TCCR0A, CS01);
			BIT_CLEAR(TCCR0A, CS02);
			break;
		case PRESCALAR_64:
			BIT_SET(TCCR0A, CS00);
			BIT_SET(TCCR0A, CS01);
			BIT_CLEAR(TCCR0A, CS02);
			break;
		case PRESCALAR_256:
			BIT_CLEAR(TCCR0A, CS00);
			BIT_CLEAR(TCCR0A, CS01);
			BIT_SET(TCCR0A, CS02);
			break;
		case PRESCALAR_1024:
			BIT_SET(TCCR0A, CS00);
			BIT_CLEAR(TCCR0A, CS01);
			BIT_SET(TCCR0A, CS02);
			break;
	}
}

void timer_setMode(const enum timer_16bit_ConReg timer, const unsigned int Waveform_Generation_Mode) {
	const unsigned int mask = 0x03;

	switch (timer) {
		case TIMER1:
			BIT_CLEAR(TCCR1A, mask);
			BIT_CLEAR(TCCR1B, mask);
			BIT_SET(TCCR1A, BITMASK_CHECK(Waveform_Generation_Mode, mask));
			BIT_SET(TCCR1B, BITMASK_CHECK((Waveform_Generation_Mode >> 2), mask));
			break;
		case TIMER3:
			BIT_CLEAR(TCCR3A, mask);
			BIT_CLEAR(TCCR3B, mask);
			BIT_SET(TCCR3A, BITMASK_CHECK(Waveform_Generation_Mode, mask));
			BIT_SET(TCCR3B, BITMASK_CHECK((Waveform_Generation_Mode >> 2), mask));
			break;
	}
}