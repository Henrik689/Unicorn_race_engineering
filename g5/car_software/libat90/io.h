/**
* @file io.h
* @brief 
*	Used for simple digital pin IO operations
*/

#ifndef IO_H
#define IO_H

#include <stdint.h>

/**
* @brief
*	Represents the two possible
*	digital values
*/
enum io_digital_t {
	LOW = 0,
	HIGH = 1
};

#define DDR_PORT(port)	(*(&(port) - 1)) //!< Maps a given PORT to the corrisponding DDR eg. PORTA -> DDRA
#define PIN_PORT(port)	(*(&(port) - 2)) //!< Maps a given PORT to the corrisponding PIN eg. PORTA -> PINA

/**
* @brief
*	Different modes that the pins
*	can be set to
*/
enum io_pinmode_t {
	INPUT,
	OUTPUT,
	INPUT_PULLUP
};

enum io_digital_t digitalRead(volatile uint8_t *inputPinRegister, int pin);
void digitalWrite(volatile uint8_t *port, int pin, enum io_digital_t value);

int pinMode(volatile uint8_t *port, int pin, enum io_pinmode_t mode);


#endif /* IO_H */
