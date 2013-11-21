#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/sfr_defs.h> // bit_is_clear()


#include "bitwise.h"
#include "io.h"

int digitalRead(int inputPinRegister, int pin){
	return bit_is_clear(inputPinRegister, pin) != 0 ? HIGH : LOW;
}

void digitalWrite(volatile int *port, int pin, int value){
	if (value == LOW) {
		BIT_CLEAR(*port, pin);
	} else {
		BIT_SET(*port, pin);
	} 
}

static inline volatile int* getDDRXFromPORTX(volatile int* port){
	volatile int *ddr_port = NULL;
	if (*port == PORTA){
		*ddr_port = DDRA;
	} else if (*port == PORTB){
		*ddr_port = DDRB;
	} else if (*port == PORTC){
		*ddr_port = DDRC;
	} else if (*port == PORTD){
		*ddr_port = DDRD;
	} else if (*port == PORTE){
		*ddr_port = DDRE;
	} else if (*port == PORTF){
		*ddr_port = DDRF;
	}
	return ddr_port;
}

int pinMode(volatile int *port, int pin, enum io_pinmode_t mode){
	volatile int *ddr_port = getDDRXFromPORTX(port);
	if( ddr_port == NULL ) return 1;

	if(mode == OUTPUT){
		BIT_SET(*ddr_port, pin);
	} else if((mode == INPUT) || (mode == INPUT_PULLUP)){
		BIT_CLEAR(*ddr_port, pin);
		if(mode == INPUT_PULLUP) BIT_SET(*port, pin);
	}

	return 0;
}

