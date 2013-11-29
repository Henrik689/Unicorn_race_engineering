/**
* @file io.h
* @brief 
*	Used for simple digital pin IO operations
*/

#ifndef IO_H
#define IO_H

#include <stdint.h>

#define HIGH 	1
#define LOW 	0

#define DDR_PORT(port)	(*(&(port) - 1)) //!< Maps a given PORT to the corrisponding DDR eg. PORTA -> DDRA

/**
* @brief
*	Different modes that the pins
*	can be set to
*
*/
enum io_pinmode_t {
	INPUT,
	OUTPUT,
	INPUT_PULLUP
};

int digitalRead(volatile uint8_t *inputPinRegister, int pin);
void digitalWrite(volatile uint8_t *port, int pin, int value);

int pinMode(volatile uint8_t *port, int pin, enum io_pinmode_t mode);


#endif /* IO_H */
