#ifndef IO_H
#define IO_H

#include <stdint.h>

#define HIGH 	1
#define LOW 	0

enum io_pinmode_t {
	INPUT,
	OUTPUT,
	INPUT_PULLUP
};

int digitalRead(uint8_t inputPinRegister, int pin);
void digitalWrite(volatile uint8_t *port, int pin, int value);

int pinMode(volatile uint8_t *port, int pin, enum io_pinmode_t mode);


#endif /* IO_H */
