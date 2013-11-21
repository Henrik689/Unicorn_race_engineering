#ifndef IO_H
#define IO_H

#define HIGH 	1
#define LOW 	0

enum io_pinmode_t {
	INPUT,
	OUTPUT,
	INPUT_PULLUP
};

int digitalRead(int inputPinRegister, int pin);
void digitalWrite(volatile int *port, int pin, int value);

int pinMode(volatile int *port, int pin, enum io_pinmode_t mode);


#endif /* IO_H */
